builtin.module {
  func.func @hst(%0: memref<1024xi32>, %1: memref<256xi32>) {
    linalg.generic {indexing_maps = [affine_map<(d0) -> (d0)>, affine_map<(d0) -> (0)>], iterator_types = ["parallel"], library_call = "hst"} ins(%0 : memref<1024xi32>) outs(%1 : memref<256xi32>) {
    ^bb0(%2: i32, %3: i32):
      linalg.yield %3 : i32
    }
    func.return
  }
}
