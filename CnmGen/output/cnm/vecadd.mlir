"cnm.kernel"() <{sym_name = "vecadd"}> ({
  %0 = cnm.operand "A" : memref<1024xi32>
  %1 = cnm.operand "B" : memref<1024xi32>
  %2 = cnm.output "C" : memref<1024xi32>
  %3 = cnm.alloc : memref<128xi32>
  %4 = cnm.alloc : memref<128xi32>
  %5 = cnm.alloc : memref<128xi32>
  %6 = arith.constant 8 : index
  "cnm.for"(%6) <{addressing = "index", fuse_branch = 0 : i8}> ({
  ^bb0(%7: index):
    %8 = arith.constant 128 : index
    %9 = cnm.compute "mul"(%7, %8) : (index, index) -> index
    cnm.dma_load %0[%9], %3 size 128 : i64 : memref<1024xi32>, memref<128xi32>
    cnm.dma_load %1[%9], %4 size 128 : i64 : memref<1024xi32>, memref<128xi32>
    %10 = arith.constant 128 : index
    "cnm.for"(%10) <{addressing = "index", fuse_branch = 0 : i8}> ({
    ^bb1(%11: index):
      %12 = cnm.load %3[%11] level "WRAM" : memref<128xi32> -> i32
      %13 = cnm.load %4[%11] level "WRAM" : memref<128xi32> -> i32
      %14 = cnm.compute "add"(%12, %13) : (i32, i32) -> i32
      cnm.store %14, %5[%11] level "WRAM" : i32, memref<128xi32>
      cnm.yield
    }) : (index) -> ()
    cnm.dma_store %5, %2[%9] size 128 : i64 : memref<128xi32>, memref<1024xi32>
    cnm.yield
  }) : (index) -> ()
  cnm.return %2 : memref<1024xi32>
}) : () -> ()
