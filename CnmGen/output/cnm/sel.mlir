"cnm.kernel"() <{sym_name = "sel"}> ({
  %0 = cnm.operand "x" : memref<1024xi32>
  %1 = cnm.output "y" : memref<1024xi32>
  %2 = cnm.output "count" : memref<32xi32>
  %3 = cnm.alloc : memref<128xi32>
  %4 = cnm.alloc : memref<128xi32>
  %5 = cnm.alloc : memref<4xi32>
  %6 = arith.constant 0 : index
  %7 = arith.constant 8 : index
  %8 = arith.constant 128 : index
  %9 = arith.constant 128 : index
  %10 = arith.constant 0 : i32
  %11 = arith.constant 1 : index
  %12 = arith.constant 4 : index
  "cnm.for"(%7) <{addressing = "index", fuse_branch = 0 : i8}> ({
  ^bb0(%13: index):
    %14 = cnm.compute "mul"(%13, %8) : (index, index) -> index
    cnm.dma_load %0[%14], %3 size 128 : i64 : memref<1024xi32>, memref<128xi32>
    %15 = "cnm.for"(%9, %6) <{addressing = "pointer", fuse_branch = 0 : i8}> ({
    ^bb1(%16: index, %17: index):
      %18 = cnm.load %3[%16] level "WRAM" : memref<128xi32> -> i32
      %19 = cnm.compute "gt"(%18, %10) : (i32, i32) -> i1
      %20 = "cnm.if"(%19) ({
        cnm.store %18, %4[%17] level "WRAM" : i32, memref<128xi32>
        %21 = cnm.compute "add"(%17, %11) : (index, index) -> index
        cnm.yield %21 : index
      }, {
        cnm.yield %17 : index
      }) : (i1) -> index
      cnm.yield %20 : index
    }) : (index, index) -> index
    %22 = cnm.compute "mul"(%13, %12) : (index, index) -> index
    cnm.dma_store %4, %1[%14] size 128 : i64 : memref<128xi32>, memref<1024xi32>
    cnm.store %15, %5[%6] level "WRAM" : index, memref<4xi32>
    cnm.dma_store %5, %2[%22] size 4 : i64 : memref<4xi32>, memref<32xi32>
    cnm.yield
  }) : (index) -> ()
  cnm.return %1, %2 : memref<1024xi32>, memref<32xi32>
}) : () -> ()
