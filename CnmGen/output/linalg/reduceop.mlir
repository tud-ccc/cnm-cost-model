builtin.module {
  func.func @reduceop(%0: memref<1024xi32>, %1: memref<1024xi32>, %2: memref<i32>) {
    linalg.generic {indexing_maps = [affine_map<(d0) -> (d0)>, affine_map<(d0) -> (d0)>, affine_map<(d0) -> ()>], iterator_types = ["reduction"]} ins(%0, %1 : memref<1024xi32>, memref<1024xi32>) outs(%2 : memref<i32>) {
    ^bb0(%3: i32, %4: i32, %5: i32):
      %6 = arith.addi %5, %3 : i32
      %7 = arith.addi %6, %4 : i32
      linalg.yield %7 : i32
    }
    func.return
  }
}
