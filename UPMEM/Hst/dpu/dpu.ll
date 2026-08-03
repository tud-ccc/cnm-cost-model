; ModuleID = 'dpu.c'
source_filename = "dpu.c"
target datalayout = "e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i32:32:32-i64:64:64-n32"
target triple = "dpu-upmem-dpurte"

module asm ".section .data.my_barrier"
module asm ".type my_barrier,@object"
module asm ".globl my_barrier"
module asm ".p2align 2"
module asm "my_barrier:"
module asm ".byte 0xFF"
module asm ".byte 1"
module asm ".byte 1"
module asm ".byte __atomic_bit_barrier_my_barrier"
module asm ".size my_barrier, 4"
module asm ".text"

%struct.barrier_t = type { i8, i8, i8, i8 }
%struct.dpu_arguments_t = type { [1 x i32], [1 x i32], [1 x i32], i32 }

@MAP_RANK_DIMS = dso_local local_unnamed_addr global [1 x i32] [i32 1], align 4, !dbg !0
@MAP_DPU_DIMS = dso_local local_unnamed_addr global [1 x i32] [i32 1], align 4, !dbg !14
@MAP_THREAD_DIMS = dso_local local_unnamed_addr global [1 x i32] [i32 24], align 4, !dbg !20
@MAP_ITER_DIMS = dso_local local_unnamed_addr global [1 x i32] [i32 524288], align 4, !dbg !22
@MAP_BUFFER_SIZES = dso_local local_unnamed_addr global [1 x i32] [i32 128], align 4, !dbg !24
@my_barrier = external dso_local global %struct.barrier_t, align 1
@DPU_INPUT_ARGUMENTS = dso_local global %struct.dpu_arguments_t zeroinitializer, section ".dpu_host", align 8, !dbg !26
@__sys_used_mram_end = external dso_local addrspace(255) global [0 x i8], align 8
@nb_cycle = dso_local global i32 0, section ".dpu_host", align 8, !dbg !36
@__atomic_bit_barrier_my_barrier = dso_local local_unnamed_addr global i8 0, section ".atomic", align 1, !dbg !38
@llvm.used = appending global [2 x i8*] [i8* bitcast (%struct.dpu_arguments_t* @DPU_INPUT_ARGUMENTS to i8*), i8* bitcast (i32* @nb_cycle to i8*)], section "llvm.metadata"

; Function Attrs: nounwind
define dso_local i32 @main() local_unnamed_addr #0 !dbg !46 {
  %1 = tail call i32 @llvm.dpu.tid.i32() #5, !dbg !75
  call void @llvm.dbg.value(metadata i32 %1, metadata !51, metadata !DIExpression()), !dbg !83
  %2 = icmp eq i32 %1, 0, !dbg !84
  br i1 %2, label %3, label %5, !dbg !86

3:                                                ; preds = %0
  %4 = tail call i8* @mem_reset() #5, !dbg !87
  br label %5, !dbg !89

5:                                                ; preds = %3, %0
  tail call void @barrier_wait(%struct.barrier_t* nonnull @my_barrier) #5, !dbg !90
  %6 = load i32, i32* getelementptr inbounds (%struct.dpu_arguments_t, %struct.dpu_arguments_t* @DPU_INPUT_ARGUMENTS, i32 0, i32 2, i32 0), align 8, !dbg !91, !tbaa !92
  call void @llvm.dbg.value(metadata i32 %6, metadata !52, metadata !DIExpression()), !dbg !83
  %7 = load i32, i32* getelementptr inbounds (%struct.dpu_arguments_t, %struct.dpu_arguments_t* @DPU_INPUT_ARGUMENTS, i32 0, i32 1, i32 0), align 4, !dbg !96, !tbaa !92
  call void @llvm.dbg.value(metadata i32 %7, metadata !53, metadata !DIExpression()), !dbg !83
  %8 = icmp ult i32 %1, %7, !dbg !97
  br i1 %8, label %9, label %68, !dbg !99

9:                                                ; preds = %5
  %10 = load i32, i32* getelementptr inbounds (%struct.dpu_arguments_t, %struct.dpu_arguments_t* @DPU_INPUT_ARGUMENTS, i32 0, i32 0, i32 0), align 8, !dbg !100, !tbaa !92
  call void @llvm.dbg.value(metadata i32 %10, metadata !54, metadata !DIExpression()), !dbg !83
  %11 = load i32, i32* getelementptr inbounds (%struct.dpu_arguments_t, %struct.dpu_arguments_t* @DPU_INPUT_ARGUMENTS, i32 0, i32 3), align 4, !dbg !101, !tbaa !102
  call void @llvm.dbg.value(metadata i32 %11, metadata !55, metadata !DIExpression()), !dbg !83
  %12 = shl i32 %10, 2, !dbg !104
  call void @llvm.dbg.value(metadata i32 %12, metadata !56, metadata !DIExpression()), !dbg !83
  %13 = tail call i8* @mem_alloc(i32 %12) #5, !dbg !105
  %14 = bitcast i8* %13 to i32*, !dbg !106
  call void @llvm.dbg.value(metadata i32* %14, metadata !57, metadata !DIExpression()), !dbg !83
  %15 = shl i32 %11, 2, !dbg !107
  %16 = tail call i8* @mem_alloc(i32 %15) #5, !dbg !108
  %17 = bitcast i8* %16 to i32*, !dbg !109
  call void @llvm.dbg.value(metadata i32* %17, metadata !58, metadata !DIExpression()), !dbg !83
  call void @llvm.dbg.value(metadata i32 undef, metadata !59, metadata !DIExpression()), !dbg !83
  %18 = mul i32 %7, %6, !dbg !110
  call void @llvm.dbg.value(metadata i32 %18, metadata !60, metadata !DIExpression()), !dbg !83
  call void @llvm.dbg.value(metadata i32 ptrtoint ([0 x i8] addrspace(255)* @__sys_used_mram_end to i32), metadata !61, metadata !DIExpression()), !dbg !83
  call void @llvm.dbg.value(metadata i32 undef, metadata !62, metadata !DIExpression()), !dbg !83
  %19 = shl i32 %1, 2, !dbg !111
  %20 = mul i32 %19, %6, !dbg !112
  %21 = add i32 %20, ptrtoint ([0 x i8] addrspace(255)* @__sys_used_mram_end to i32), !dbg !113
  call void @llvm.dbg.value(metadata i32 %21, metadata !63, metadata !DIExpression()), !dbg !83
  %22 = mul i32 %11, %1, !dbg !114
  %23 = add i32 %22, %18
  %24 = shl i32 %23, 2
  %25 = add i32 %24, ptrtoint ([0 x i8] addrspace(255)* @__sys_used_mram_end to i32), !dbg !115
  call void @llvm.dbg.value(metadata i32 %25, metadata !64, metadata !DIExpression()), !dbg !83
  %26 = udiv i32 %6, %10, !dbg !116
  call void @llvm.dbg.value(metadata i32 %26, metadata !65, metadata !DIExpression()), !dbg !83
  call void @llvm.dbg.value(metadata i32 0, metadata !66, metadata !DIExpression()), !dbg !117
  call void @llvm.dbg.value(metadata i32 %21, metadata !63, metadata !DIExpression()), !dbg !83
  %27 = icmp ugt i32 %10, %6, !dbg !118
  br i1 %27, label %55, label %28, !dbg !119

28:                                               ; preds = %9
  %29 = add i32 %12, -8
  %30 = icmp ult i32 %29, 2041
  %31 = and i32 %10, 1
  %32 = icmp eq i32 %31, 0
  tail call void @llvm.assume(i1 %30) #5, !dbg !83
  tail call void @llvm.assume(i1 %32) #5, !dbg !83
  %33 = icmp eq i32 %10, 0
  %34 = icmp ugt i32 %26, 1
  %35 = select i1 %34, i32 %26, i32 1
  br i1 %33, label %61, label %36, !dbg !119

36:                                               ; preds = %28, %51
  %37 = phi i32 [ %53, %51 ], [ 0, %28 ]
  %38 = phi i32 [ %52, %51 ], [ %21, %28 ]
  call void @llvm.dbg.value(metadata i32 %37, metadata !66, metadata !DIExpression()), !dbg !117
  call void @llvm.dbg.value(metadata i32 %38, metadata !63, metadata !DIExpression()), !dbg !83
  %39 = inttoptr i32 %38 to i8 addrspace(255)*, !dbg !120
  call void @llvm.dbg.value(metadata i8 addrspace(255)* %39, metadata !121, metadata !DIExpression()) #5, !dbg !129
  call void @llvm.dbg.value(metadata i8* %13, metadata !127, metadata !DIExpression()) #5, !dbg !129
  call void @llvm.dbg.value(metadata i32 %12, metadata !128, metadata !DIExpression()) #5, !dbg !129
  tail call void @llvm.dpu.ldma(i8* %13, i8 addrspace(255)* %39, i32 %12) #5, !dbg !131
  call void @llvm.dbg.value(metadata i32 0, metadata !68, metadata !DIExpression()), !dbg !132
  br label %40, !dbg !133

40:                                               ; preds = %36, %40
  %41 = phi i32 [ 0, %36 ], [ %49, %40 ]
  call void @llvm.dbg.value(metadata i32 %41, metadata !68, metadata !DIExpression()), !dbg !132
  %42 = getelementptr inbounds i32, i32* %14, i32 %41, !dbg !134
  %43 = load i32, i32* %42, align 4, !dbg !134, !tbaa !92
  %44 = mul i32 %43, %11, !dbg !135
  %45 = lshr i32 %44, 12, !dbg !136
  call void @llvm.dbg.value(metadata i32 %45, metadata !72, metadata !DIExpression()), !dbg !137
  %46 = getelementptr inbounds i32, i32* %17, i32 %45, !dbg !138
  %47 = load i32, i32* %46, align 4, !dbg !139, !tbaa !92
  %48 = add i32 %47, 1, !dbg !139
  store i32 %48, i32* %46, align 4, !dbg !139, !tbaa !92
  %49 = add nuw nsw i32 %41, 1, !dbg !140
  call void @llvm.dbg.value(metadata i32 %49, metadata !68, metadata !DIExpression()), !dbg !132
  %50 = icmp eq i32 %49, %10, !dbg !141
  br i1 %50, label %51, label %40, !dbg !133, !llvm.loop !142

51:                                               ; preds = %40
  %52 = add i32 %38, %12, !dbg !145
  call void @llvm.dbg.value(metadata i32 %52, metadata !63, metadata !DIExpression()), !dbg !83
  %53 = add nuw i32 %37, 1, !dbg !146
  call void @llvm.dbg.value(metadata i32 %53, metadata !66, metadata !DIExpression()), !dbg !117
  %54 = icmp eq i32 %53, %35, !dbg !118
  br i1 %54, label %55, label %36, !dbg !119, !llvm.loop !147

55:                                               ; preds = %51, %61, %9
  %56 = inttoptr i32 %25 to i8 addrspace(255)*, !dbg !149
  call void @llvm.dbg.value(metadata i8* %16, metadata !150, metadata !DIExpression()) #5, !dbg !155
  call void @llvm.dbg.value(metadata i8 addrspace(255)* %56, metadata !153, metadata !DIExpression()) #5, !dbg !155
  call void @llvm.dbg.value(metadata i32 %15, metadata !154, metadata !DIExpression()) #5, !dbg !155
  %57 = add i32 %15, -8, !dbg !157
  %58 = icmp ult i32 %57, 2041, !dbg !157
  %59 = and i32 %11, 1, !dbg !157
  %60 = icmp eq i32 %59, 0, !dbg !157
  tail call void @llvm.assume(i1 %58) #5, !dbg !158
  tail call void @llvm.assume(i1 %60) #5, !dbg !158
  tail call void @llvm.dpu.sdma(i8* %16, i8 addrspace(255)* %56, i32 %15) #5, !dbg !159
  br label %68

61:                                               ; preds = %28, %61
  %62 = phi i32 [ %66, %61 ], [ 0, %28 ]
  %63 = phi i32 [ %65, %61 ], [ %21, %28 ]
  call void @llvm.dbg.value(metadata i32 %62, metadata !66, metadata !DIExpression()), !dbg !117
  call void @llvm.dbg.value(metadata i32 %63, metadata !63, metadata !DIExpression()), !dbg !83
  %64 = inttoptr i32 %63 to i8 addrspace(255)*, !dbg !120
  call void @llvm.dbg.value(metadata i8 addrspace(255)* %64, metadata !121, metadata !DIExpression()) #5, !dbg !129
  call void @llvm.dbg.value(metadata i8* %13, metadata !127, metadata !DIExpression()) #5, !dbg !129
  call void @llvm.dbg.value(metadata i32 %12, metadata !128, metadata !DIExpression()) #5, !dbg !129
  tail call void @llvm.dpu.ldma(i8* %13, i8 addrspace(255)* %64, i32 %12) #5, !dbg !131
  call void @llvm.dbg.value(metadata i32 0, metadata !68, metadata !DIExpression()), !dbg !132
  %65 = add i32 %63, %12, !dbg !145
  call void @llvm.dbg.value(metadata i32 %65, metadata !63, metadata !DIExpression()), !dbg !83
  %66 = add nuw i32 %62, 1, !dbg !146
  call void @llvm.dbg.value(metadata i32 %66, metadata !66, metadata !DIExpression()), !dbg !117
  %67 = icmp eq i32 %66, %35, !dbg !118
  br i1 %67, label %55, label %61, !dbg !119, !llvm.loop !147

68:                                               ; preds = %5, %55
  ret i32 0, !dbg !160
}

declare !dbg !161 dso_local i8* @mem_reset() local_unnamed_addr #1

declare !dbg !165 dso_local void @barrier_wait(%struct.barrier_t*) local_unnamed_addr #1

declare !dbg !176 dso_local i8* @mem_alloc(i32) local_unnamed_addr #1

; Function Attrs: nounwind readnone
declare i32 @llvm.dpu.tid.i32() #2

; Function Attrs: nofree nosync nounwind willreturn
declare void @llvm.assume(i1 noundef) #3

declare void @llvm.dpu.ldma(i8* writeonly, i8 addrspace(255)* readonly, i32)

declare void @llvm.dpu.sdma(i8* readonly, i8 addrspace(255)* writeonly, i32)

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.value(metadata, metadata, metadata) #4

attributes #0 = { nounwind "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind readnone }
attributes #3 = { nofree nosync nounwind willreturn }
attributes #4 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #5 = { nounwind }

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!42, !43, !44}
!llvm.ident = !{!45}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "MAP_RANK_DIMS", scope: !2, file: !16, line: 2, type: !17, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "clang version 12.0.0 (https://github.com/upmem/llvm-project.git 846fdda8285dcc9b20ee5d2fec9e54dfea6a8928)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, retainedTypes: !5, globals: !13, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "dpu.c", directory: "/home/hamid.farzaneh/CostModelEvaluationRepo/NewEval/Hst/dpu")
!4 = !{}
!5 = !{!6, !7, !10, !11}
!6 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !7, size: 32)
!7 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint32_t", file: !8, line: 48, baseType: !9)
!8 = !DIFile(filename: "/opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/stdlib/stdint.h", directory: "")
!9 = !DIBasicType(name: "unsigned int", size: 32, encoding: DW_ATE_unsigned)
!10 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 32)
!11 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !12, size: 32)
!12 = !DIDerivedType(tag: DW_TAG_const_type, baseType: null)
!13 = !{!0, !14, !20, !22, !24, !26, !36, !38}
!14 = !DIGlobalVariableExpression(var: !15, expr: !DIExpression())
!15 = distinct !DIGlobalVariable(name: "MAP_DPU_DIMS", scope: !2, file: !16, line: 3, type: !17, isLocal: false, isDefinition: true)
!16 = !DIFile(filename: "./../generated_headers/gen_map.h", directory: "/home/hamid.farzaneh/CostModelEvaluationRepo/NewEval/Hst/dpu")
!17 = !DICompositeType(tag: DW_TAG_array_type, baseType: !7, size: 32, elements: !18)
!18 = !{!19}
!19 = !DISubrange(count: 1)
!20 = !DIGlobalVariableExpression(var: !21, expr: !DIExpression())
!21 = distinct !DIGlobalVariable(name: "MAP_THREAD_DIMS", scope: !2, file: !16, line: 4, type: !17, isLocal: false, isDefinition: true)
!22 = !DIGlobalVariableExpression(var: !23, expr: !DIExpression())
!23 = distinct !DIGlobalVariable(name: "MAP_ITER_DIMS", scope: !2, file: !16, line: 5, type: !17, isLocal: false, isDefinition: true)
!24 = !DIGlobalVariableExpression(var: !25, expr: !DIExpression())
!25 = distinct !DIGlobalVariable(name: "MAP_BUFFER_SIZES", scope: !2, file: !16, line: 6, type: !17, isLocal: false, isDefinition: true)
!26 = !DIGlobalVariableExpression(var: !27, expr: !DIExpression())
!27 = distinct !DIGlobalVariable(name: "DPU_INPUT_ARGUMENTS", scope: !2, file: !3, line: 15, type: !28, isLocal: false, isDefinition: true, align: 64)
!28 = !DIDerivedType(tag: DW_TAG_typedef, name: "dpu_arguments_t", file: !29, line: 7, baseType: !30)
!29 = !DIFile(filename: "./../generated_headers/common_structs.h", directory: "/home/hamid.farzaneh/CostModelEvaluationRepo/NewEval/Hst/dpu")
!30 = distinct !DICompositeType(tag: DW_TAG_structure_type, file: !29, line: 2, size: 128, elements: !31)
!31 = !{!32, !33, !34, !35}
!32 = !DIDerivedType(tag: DW_TAG_member, name: "BUFFER_SIZES", scope: !30, file: !29, line: 3, baseType: !17, size: 32)
!33 = !DIDerivedType(tag: DW_TAG_member, name: "THREADS", scope: !30, file: !29, line: 4, baseType: !17, size: 32, offset: 32)
!34 = !DIDerivedType(tag: DW_TAG_member, name: "ITERS", scope: !30, file: !29, line: 5, baseType: !17, size: 32, offset: 64)
!35 = !DIDerivedType(tag: DW_TAG_member, name: "BINS", scope: !30, file: !29, line: 6, baseType: !7, size: 32, offset: 96)
!36 = !DIGlobalVariableExpression(var: !37, expr: !DIExpression())
!37 = distinct !DIGlobalVariable(name: "nb_cycle", scope: !2, file: !3, line: 16, type: !7, isLocal: false, isDefinition: true, align: 64)
!38 = !DIGlobalVariableExpression(var: !39, expr: !DIExpression())
!39 = distinct !DIGlobalVariable(name: "__atomic_bit_barrier_my_barrier", scope: !2, file: !3, line: 19, type: !40, isLocal: false, isDefinition: true)
!40 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint8_t", file: !8, line: 40, baseType: !41)
!41 = !DIBasicType(name: "unsigned char", size: 8, encoding: DW_ATE_unsigned_char)
!42 = !{i32 7, !"Dwarf Version", i32 4}
!43 = !{i32 2, !"Debug Info Version", i32 3}
!44 = !{i32 1, !"wchar_size", i32 1}
!45 = !{!"clang version 12.0.0 (https://github.com/upmem/llvm-project.git 846fdda8285dcc9b20ee5d2fec9e54dfea6a8928)"}
!46 = distinct !DISubprogram(name: "main", scope: !3, file: !3, line: 22, type: !47, scopeLine: 22, flags: DIFlagAllCallsDescribed, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !50)
!47 = !DISubroutineType(types: !48)
!48 = !{!49}
!49 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!50 = !{!51, !52, !53, !54, !55, !56, !57, !58, !59, !60, !61, !62, !63, !64, !65, !66, !68, !72}
!51 = !DILocalVariable(name: "tasklet_id", scope: !46, file: !3, line: 23, type: !9)
!52 = !DILocalVariable(name: "ITER_PER_THREAD", scope: !46, file: !3, line: 34, type: !7)
!53 = !DILocalVariable(name: "THREADS", scope: !46, file: !3, line: 35, type: !7)
!54 = !DILocalVariable(name: "BUFFER_COUNT", scope: !46, file: !3, line: 39, type: !7)
!55 = !DILocalVariable(name: "BINS", scope: !46, file: !3, line: 40, type: !7)
!56 = !DILocalVariable(name: "BUFFER_SIZE", scope: !46, file: !3, line: 41, type: !7)
!57 = !DILocalVariable(name: "cache_INPUT", scope: !46, file: !3, line: 43, type: !6)
!58 = !DILocalVariable(name: "cache_HST", scope: !46, file: !3, line: 44, type: !6)
!59 = !DILocalVariable(name: "arr_el_count", scope: !46, file: !3, line: 46, type: !7)
!60 = !DILocalVariable(name: "arr_el_size", scope: !46, file: !3, line: 47, type: !7)
!61 = !DILocalVariable(name: "mram_base_addr_INPUT", scope: !46, file: !3, line: 49, type: !7)
!62 = !DILocalVariable(name: "mram_base_addr_HST", scope: !46, file: !3, line: 50, type: !7)
!63 = !DILocalVariable(name: "mram_temp_addr_INPUT", scope: !46, file: !3, line: 52, type: !7)
!64 = !DILocalVariable(name: "mram_temp_addr_HST", scope: !46, file: !3, line: 54, type: !7)
!65 = !DILocalVariable(name: "local_iter_count", scope: !46, file: !3, line: 57, type: !7)
!66 = !DILocalVariable(name: "r", scope: !67, file: !3, line: 61, type: !49)
!67 = distinct !DILexicalBlock(scope: !46, file: !3, line: 61, column: 3)
!68 = !DILocalVariable(name: "c", scope: !69, file: !3, line: 64, type: !49)
!69 = distinct !DILexicalBlock(scope: !70, file: !3, line: 64, column: 5)
!70 = distinct !DILexicalBlock(scope: !71, file: !3, line: 61, column: 46)
!71 = distinct !DILexicalBlock(scope: !67, file: !3, line: 61, column: 3)
!72 = !DILocalVariable(name: "d", scope: !73, file: !3, line: 66, type: !7)
!73 = distinct !DILexicalBlock(scope: !74, file: !3, line: 64, column: 44)
!74 = distinct !DILexicalBlock(scope: !69, file: !3, line: 64, column: 5)
!75 = !DILocation(line: 35, column: 12, scope: !76, inlinedAt: !82)
!76 = distinct !DISubprogram(name: "me", scope: !77, file: !77, line: 33, type: !78, scopeLine: 34, flags: DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !4)
!77 = !DIFile(filename: "/opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/defs.h", directory: "")
!78 = !DISubroutineType(types: !79)
!79 = !{!80}
!80 = !DIDerivedType(tag: DW_TAG_typedef, name: "sysname_t", file: !81, line: 27, baseType: !9)
!81 = !DIFile(filename: "/opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/sysdef.h", directory: "")
!82 = distinct !DILocation(line: 23, column: 29, scope: !46)
!83 = !DILocation(line: 0, scope: !46)
!84 = !DILocation(line: 24, column: 18, scope: !85)
!85 = distinct !DILexicalBlock(scope: !46, file: !3, line: 24, column: 7)
!86 = !DILocation(line: 24, column: 7, scope: !46)
!87 = !DILocation(line: 25, column: 5, scope: !88)
!88 = distinct !DILexicalBlock(scope: !85, file: !3, line: 24, column: 24)
!89 = !DILocation(line: 31, column: 3, scope: !88)
!90 = !DILocation(line: 32, column: 3, scope: !46)
!91 = !DILocation(line: 34, column: 30, scope: !46)
!92 = !{!93, !93, i64 0}
!93 = !{!"int", !94, i64 0}
!94 = !{!"omnipotent char", !95, i64 0}
!95 = !{!"Simple C/C++ TBAA"}
!96 = !DILocation(line: 35, column: 22, scope: !46)
!97 = !DILocation(line: 36, column: 18, scope: !98)
!98 = distinct !DILexicalBlock(scope: !46, file: !3, line: 36, column: 7)
!99 = !DILocation(line: 36, column: 7, scope: !46)
!100 = !DILocation(line: 39, column: 27, scope: !46)
!101 = !DILocation(line: 40, column: 39, scope: !46)
!102 = !{!103, !93, i64 12}
!103 = !{!"", !94, i64 0, !94, i64 4, !94, i64 8, !93, i64 12}
!104 = !DILocation(line: 41, column: 39, scope: !46)
!105 = !DILocation(line: 43, column: 25, scope: !46)
!106 = !DILocation(line: 43, column: 20, scope: !46)
!107 = !DILocation(line: 44, column: 38, scope: !46)
!108 = !DILocation(line: 44, column: 23, scope: !46)
!109 = !DILocation(line: 44, column: 18, scope: !46)
!110 = !DILocation(line: 47, column: 39, scope: !46)
!111 = !DILocation(line: 53, column: 42, scope: !46)
!112 = !DILocation(line: 53, column: 61, scope: !46)
!113 = !DILocation(line: 53, column: 28, scope: !46)
!114 = !DILocation(line: 55, column: 48, scope: !46)
!115 = !DILocation(line: 55, column: 26, scope: !46)
!116 = !DILocation(line: 57, column: 47, scope: !46)
!117 = !DILocation(line: 0, scope: !67)
!118 = !DILocation(line: 61, column: 21, scope: !71)
!119 = !DILocation(line: 61, column: 3, scope: !67)
!120 = !DILocation(line: 62, column: 15, scope: !70)
!121 = !DILocalVariable(name: "from", arg: 1, scope: !122, file: !123, line: 33, type: !11)
!122 = distinct !DISubprogram(name: "mram_read", scope: !123, file: !123, line: 33, type: !124, scopeLine: 37, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !126)
!123 = !DIFile(filename: "/opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/mram.h", directory: "")
!124 = !DISubroutineType(types: !125)
!125 = !{null, !11, !10, !9}
!126 = !{!121, !127, !128}
!127 = !DILocalVariable(name: "to", arg: 2, scope: !122, file: !123, line: 33, type: !10)
!128 = !DILocalVariable(name: "nb_of_bytes", arg: 3, scope: !122, file: !123, line: 33, type: !9)
!129 = !DILocation(line: 0, scope: !122, inlinedAt: !130)
!130 = distinct !DILocation(line: 62, column: 5, scope: !70)
!131 = !DILocation(line: 39, column: 24, scope: !122, inlinedAt: !130)
!132 = !DILocation(line: 0, scope: !69)
!133 = !DILocation(line: 64, column: 5, scope: !69)
!134 = !DILocation(line: 66, column: 14, scope: !73)
!135 = !DILocation(line: 66, column: 29, scope: !73)
!136 = !DILocation(line: 66, column: 37, scope: !73)
!137 = !DILocation(line: 0, scope: !73)
!138 = !DILocation(line: 67, column: 7, scope: !73)
!139 = !DILocation(line: 67, column: 20, scope: !73)
!140 = !DILocation(line: 64, column: 40, scope: !74)
!141 = !DILocation(line: 64, column: 23, scope: !74)
!142 = distinct !{!142, !133, !143, !144}
!143 = !DILocation(line: 68, column: 5, scope: !69)
!144 = !{!"llvm.loop.mustprogress"}
!145 = !DILocation(line: 69, column: 26, scope: !70)
!146 = !DILocation(line: 61, column: 42, scope: !71)
!147 = distinct !{!147, !119, !148, !144}
!148 = !DILocation(line: 70, column: 3, scope: !67)
!149 = !DILocation(line: 71, column: 25, scope: !46)
!150 = !DILocalVariable(name: "from", arg: 1, scope: !151, file: !123, line: 55, type: !11)
!151 = distinct !DISubprogram(name: "mram_write", scope: !123, file: !123, line: 55, type: !124, scopeLine: 59, flags: DIFlagPrototyped | DIFlagAllCallsDescribed, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition | DISPFlagOptimized, unit: !2, retainedNodes: !152)
!152 = !{!150, !153, !154}
!153 = !DILocalVariable(name: "to", arg: 2, scope: !151, file: !123, line: 55, type: !10)
!154 = !DILocalVariable(name: "nb_of_bytes", arg: 3, scope: !151, file: !123, line: 55, type: !9)
!155 = !DILocation(line: 0, scope: !151, inlinedAt: !156)
!156 = distinct !DILocation(line: 71, column: 3, scope: !46)
!157 = !DILocation(line: 60, column: 39, scope: !151, inlinedAt: !156)
!158 = !DILocation(line: 60, column: 5, scope: !151, inlinedAt: !156)
!159 = !DILocation(line: 61, column: 24, scope: !151, inlinedAt: !156)
!160 = !DILocation(line: 75, column: 1, scope: !46)
!161 = !DISubprogram(name: "mem_reset", scope: !162, file: !162, line: 50, type: !163, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !4)
!162 = !DIFile(filename: "/opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/alloc.h", directory: "")
!163 = !DISubroutineType(types: !164)
!164 = !{!10}
!165 = !DISubprogram(name: "barrier_wait", scope: !166, file: !166, line: 72, type: !167, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !4)
!166 = !DIFile(filename: "/opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/barrier.h", directory: "")
!167 = !DISubroutineType(types: !168)
!168 = !{null, !169}
!169 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !170, size: 32)
!170 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "barrier_t", file: !166, line: 33, size: 32, elements: !171)
!171 = !{!172, !173, !174, !175}
!172 = !DIDerivedType(tag: DW_TAG_member, name: "wait_queue", scope: !170, file: !166, line: 34, baseType: !40, size: 8)
!173 = !DIDerivedType(tag: DW_TAG_member, name: "count", scope: !170, file: !166, line: 35, baseType: !40, size: 8, offset: 8)
!174 = !DIDerivedType(tag: DW_TAG_member, name: "initial_count", scope: !170, file: !166, line: 36, baseType: !40, size: 8, offset: 16)
!175 = !DIDerivedType(tag: DW_TAG_member, name: "lock", scope: !170, file: !166, line: 37, baseType: !40, size: 8, offset: 24)
!176 = !DISubprogram(name: "mem_alloc", scope: !162, file: !162, line: 38, type: !177, flags: DIFlagPrototyped, spFlags: DISPFlagOptimized, retainedNodes: !4)
!177 = !DISubroutineType(types: !178)
!178 = !{!10, !9}
