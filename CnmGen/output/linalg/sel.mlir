builtin.module {
  func.func @sel(%0: memref<1024xi32>, %1: memref<1024xi32>) {
    linalg.generic {indexing_maps = [affine_map<(d0) -> (d0)>, affine_map<(d0) -> (d0)>], iterator_types = ["parallel"], library_call = "sel"} ins(%0 : memref<1024xi32>) outs(%1 : memref<1024xi32>) {
    ^bb0(%2: i32, %3: i32):
      linalg.yield %2 : i32
    }
    func.return
  }
}
