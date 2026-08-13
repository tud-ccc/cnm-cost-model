"cnm.kernel"() <{sym_name = "gemv"}> ({
  %0 = cnm.operand "A" : memref<64x256xi32>
  %1 = cnm.operand "x" : memref<256xi32>
  %2 = cnm.output "y" : memref<64xi32>
  %3 = cnm.alloc : memref<128xi32>
  %4 = cnm.alloc : memref<128xi32>
  %5 = arith.constant 64 : index
  %6 = arith.constant 2 : index
  %7 = arith.constant 128 : index
  %8 = arith.constant 128 : index
  %9 = arith.constant 256 : index
  "cnm.for"(%5) <{addressing = "index", fuse_branch = 0 : i8}> ({
  ^bb0(%10: index):
    %11 = arith.constant 0 : i32
    %12 = cnm.compute "mul"(%10, %9) : (index, index) -> index
    %13 = "cnm.for"(%6, %11) <{addressing = "index", fuse_branch = 0 : i8}> ({
    ^bb1(%14: index, %15: i32):
      %16 = cnm.compute "mul"(%14, %7) : (index, index) -> index
      %17 = cnm.compute "add"(%12, %16) : (index, index) -> index
      cnm.dma_load %0[%17], %3 size 128 : i64 : memref<64x256xi32>, memref<128xi32>
      cnm.dma_load %1[%16], %4 size 128 : i64 : memref<256xi32>, memref<128xi32>
      %18 = "cnm.for"(%8, %15) <{addressing = "pointer", fuse_branch = 1 : i8}> ({
      ^bb2(%19: index, %20: i32):
        %21 = cnm.load %3[%19] level "WRAM" : memref<128xi32> -> i32
        %22 = cnm.load %4[%19] level "WRAM" : memref<128xi32> -> i32
        %23 = cnm.compute "mul"(%21, %22) : (i32, i32) -> i32
        %24 = cnm.compute "add"(%20, %23) : (i32, i32) -> i32
        cnm.yield %24 : i32
      }) : (index, i32) -> i32
      cnm.yield %18 : i32
    }) : (index, i32) -> i32
    cnm.store %13, %2[%10] level "MRAM" : i32, memref<64xi32>
    cnm.yield
  }) : (index) -> ()
  cnm.return %2 : memref<64xi32>
}) : () -> ()
