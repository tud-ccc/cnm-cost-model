"cnm.kernel"() <{sym_name = "reduceop"}> ({
  %0 = cnm.operand "A" : memref<1024xi32>
  %1 = cnm.operand "B" : memref<1024xi32>
  %2 = cnm.output "acc" : memref<i32>
  %3 = cnm.alloc : memref<128xi32>
  %4 = cnm.alloc : memref<128xi32>
  %5 = arith.constant 0 : i32
  %6 = arith.constant 8 : index
  %7 = arith.constant 128 : index
  %8 = arith.constant 128 : index
  %9 = "cnm.for"(%6, %5) <{addressing = "index", fuse_branch = 0 : i8}> ({
  ^bb0(%10: index, %11: i32):
    %12 = cnm.compute "mul"(%10, %7) : (index, index) -> index
    cnm.dma_load %0[%12], %3 size 128 : i64 : memref<1024xi32>, memref<128xi32>
    cnm.dma_load %1[%12], %4 size 128 : i64 : memref<1024xi32>, memref<128xi32>
    %13 = "cnm.for"(%8, %11) <{addressing = "pointer", fuse_branch = 1 : i8}> ({
    ^bb1(%14: index, %15: i32):
      %16 = cnm.load %3[%14] level "WRAM" : memref<128xi32> -> i32
      %17 = cnm.load %4[%14] level "WRAM" : memref<128xi32> -> i32
      %18 = cnm.compute "add"(%15, %16) : (i32, i32) -> i32
      %19 = cnm.compute "add"(%18, %17) : (i32, i32) -> i32
      cnm.yield %19 : i32
    }) : (index, i32) -> i32
    cnm.yield %13 : i32
  }) : (index, i32) -> i32
  cnm.store %9, %2[] level "MRAM" : i32, memref<i32>
  cnm.return %2 : memref<i32>
}) : () -> ()
