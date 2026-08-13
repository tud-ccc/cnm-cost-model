import torch
import torch.nn as nn
import torchaudio.models
import math
import csv

def conv_to_gemm(input_shape, weight_shape, stride=1, padding=0):
    """Converts 1D convolution to GEMM format."""
    batch, in_channels, time = input_shape  # 1D conv: (batch, channels, time)
    out_channels, _, kernel_size = weight_shape  # Conv1D weight shape

    # Compute output time dimension
    time_out = (time - kernel_size + 2 * padding) // stride + 1

    # im2col transformation (1D case)
    im2col_shape = (in_channels * kernel_size, time_out)

    # GEMM Shape: (out_channels, in_channels * kernel_size) x (in_channels * kernel_size, time_out)
    gemm_shape = (im2col_shape[0],out_channels, im2col_shape[1])

    return {
        "Type": "CONV",
        "Input Shape": input_shape,
        "Output Shape": (batch, out_channels, time_out),
        "Weight Shape": weight_shape,
        "GEMM Shape": gemm_shape,
    }


def lstm_to_gemm(input_size, hidden_size):
    """Converts LSTM to GEMM format."""
    gemm_shape = (4 * hidden_size, input_size + hidden_size)  # 4 gates
    return {
        "Type": "LSTM",
        "LSTM GEMM Shape": gemm_shape,
        "Weight Shape": (4 * hidden_size, input_size + hidden_size),
    }

model = torchaudio.models.emformer_rnnt_base(1)
acceleratable_layers_info = {}

# Hook function for Convolution layers
def conv_hook(module, input, output):
    input_shape = input[0].shape
    weight_shape = module.weight.shape
    stride = module.stride[0]
    padding = module.padding[0]

    # Convert convolution to GEMM
    gemm_details = conv_to_gemm(input_shape, weight_shape, stride, padding)
    acceleratable_layers_info[module] = gemm_details


# Hook function for LSTM layers
def lstm_hook(module, input, output):
    input_size = module.input_size
    hidden_size = module.hidden_size

    # Convert LSTM to GEMM
    gemm_details = lstm_to_gemm(input_size, hidden_size)
    acceleratable_layers_info[module] = gemm_details


# Hook function for Fully Connected layers
def fc_hook(module, input, output):
    input_shape = input[0].shape
    output_shape = output.shape
    acceleratable_layers_info[module] = {
        "Type": "FC",
        "Input Shape": input_shape,
        "Output Shape": output_shape,
        "GEMM Shape": (output_shape[-1], input_shape[-1]),
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


num_symbols = 30
model = torchaudio.models.emformer_rnnt_base(num_symbols=num_symbols)
hooks = []

for name, layer in model.named_modules():
    if isinstance(layer, nn.Conv1d):  # Wav2Letter uses Conv1D
        hooks.append(layer.register_forward_hook(conv_hook))
    elif isinstance(layer, nn.LSTM):
        hooks.append(layer.register_forward_hook(lstm_hook))
    elif isinstance(layer, nn.Linear):
        hooks.append(layer.register_forward_hook(fc_hook))
    elif isinstance(layer, nn.ReLU):
        hooks.append(layer.register_forward_hook(relu_hook))



csv_file = open("rnnt.csv", "w")
writer = csv.writer(csv_file, delimiter=',')
i = 1
while i <= 1024:
    acceleratable_layers_info = {}
    row = []
    batch_size = i 
    time_steps = 100  # Number of time steps in input sequence
    feature_dim = 80  # Feature dimension (Mel-spectrogram size)
    sources = torch.randn(batch_size, time_steps, feature_dim)
    source_lengths = torch.tensor([time_steps], dtype=torch.int64)
    targets = torch.randint(low=0, high=num_symbols, size=(batch_size, 20), dtype=torch.int64)
    target_lengths = torch.tensor([20], dtype=torch.int64)
    logits, _, _, _ = model(sources, source_lengths, targets, target_lengths)
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
