"cnm.kernel"() <{sym_name = "gemm"}> ({
  %0 = cnm.operand "A" : memref<64x256xi32>
  %1 = cnm.operand "B" : memref<256x256xi32>
  %2 = cnm.output "C" : memref<64x256xi32>
  %3 = cnm.alloc : memref<128xi32>
  %4 = cnm.alloc : memref<128xi32>
  %5 = cnm.alloc : memref<128xi32>
  %6 = arith.constant 64 : index
  %7 = arith.constant 2 : index
  %8 = arith.constant 2 : index
  %9 = arith.constant 128 : index
  %10 = arith.constant 256 : index
  %11 = arith.constant 256 : index
  "cnm.for"(%6) <{addressing = "index", fuse_branch = 0 : i8}> ({
  ^bb0(%12: index):
    %13 = cnm.compute "mul"(%12, %10) : (index, index) -> index
    %14 = cnm.compute "mul"(%12, %11) : (index, index) -> index
    "cnm.for"(%7) <{addressing = "index", fuse_branch = 0 : i8}> ({
    ^bb1(%15: index):
      %16 = arith.constant 0 : i32
      "cnm.for"(%9) <{addressing = "index", fuse_branch = 0 : i8}> ({
      ^bb2(%17: index):
        cnm.store %16, %5[%17] level "WRAM" : i32, memref<128xi32>
        cnm.yield
      }) : (index) -> ()
      %18 = cnm.compute "mul"(%15, %9) : (index, index) -> index
      "cnm.for"(%8) <{addressing = "index", fuse_branch = 0 : i8}> ({
      ^bb3(%19: index):
        %20 = cnm.compute "mul"(%19, %9) : (index, index) -> index
        %21 = cnm.compute "add"(%13, %20) : (index, index) -> index
        cnm.dma_load %0[%21], %3 size 128 : i64 : memref<64x256xi32>, memref<128xi32>
        "cnm.for"(%9) <{addressing = "index", fuse_branch = 0 : i8}> ({
        ^bb4(%22: index):
          %23 = cnm.compute "add"(%18, %22) : (index, index) -> index
          %24 = cnm.compute "mul"(%23, %10) : (index, index) -> index
          %25 = cnm.compute "add"(%24, %20) : (index, index) -> index
          cnm.dma_load %1[%25], %4 size 128 : i64 : memref<256x256xi32>, memref<128xi32>
          %26 = cnm.load %5[%22] level "WRAM" : memref<128xi32> -> i32
          %27 = "cnm.for"(%9, %26) <{addressing = "index", fuse_branch = 0 : i8}> ({
          ^bb5(%28: index, %29: i32):
            %30 = cnm.load %3[%28] level "WRAM" : memref<128xi32> -> i32
            %31 = cnm.load %4[%28] level "WRAM" : memref<128xi32> -> i32
            %32 = cnm.compute "mul"(%30, %31) : (i32, i32) -> i32
            %33 = cnm.compute "add"(%29, %32) : (i32, i32) -> i32
            cnm.store %33, %5[%22] level "WRAM" : i32, memref<128xi32>
            cnm.yield %33 : i32
          }) : (index, i32) -> i32
          cnm.yield
        }) : (index) -> ()
        cnm.yield
      }) : (index) -> ()
      %34 = cnm.compute "add"(%14, %18) : (index, index) -> index
      cnm.dma_store %5, %2[%34] size 128 : i64 : memref<128xi32>, memref<64x256xi32>
      cnm.yield
    }) : (index) -> ()
    cnm.yield
  }) : (index) -> ()
  cnm.return %2 : memref<64x256xi32>
}) : () -> ()
