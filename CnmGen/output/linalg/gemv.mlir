builtin.module {
  func.func @gemv(%0: memref<64x256xi32>, %1: memref<256xi32>, %2: memref<64xi32>) {
    linalg.generic {indexing_maps = [affine_map<(d0, d1) -> (d0, d1)>, affine_map<(d0, d1) -> (d1)>, affine_map<(d0, d1) -> (d0)>], iterator_types = ["parallel", "reduction"]} ins(%0, %1 : memref<64x256xi32>, memref<256xi32>) outs(%2 : memref<64xi32>) {
    ^bb0(%3: i32, %4: i32, %5: i32):
      %6 = arith.muli %3, %4 : i32
      %7 = arith.addi %6, %5 : i32
      linalg.yield %7 : i32
    }
    func.return
  }
}
