builtin.module {
  func.func @gemm(%0: memref<64x256xi32>, %1: memref<256x256xi32>, %2: memref<64x256xi32>) {
    linalg.matmul ins(%0, %1 : memref<64x256xi32>, memref<256x256xi32>) outs(%2 : memref<64x256xi32>)
    func.return
  }
}
