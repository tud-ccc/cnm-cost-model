"cnm.kernel"() <{sym_name = "hst"}> ({
  %0 = cnm.operand "x" : memref<1024xi32>
  %1 = cnm.output "bins" : memref<256xi32>
  %2 = cnm.alloc : memref<128xi32>
  %3 = cnm.alloc : memref<256xi32>
  %4 = arith.constant 256 : index
  %5 = arith.constant 0 : i32
  "cnm.for"(%4) <{addressing = "index", fuse_branch = 0 : i8}> ({
  ^bb0(%6: index):
    cnm.store %5, %3[%6] level "WRAM" : i32, memref<256xi32>
    cnm.yield
  }) : (index) -> ()
  %7 = arith.constant 8 : index
  %8 = arith.constant 128 : index
  %9 = arith.constant 128 : index
  %10 = arith.constant 256 : i32
  %11 = arith.constant 12 : i32
  %12 = arith.constant 1 : i32
  "cnm.for"(%7) <{addressing = "index", fuse_branch = 0 : i8}> ({
  ^bb1(%13: index):
    %14 = cnm.compute "mul"(%13, %8) : (index, index) -> index
    cnm.dma_load %0[%14], %2 size 128 : i64 : memref<1024xi32>, memref<128xi32>
    "cnm.for"(%9) <{addressing = "pointer", fuse_branch = 0 : i8}> ({
    ^bb2(%15: index):
      %16 = cnm.load %2[%15] level "WRAM" : memref<128xi32> -> i32
      %17 = cnm.compute "mul"(%16, %10) : (i32, i32) -> i32
      %18 = cnm.compute "shr"(%17, %11) : (i32, i32) -> i32
      %19 = cnm.compute "index_cast"(%18) : (i32) -> index
      %20 = cnm.load %3[%19] level "WRAM" : memref<256xi32> -> i32
      %21 = cnm.compute "add"(%20, %12) : (i32, i32) -> i32
      cnm.store %21, %3[%19] level "WRAM" : i32, memref<256xi32>
      cnm.yield
    }) : (index) -> ()
    cnm.yield
  }) : (index) -> ()
  %22 = arith.constant 0 : index
  cnm.dma_store %3, %1[%22] size 256 : i64 : memref<256xi32>, memref<256xi32>
  cnm.return %1 : memref<256xi32>
}) : () -> ()
