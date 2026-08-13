import torch
import torch.nn as nn
import torchvision.models as models
import math
import csv 

def conv_to_gemm(input_shape, weight_shape, stride=1, padding=0):
    """
    Converts convolution operation to matrix-matrix multiplication (GEMM).

    Args:
        input_shape (tuple): (batch, in_channels, H, W)
        weight_shape (tuple): (out_channels, in_channels, kernel_H, kernel_W)
        stride (int): Stride size
        padding (int): Padding size

    Returns:
        Dict with GEMM dimensions
    """
    batch, in_channels, H, W = input_shape
    out_channels, _, kernel_H, kernel_W = weight_shape

    # Compute output spatial dimensions
    H_out = (H - kernel_H + 2 * padding) // stride + 1
    W_out = (W - kernel_W + 2 * padding) // stride + 1

    # im2col transformation: Flatten input patches into a matrix
    im2col_shape = (in_channels * kernel_H * kernel_W, H_out * W_out)

    # GEMM Shape: (out_channels, in_channels * kernel_H * kernel_W) x (in_channels * kernel_H * kernel_W, H_out * W_out)
    gemm_shape = (im2col_shape[0], out_channels, im2col_shape[1])

    return {
        "Type": "CONV",
        "Input Shape": input_shape,
        "Output Shape": (batch, out_channels, H_out, W_out),
        "Weight Shape": weight_shape,
        "GEMM Shape": gemm_shape
    }

# Load AlexNet
alexnet = models.alexnet()

# Define a dummy input tensor (batch=1, channels=3, height=224, width=224)

# Dictionary to store GEMM dimensions
acceleratable_layers_info = {}

# Hook function to extract GEMM dimensions
def conv_hook(module, input, output):
    input_shape = input[0].shape
    weight_shape = module.weight.shape
    stride = module.stride[0]
    padding = module.padding[0]

    # Convert convolution to GEMM dimensions
    gemm_details = conv_to_gemm(input_shape, weight_shape, stride, padding)
    acceleratable_layers_info[module] = gemm_details

def fc_hook(module, input, output):
    input_shape = input[0].shape
    output_shape = output.shape
    acceleratable_layers_info[module] = {
        "Type": "FC",
        "Input Shape": input_shape,
        "Output Shape": output_shape,
        "GEMM Shape": (output_shape[-1], input_shape[-1])
    }

def relu_hook(module, input, output):
    input_shape = input[0].shape
    output_shape = output.shape
    acceleratable_layers_info[module] = {
        "Type": "RELU",
        "Input Shape": input_shape,
        "Output Shape": output_shape,
        "RELU Shape": (input_shape)
    }

# Register hooks for convolutional and fully connected layers
hooks = []
for name, layer in alexnet.named_modules():
    if isinstance(layer, nn.Conv2d):
        hooks.append(layer.register_forward_hook(conv_hook))
    elif isinstance(layer, nn.Linear):
        hooks.append(layer.register_forward_hook(fc_hook))
    elif isinstance(layer, nn.ReLU):
        hooks.append(layer.register_forward_hook(relu_hook))

# Forward pass to compute GEMM dimensions


# Print extracted GEMM dimensions
csv_file = open("alex_net_ops.csv", "w")
writer = csv.writer(csv_file, delimiter=',')
i = 1
while i <= 1024:
    acceleratable_layers_info = {}
    dummy_input = torch.randn(i, 3, 224, 224)
    alexnet(dummy_input)
    row = []
    for layer, details in acceleratable_layers_info.items():
        if details["Type"] == "FC":
            row += ["mm",list(details["Input Shape"])[0], list(details["Input Shape"])[1], (list(details["Output Shape"])[-1])]
        if details["Type"] == "CONV":
            row += ["mm" , list(details["GEMM Shape"])[0], list(details["GEMM Shape"])[1] , list(details["GEMM Shape"])[2]]
        if details["Type"] == "RELU":
            row += ["relu", math.prod(list(details["RELU Shape"]))]
    writer.writerow(row)
    i*= 2

# Remove hooks
for hook in hooks:
    hook.remove()