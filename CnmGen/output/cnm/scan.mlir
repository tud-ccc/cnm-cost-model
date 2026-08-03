"cnm.kernel"() <{sym_name = "scan"}> ({
  %0 = cnm.operand "x" : memref<1024xi32>
  %1 = cnm.output "y" : memref<1024xi32>
  %2 = cnm.alloc : memref<128xi32>
  %3 = cnm.alloc : memref<16xi32>
  %4 = arith.constant 0 : index
  %5 = arith.constant 0 : i32
  %6 = arith.constant 8 : index
  %7 = arith.constant 128 : index
  %8 = arith.constant 128 : index
  %9 = arith.constant 16 : index
  %10 = arith.constant 8 : index
  %11 = "cnm.for"(%6, %5) <{addressing = "index", fuse_branch = 0 : i8}> ({
  ^bb0(%12: index, %13: i32):
    %14 = cnm.compute "mul"(%12, %7) : (index, index) -> index
    cnm.dma_load %0[%14], %2 size 128 : i64 : memref<1024xi32>, memref<128xi32>
    %15 = "cnm.for"(%8, %13) <{addressing = "pointer", fuse_branch = 1 : i8}> ({
    ^bb1(%16: index, %17: i32):
      %18 = cnm.load %2[%16] level "WRAM" : memref<128xi32> -> i32
      %19 = cnm.compute "add"(%17, %18) : (i32, i32) -> i32
      cnm.yield %19 : i32
    }) : (index, i32) -> i32
    cnm.yield %15 : i32
  }) : (index, i32) -> i32
  cnm.store %11, %3[%4] level "WRAM" : i32, memref<16xi32>
  cnm.barrier
  %20 = "cnm.for"(%9, %5) <{addressing = "index", fuse_branch = 0 : i8}> ({
  ^bb2(%21: index, %22: i32):
    %23 = cnm.compute "lt"(%21, %10) : (index, index) -> i1
    %24 = "cnm.if"(%23) ({
      %25 = cnm.load %3[%21] level "WRAM" : memref<16xi32> -> i32
      %26 = cnm.compute "add"(%22, %25) : (i32, i32) -> i32
      cnm.yield %26 : i32
    }, {
      cnm.yield %22 : i32
    }) : (i1) -> i32
    cnm.yield %24 : i32
  }) : (index, i32) -> i32
  %27 = "cnm.for"(%6, %20) <{addressing = "index", fuse_branch = 0 : i8}> ({
  ^bb3(%28: index, %29: i32):
    %30 = cnm.compute "mul"(%28, %7) : (index, index) -> index
    cnm.dma_load %0[%30], %2 size 128 : i64 : memref<1024xi32>, memref<128xi32>
    %31 = "cnm.for"(%8, %29) <{addressing = "index", fuse_branch = 0 : i8}> ({
    ^bb4(%32: index, %33: i32):
      %34 = cnm.load %2[%32] level "WRAM" : memref<128xi32> -> i32
      %35 = cnm.compute "add"(%33, %34) : (i32, i32) -> i32
      cnm.store %35, %2[%32] level "WRAM" : i32, memref<128xi32>
      cnm.yield %35 : i32
    }) : (index, i32) -> i32
    cnm.dma_store %2, %1[%30] size 128 : i64 : memref<128xi32>, memref<1024xi32>
    cnm.yield %31 : i32
  }) : (index, i32) -> i32
  cnm.return %1 : memref<1024xi32>
}) : () -> ()
