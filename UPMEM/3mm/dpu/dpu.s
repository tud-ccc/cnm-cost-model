	.text
	.file	"dpu.c"
                                        // Start of file scope inline assembly
	.section	.data.my_barrier,"aw",@progbits
	.type	my_barrier,@object
	.globl	my_barrier
	.p2align	2
my_barrier:
	.byte	255
	.byte	1
	.byte	1
	.byte	__atomic_bit_barrier_my_barrier
	.size	my_barrier, 4
	.text

                                        // End of file scope inline assembly
	.file	1 "/opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/stdlib" "stdint.h"
	.file	2 "/home/hamid.farzaneh/CostModelEvaluationRepo/NewEval/Matmul/dpu" "./../generated_headers/gen_map.h"
	.file	3 "/home/hamid.farzaneh/CostModelEvaluationRepo/NewEval/Matmul/dpu" "./../generated_headers/common_structs.h"
	.file	4 "/home/hamid.farzaneh/CostModelEvaluationRepo/NewEval/Matmul/dpu" "dpu.c"
	.section	.text.main,"ax",@progbits
	.globl	main                            // -- Begin function main
	.type	main,@function
main:                                   // @main
.Lfunc_begin0:
	.loc	4 22 0                          // dpu.c:22:0
	.cfi_sections .debug_frame
	.cfi_startproc
// %bb.0:
	.cfi_def_cfa_offset -88
	.cfi_offset 23, -8
	.cfi_offset 22, -4
	sd r22, 80, d22
	add r22, r22, 88
	.cfi_offset 15, -64
	.cfi_offset 14, -60
.Ltmp0:
	sd r22, -64, d14
	.cfi_offset 17, -72
	.cfi_offset 16, -68
	sd r22, -72, d16
	.cfi_offset 19, -80
	.cfi_offset 18, -76
	sd r22, -80, d18
	.cfi_offset 21, -88
	.cfi_offset 20, -84
	sd r22, -88, d20
.Ltmp1:
	//DEBUG_VALUE: main:tasklet_id <- $r20
	.file	5 "/opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib" "defs.h"
	.loc	5 35 12 prologue_end            // /opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/defs.h:35:12
	move r20, id, nz, .LBB0_2
.Ltmp2:
// %bb.1:
	.loc	4 25 5                          // dpu.c:25:5
	call r23, mem_reset
.Ltmp3:
.LBB0_2:
	.loc	4 32 3                          // dpu.c:32:3
	move r0, my_barrier
	call r23, barrier_wait
.Ltmp4:
	.loc	4 34 31                         // dpu.c:34:31
	move r0, DPU_INPUT_ARGUMENTS
.Ltmp5:
	//DEBUG_VALUE: main:ITER_PER_THREAD2 <- undef
	//DEBUG_VALUE: main:ITER_PER_THREAD1 <- undef
	.loc	4 37 22                         // dpu.c:37:22
	lw r21, r0, 4
.Ltmp6:
	//DEBUG_VALUE: main:THREADS <- $r21
	.loc	4 38 7                          // dpu.c:38:7
	jgeu r20, r21, .LBB0_16
.Ltmp7:
// %bb.3:
	//DEBUG_VALUE: main:THREADS <- $r21
	.loc	4 41 27                         // dpu.c:41:27
	lw r14, zero, DPU_INPUT_ARGUMENTS
.Ltmp8:
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r14
	.loc	4 0 0 is_stmt 0                 // dpu.c:0:0
	lw r15, r0, 8
.Ltmp9:
	//DEBUG_VALUE: main:ITER_PER_THREAD1 <- $r15
	lw r16, r0, 12
.Ltmp10:
	//DEBUG_VALUE: main:ITER_PER_THREAD2 <- $r16
	.loc	4 42 39 is_stmt 1               // dpu.c:42:39
	lsl r17, r14, 2
.Ltmp11:
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r17
	.loc	4 43 21                         // dpu.c:43:21
	move r0, r17
	call r23, mem_alloc
.Ltmp12:
	move r18, r0
.Ltmp13:
	//DEBUG_VALUE: main:cache_A <- $r18
	.loc	4 44 21                         // dpu.c:44:21
	move r0, r17
	call r23, mem_alloc
.Ltmp14:
	move r19, r0
.Ltmp15:
	//DEBUG_VALUE: main:cache_B <- $r19
	.loc	4 45 21                         // dpu.c:45:21
	move r0, r17
	call r23, mem_alloc
.Ltmp16:
	sw r22, -56, r0
.Ltmp17:
	//DEBUG_VALUE: main:cache_C <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	.loc	4 47 46                         // dpu.c:47:46
	move r0, r14
	move r1, r21
	call r23, __mulsi3
.Ltmp18:
	move r21, r0
.Ltmp19:
	//DEBUG_VALUE: main:Amat_el_count <- undef
	.loc	4 48 41                         // dpu.c:48:41
	move r1, r15
	call r23, __mulsi3
.Ltmp20:
	sw r22, -44, r0
.Ltmp21:
	//DEBUG_VALUE: main:mram_base_addr_C <- undef
	//DEBUG_VALUE: main:mram_base_addr_B <- undef
	//DEBUG_VALUE: main:res_el_size <- [DW_OP_constu 4, DW_OP_mul, DW_OP_stack_value] undef
	//DEBUG_VALUE: main:res_el_count <- undef
	//DEBUG_VALUE: main:Bmat_el_size <- undef
	//DEBUG_VALUE: main:Bmat_el_count <- undef
	//DEBUG_VALUE: main:Amat_el_size <- undef
	.loc	4 63 19                         // dpu.c:63:19
	move r0, r15
	move r1, r20
	call r23, __mulsi3
.Ltmp22:
	sw r22, -48, r0
.Ltmp23:
	//DEBUG_VALUE: main:mram_temp_addr_A <- undef
	.loc	4 67 19                         // dpu.c:67:19
	move r0, r14
	move r1, r16
	call r23, __mulsi3
.Ltmp24:
	.loc	4 67 54 is_stmt 0               // dpu.c:67:54
	move r1, r20
	call r23, __mulsi3
.Ltmp25:
	move r20, r0
.Ltmp26:
	//DEBUG_VALUE: main:local_iter_count1 <- undef
	//DEBUG_VALUE: main:mram_temp_addr_C <- undef
	//DEBUG_VALUE: main:mram_temp_addr_B <- undef
	//DEBUG_VALUE: main:mram_base_addr_A <- undef
	sw r22, -16, r16
.Ltmp27:
	//DEBUG_VALUE: main:ITER_PER_THREAD2 <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	.loc	4 75 49 is_stmt 1               // dpu.c:75:49
	move r0, r16
	move r1, r14
	call r23, __udiv32
.Ltmp28:
	sw r22, -28, r0
.Ltmp29:
	//DEBUG_VALUE: main:local_iter_count2 <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_temp_addr_C <- undef
	//DEBUG_VALUE: main:mram_temp_addr_A <- undef
	sw r22, -12, r15
.Ltmp30:
	//DEBUG_VALUE: a1 <- 0
	//DEBUG_VALUE: main:ITER_PER_THREAD1 <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	.loc	4 76 3                          // dpu.c:76:3
	jeq r15, 0, .LBB0_16
.Ltmp31:
// %bb.4:
	//DEBUG_VALUE: main:ITER_PER_THREAD1 <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:local_iter_count2 <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:ITER_PER_THREAD2 <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:cache_C <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:cache_B <- $r19
	//DEBUG_VALUE: main:cache_A <- $r18
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r17
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r14
	.loc	4 0 3 is_stmt 0                 // dpu.c:0:3
	lw r2, r22, -44
	lsl r0, r2, 2
.Ltmp32:
	//DEBUG_VALUE: main:Amat_el_size <- $r0
	move r15, __sys_used_mram_end
.Ltmp33:
	//DEBUG_VALUE: main:mram_base_addr_A <- $r15
	lsl_add r1, r15, r20, 2
	lsl_add r1, r1, r2, 2
.Ltmp34:
	//DEBUG_VALUE: main:mram_temp_addr_B <- $r1
	sw r22, -40, r1
.Ltmp35:
	//DEBUG_VALUE: main:mram_temp_addr_B <- [DW_OP_constu 40, DW_OP_minus] [$r22+0]
	.loc	4 59 56 is_stmt 1               // dpu.c:59:56
	add r16, r0, r15
.Ltmp36:
	//DEBUG_VALUE: main:Bmat_el_size <- $r21
	.loc	4 0 56 is_stmt 0                // dpu.c:0:56
	lw r20, r22, -48
	add r1, r21, r20
	lw r0, r22, -16
.Ltmp37:
	call r23, __mulsi3
.Ltmp38:
	.loc	4 70 24 is_stmt 1               // dpu.c:70:24
	lsl_add r16, r16, r0, 2
.Ltmp39:
	//DEBUG_VALUE: main:mram_temp_addr_C <- $r16
	.loc	4 63 54                         // dpu.c:63:54
	move r0, r20
	move r1, r14
	call r23, __mulsi3
	lsl_add r1, r15, r0, 2
	move r2, 0
	sw r22, -52, r17
	jump .LBB0_5
.Ltmp42:
.LBB0_15:                               //   in Loop: Header=BB0_5 Depth=1
	lw r2, r22, -20
	add r2, r2, 1
	lw r1, r22, -24
	add r1, r1, r17
	lw r0, r22, -12
	jeq r2, r0, .LBB0_16
.LBB0_5:                                // =>This Loop Header: Depth=1
	move r0, -1
	lsr_add r0, r0, r17, 3
	lsl_add r0, r18, r0, 24
	ldma r0, r1, 0
	lw r0, r22, -16
	sw r22, -24, r1
	sw r22, -20, r2
	jgtu r14, r0, .LBB0_15
	move r1, 0, true, .LBB0_7
.LBB0_14:                               //   in Loop: Header=BB0_7 Depth=2
	move r0, -1
	lsr_add r0, r0, r17, 3
	lw r1, r22, -56
	lsl_add r0, r1, r0, 24
	lw r16, r22, -36
	sdma r0, r16, 0
	lw r1, r22, -32
	add r1, r1, 1
	add r16, r16, r17
	lw r0, r22, -28
	jgeu r1, r0, .LBB0_15
.LBB0_7:                                //   Parent Loop BB0_5 Depth=1
	sw r22, -32, r1
	sw r22, -36, r16
	jeq r14, 0, .LBB0_14
	move r0, 0
	sw r22, -44, r0
.LBB0_9:                                //   Parent Loop BB0_5 Depth=1
	sw r22, -48, r0
	lw r21, r22, -44
	lw r16, r22, -40
.LBB0_10:                               //   Parent Loop BB0_5 Depth=1
	move r0, -1
	lsr_add r0, r0, r17, 3
	lsl_add r0, r19, r0, 24
	ldma r0, r16, 0
	lw r0, r22, -56
	lsl_add r15, r0, r21, 2
	move r17, 0
	sw r15, 0, 0
	move r20, r17
.LBB0_11:                               //   Parent Loop BB0_5 Depth=1
	lsl_add r0, r18, r20, 2
	lw r1, r0, 0
	lsl_add r0, r19, r20, 2
	lw r0, r0, 0
	call r23, __mulsi3
	add r17, r17, r0
	add r20, r20, 1
	sw r15, 0, r17
	jneq r14, r20, .LBB0_11
	add r21, r21, 1
	lw r17, r22, -52
	add r16, r16, r17
	jneq r21, r14, .LBB0_10
	lw r0, r22, -48
	add r0, r0, 1
	jneq r0, r14, .LBB0_9
	jump .LBB0_14
	move r0, 0
	ld d20, r22, -88
	ld d18, r22, -80
	ld d16, r22, -72
	ld d14, r22, -64
	ld d22, r22, -8
	jump r23
.Ltmp81:
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
	.file	7 "/opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib" "sysdef.h"
	.section	.stack_sizes,"o",@progbits,.text.main
	.long	.Lfunc_begin0
	.byte	88
	.section	.text.main,"ax",@progbits
                                        // -- End function
	.type	MAP_RANK_DIMS,@object           // @MAP_RANK_DIMS
	.section	.data.MAP_RANK_DIMS,"aw",@progbits
	.globl	MAP_RANK_DIMS
	.p2align	2
MAP_RANK_DIMS:
	.long	1                               // 0x1
	.size	MAP_RANK_DIMS, 4

	.type	MAP_DPU_DIMS,@object            // @MAP_DPU_DIMS
	.section	.data.MAP_DPU_DIMS,"aw",@progbits
	.globl	MAP_DPU_DIMS
	.p2align	2
MAP_DPU_DIMS:
	.long	1                               // 0x1
	.size	MAP_DPU_DIMS, 4

	.type	MAP_THREAD_DIMS,@object         // @MAP_THREAD_DIMS
	.section	.data.MAP_THREAD_DIMS,"aw",@progbits
	.globl	MAP_THREAD_DIMS
	.p2align	2
MAP_THREAD_DIMS:
	.long	1                               // 0x1
	.size	MAP_THREAD_DIMS, 4

	.type	MAP_ITER_DIMS,@object           // @MAP_ITER_DIMS
	.section	.data.MAP_ITER_DIMS,"aw",@progbits
	.globl	MAP_ITER_DIMS
	.p2align	2
MAP_ITER_DIMS:
	.long	128                             // 0x80
	.long	128                             // 0x80
	.size	MAP_ITER_DIMS, 8

	.type	MAP_BUFFER_SIZES,@object        // @MAP_BUFFER_SIZES
	.section	.data.MAP_BUFFER_SIZES,"aw",@progbits
	.globl	MAP_BUFFER_SIZES
	.p2align	2
MAP_BUFFER_SIZES:
	.long	32                              // 0x20
	.size	MAP_BUFFER_SIZES, 4

	.type	DPU_INPUT_ARGUMENTS,@object     // @DPU_INPUT_ARGUMENTS
	.section	.dpu_host,"aw",@progbits
	.globl	DPU_INPUT_ARGUMENTS
	.p2align	3
DPU_INPUT_ARGUMENTS:
	.zero	16
	.size	DPU_INPUT_ARGUMENTS, 16

	.type	nb_cycle,@object                // @nb_cycle
	.globl	nb_cycle
	.p2align	3
nb_cycle:
	.long	0                               // 0x0
	.size	nb_cycle, 4

	.type	__atomic_bit_barrier_my_barrier,@object // @__atomic_bit_barrier_my_barrier
	.section	.atomic,"aw",@progbits
	.globl	__atomic_bit_barrier_my_barrier
__atomic_bit_barrier_my_barrier:
	.byte	0                               // 0x0
	.size	__atomic_bit_barrier_my_barrier, 1

	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
	.long	.Ltmp1-.Lfunc_begin0
	.long	.Ltmp2-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	100                             // DW_OP_reg20
	.long	0
	.long	0
.Ldebug_loc1:
	.long	.Ltmp10-.Lfunc_begin0
	.long	.Ltmp27-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	96                              // DW_OP_reg16
	.long	.Ltmp27-.Lfunc_begin0
	.long	.Ltmp80-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	112                             // -16
	.long	0
	.long	0
.Ldebug_loc2:
	.long	.Ltmp9-.Lfunc_begin0
	.long	.Ltmp30-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	95                              // DW_OP_reg15
	.long	.Ltmp30-.Lfunc_begin0
	.long	.Ltmp80-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	116                             // -12
	.long	0
	.long	0
.Ldebug_loc3:
	.long	.Ltmp6-.Lfunc_begin0
	.long	.Ltmp19-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	101                             // DW_OP_reg21
	.long	0
	.long	0
.Ldebug_loc4:
	.long	.Ltmp8-.Lfunc_begin0
	.long	.Ltmp80-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	94                              // DW_OP_reg14
	.long	0
	.long	0
.Ldebug_loc5:
	.long	.Ltmp11-.Lfunc_begin0
	.long	.Ltmp42-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	97                              // DW_OP_reg17
	.long	0
	.long	0
.Ldebug_loc6:
	.long	.Ltmp13-.Lfunc_begin0
	.long	.Ltmp80-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	98                              // DW_OP_reg18
	.long	0
	.long	0
.Ldebug_loc7:
	.long	.Ltmp15-.Lfunc_begin0
	.long	.Ltmp80-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	99                              // DW_OP_reg19
	.long	0
	.long	0
.Ldebug_loc8:
	.long	.Ltmp17-.Lfunc_begin0
	.long	.Ltmp80-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	72                              // -56
	.long	0
	.long	0
.Ldebug_loc9:
	.long	.Ltmp36-.Lfunc_begin0
	.long	.Ltmp42-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	101                             // DW_OP_reg21
	.long	0
	.long	0
.Ldebug_loc10:
	.long	.Ltmp32-.Lfunc_begin0
	.long	.Ltmp37-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	80                              // DW_OP_reg0
	.long	0
	.long	0
.Ldebug_loc11:
	.long	.Ltmp41-.Lfunc_begin0
	.long	.Ltmp42-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	.Ltmp44-.Lfunc_begin0
	.long	.Ltmp45-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	104                             // -24
	.long	.Ltmp45-.Lfunc_begin0
	.long	.Ltmp49-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	0
	.long	0
.Ldebug_loc12:
	.long	.Ltmp39-.Lfunc_begin0
	.long	.Ltmp52-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	96                              // DW_OP_reg16
	.long	.Ltmp52-.Lfunc_begin0
	.long	.Ltmp59-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	92                              // -36
	.long	.Ltmp59-.Lfunc_begin0
	.long	.Ltmp62-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	96                              // DW_OP_reg16
	.long	.Ltmp62-.Lfunc_begin0
	.long	.Ltmp80-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	92                              // -36
	.long	0
	.long	0
.Ldebug_loc13:
	.long	.Ltmp34-.Lfunc_begin0
	.long	.Ltmp35-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	.Ltmp35-.Lfunc_begin0
	.long	.Ltmp80-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	88                              // -40
	.long	0
	.long	0
.Ldebug_loc14:
	.long	.Ltmp33-.Lfunc_begin0
	.long	.Ltmp42-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	95                              // DW_OP_reg15
	.long	0
	.long	0
.Ldebug_loc15:
	.long	.Ltmp29-.Lfunc_begin0
	.long	.Ltmp80-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	100                             // -28
	.long	0
	.long	0
.Ldebug_loc16:
	.long	.Ltmp30-.Lfunc_begin0
	.long	.Ltmp31-.Lfunc_begin0
	.short	3                               // Loc expr size
	.byte	17                              // DW_OP_consts
	.byte	0                               // 0
	.byte	159                             // DW_OP_stack_value
	.long	.Ltmp44-.Lfunc_begin0
	.long	.Ltmp47-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	82                              // DW_OP_reg2
	.long	.Ltmp48-.Lfunc_begin0
	.long	.Ltmp80-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	82                              // DW_OP_reg2
	.long	0
	.long	0
.Ldebug_loc17:
	.long	.Ltmp50-.Lfunc_begin0
	.long	.Ltmp52-.Lfunc_begin0
	.short	3                               // Loc expr size
	.byte	17                              // DW_OP_consts
	.byte	0                               // 0
	.byte	159                             // DW_OP_stack_value
	.long	.Ltmp52-.Lfunc_begin0
	.long	.Ltmp58-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	96                              // -32
	.long	.Ltmp58-.Lfunc_begin0
	.long	.Ltmp61-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	.Ltmp61-.Lfunc_begin0
	.long	.Ltmp80-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	96                              // -32
	.long	0
	.long	0
.Ldebug_loc18:
	.long	.Ltmp53-.Lfunc_begin0
	.long	.Ltmp54-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	72                              // -56
	.long	.Ltmp54-.Lfunc_begin0
	.long	.Ltmp55-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	.Ltmp55-.Lfunc_begin0
	.long	.Ltmp60-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	72                              // -56
	.long	0
	.long	0
.Ldebug_loc19:
	.long	.Ltmp53-.Lfunc_begin0
	.long	.Ltmp56-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	92                              // -36
	.long	.Ltmp56-.Lfunc_begin0
	.long	.Ltmp59-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	96                              // DW_OP_reg16
	.long	0
	.long	0
.Ldebug_loc20:
	.long	.Ltmp60-.Lfunc_begin0
	.long	.Ltmp64-.Lfunc_begin0
	.short	3                               // Loc expr size
	.byte	17                              // DW_OP_consts
	.byte	0                               // 0
	.byte	159                             // DW_OP_stack_value
	.long	.Ltmp64-.Lfunc_begin0
	.long	.Ltmp65-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	80                              // DW_OP_reg0
	.long	.Ltmp65-.Lfunc_begin0
	.long	.Ltmp79-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	80                              // -48
	.long	.Ltmp79-.Lfunc_begin0
	.long	.Ltmp80-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	80                              // DW_OP_reg0
	.long	0
	.long	0
	.section	.debug_abbrev,"",@progbits
	.byte	1                               // Abbreviation Code
	.byte	17                              // DW_TAG_compile_unit
	.byte	1                               // DW_CHILDREN_yes
	.byte	37                              // DW_AT_producer
	.byte	14                              // DW_FORM_strp
	.byte	19                              // DW_AT_language
	.byte	5                               // DW_FORM_data2
	.byte	3                               // DW_AT_name
	.byte	14                              // DW_FORM_strp
	.byte	16                              // DW_AT_stmt_list
	.byte	23                              // DW_FORM_sec_offset
	.byte	27                              // DW_AT_comp_dir
	.byte	14                              // DW_FORM_strp
	.byte	17                              // DW_AT_low_pc
	.byte	1                               // DW_FORM_addr
	.byte	18                              // DW_AT_high_pc
	.byte	6                               // DW_FORM_data4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	2                               // Abbreviation Code
	.byte	52                              // DW_TAG_variable
	.byte	0                               // DW_CHILDREN_no
	.byte	3                               // DW_AT_name
	.byte	14                              // DW_FORM_strp
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	63                              // DW_AT_external
	.byte	25                              // DW_FORM_flag_present
	.byte	58                              // DW_AT_decl_file
	.byte	11                              // DW_FORM_data1
	.byte	59                              // DW_AT_decl_line
	.byte	11                              // DW_FORM_data1
	.byte	2                               // DW_AT_location
	.byte	24                              // DW_FORM_exprloc
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	3                               // Abbreviation Code
	.byte	1                               // DW_TAG_array_type
	.byte	1                               // DW_CHILDREN_yes
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	4                               // Abbreviation Code
	.byte	33                              // DW_TAG_subrange_type
	.byte	0                               // DW_CHILDREN_no
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	55                              // DW_AT_count
	.byte	11                              // DW_FORM_data1
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	5                               // Abbreviation Code
	.byte	22                              // DW_TAG_typedef
	.byte	0                               // DW_CHILDREN_no
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	3                               // DW_AT_name
	.byte	14                              // DW_FORM_strp
	.byte	58                              // DW_AT_decl_file
	.byte	11                              // DW_FORM_data1
	.byte	59                              // DW_AT_decl_line
	.byte	11                              // DW_FORM_data1
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	6                               // Abbreviation Code
	.byte	36                              // DW_TAG_base_type
	.byte	0                               // DW_CHILDREN_no
	.byte	3                               // DW_AT_name
	.byte	14                              // DW_FORM_strp
	.byte	62                              // DW_AT_encoding
	.byte	11                              // DW_FORM_data1
	.byte	11                              // DW_AT_byte_size
	.byte	11                              // DW_FORM_data1
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	7                               // Abbreviation Code
	.byte	36                              // DW_TAG_base_type
	.byte	0                               // DW_CHILDREN_no
	.byte	3                               // DW_AT_name
	.byte	14                              // DW_FORM_strp
	.byte	11                              // DW_AT_byte_size
	.byte	11                              // DW_FORM_data1
	.byte	62                              // DW_AT_encoding
	.byte	11                              // DW_FORM_data1
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	8                               // Abbreviation Code
	.byte	52                              // DW_TAG_variable
	.byte	0                               // DW_CHILDREN_no
	.byte	3                               // DW_AT_name
	.byte	14                              // DW_FORM_strp
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	63                              // DW_AT_external
	.byte	25                              // DW_FORM_flag_present
	.byte	58                              // DW_AT_decl_file
	.byte	11                              // DW_FORM_data1
	.byte	59                              // DW_AT_decl_line
	.byte	11                              // DW_FORM_data1
	.ascii	"\210\001"                      // DW_AT_alignment
	.byte	15                              // DW_FORM_udata
	.byte	2                               // DW_AT_location
	.byte	24                              // DW_FORM_exprloc
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	9                               // Abbreviation Code
	.byte	19                              // DW_TAG_structure_type
	.byte	1                               // DW_CHILDREN_yes
	.byte	11                              // DW_AT_byte_size
	.byte	11                              // DW_FORM_data1
	.byte	58                              // DW_AT_decl_file
	.byte	11                              // DW_FORM_data1
	.byte	59                              // DW_AT_decl_line
	.byte	11                              // DW_FORM_data1
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	10                              // Abbreviation Code
	.byte	13                              // DW_TAG_member
	.byte	0                               // DW_CHILDREN_no
	.byte	3                               // DW_AT_name
	.byte	14                              // DW_FORM_strp
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	58                              // DW_AT_decl_file
	.byte	11                              // DW_FORM_data1
	.byte	59                              // DW_AT_decl_line
	.byte	11                              // DW_FORM_data1
	.byte	56                              // DW_AT_data_member_location
	.byte	11                              // DW_FORM_data1
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	11                              // Abbreviation Code
	.byte	15                              // DW_TAG_pointer_type
	.byte	0                               // DW_CHILDREN_no
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	12                              // Abbreviation Code
	.byte	15                              // DW_TAG_pointer_type
	.byte	0                               // DW_CHILDREN_no
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	13                              // Abbreviation Code
	.byte	38                              // DW_TAG_const_type
	.byte	0                               // DW_CHILDREN_no
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	14                              // Abbreviation Code
	.byte	46                              // DW_TAG_subprogram
	.byte	0                               // DW_CHILDREN_no
	.byte	3                               // DW_AT_name
	.byte	14                              // DW_FORM_strp
	.byte	58                              // DW_AT_decl_file
	.byte	11                              // DW_FORM_data1
	.byte	59                              // DW_AT_decl_line
	.byte	11                              // DW_FORM_data1
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	32                              // DW_AT_inline
	.byte	11                              // DW_FORM_data1
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	15                              // Abbreviation Code
	.byte	46                              // DW_TAG_subprogram
	.byte	1                               // DW_CHILDREN_yes
	.byte	3                               // DW_AT_name
	.byte	14                              // DW_FORM_strp
	.byte	58                              // DW_AT_decl_file
	.byte	11                              // DW_FORM_data1
	.byte	59                              // DW_AT_decl_line
	.byte	11                              // DW_FORM_data1
	.byte	39                              // DW_AT_prototyped
	.byte	25                              // DW_FORM_flag_present
	.byte	32                              // DW_AT_inline
	.byte	11                              // DW_FORM_data1
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	16                              // Abbreviation Code
	.byte	5                               // DW_TAG_formal_parameter
	.byte	0                               // DW_CHILDREN_no
	.byte	3                               // DW_AT_name
	.byte	14                              // DW_FORM_strp
	.byte	58                              // DW_AT_decl_file
	.byte	11                              // DW_FORM_data1
	.byte	59                              // DW_AT_decl_line
	.byte	11                              // DW_FORM_data1
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	17                              // Abbreviation Code
	.byte	46                              // DW_TAG_subprogram
	.byte	1                               // DW_CHILDREN_yes
	.byte	17                              // DW_AT_low_pc
	.byte	1                               // DW_FORM_addr
	.byte	18                              // DW_AT_high_pc
	.byte	6                               // DW_FORM_data4
	.byte	64                              // DW_AT_frame_base
	.byte	24                              // DW_FORM_exprloc
	.ascii	"\227B"                         // DW_AT_GNU_all_call_sites
	.byte	25                              // DW_FORM_flag_present
	.byte	3                               // DW_AT_name
	.byte	14                              // DW_FORM_strp
	.byte	58                              // DW_AT_decl_file
	.byte	11                              // DW_FORM_data1
	.byte	59                              // DW_AT_decl_line
	.byte	11                              // DW_FORM_data1
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	63                              // DW_AT_external
	.byte	25                              // DW_FORM_flag_present
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	18                              // Abbreviation Code
	.byte	52                              // DW_TAG_variable
	.byte	0                               // DW_CHILDREN_no
	.byte	2                               // DW_AT_location
	.byte	23                              // DW_FORM_sec_offset
	.byte	3                               // DW_AT_name
	.byte	14                              // DW_FORM_strp
	.byte	58                              // DW_AT_decl_file
	.byte	11                              // DW_FORM_data1
	.byte	59                              // DW_AT_decl_line
	.byte	11                              // DW_FORM_data1
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	19                              // Abbreviation Code
	.byte	52                              // DW_TAG_variable
	.byte	0                               // DW_CHILDREN_no
	.byte	3                               // DW_AT_name
	.byte	14                              // DW_FORM_strp
	.byte	58                              // DW_AT_decl_file
	.byte	11                              // DW_FORM_data1
	.byte	59                              // DW_AT_decl_line
	.byte	11                              // DW_FORM_data1
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	20                              // Abbreviation Code
	.byte	29                              // DW_TAG_inlined_subroutine
	.byte	0                               // DW_CHILDREN_no
	.byte	49                              // DW_AT_abstract_origin
	.byte	19                              // DW_FORM_ref4
	.byte	17                              // DW_AT_low_pc
	.byte	1                               // DW_FORM_addr
	.byte	18                              // DW_AT_high_pc
	.byte	6                               // DW_FORM_data4
	.byte	88                              // DW_AT_call_file
	.byte	11                              // DW_FORM_data1
	.byte	89                              // DW_AT_call_line
	.byte	11                              // DW_FORM_data1
	.byte	87                              // DW_AT_call_column
	.byte	11                              // DW_FORM_data1
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	21                              // Abbreviation Code
	.byte	11                              // DW_TAG_lexical_block
	.byte	1                               // DW_CHILDREN_yes
	.byte	85                              // DW_AT_ranges
	.byte	23                              // DW_FORM_sec_offset
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	22                              // Abbreviation Code
	.byte	29                              // DW_TAG_inlined_subroutine
	.byte	1                               // DW_CHILDREN_yes
	.byte	49                              // DW_AT_abstract_origin
	.byte	19                              // DW_FORM_ref4
	.byte	17                              // DW_AT_low_pc
	.byte	1                               // DW_FORM_addr
	.byte	18                              // DW_AT_high_pc
	.byte	6                               // DW_FORM_data4
	.byte	88                              // DW_AT_call_file
	.byte	11                              // DW_FORM_data1
	.byte	89                              // DW_AT_call_line
	.byte	11                              // DW_FORM_data1
	.byte	87                              // DW_AT_call_column
	.byte	11                              // DW_FORM_data1
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	23                              // Abbreviation Code
	.byte	5                               // DW_TAG_formal_parameter
	.byte	0                               // DW_CHILDREN_no
	.byte	2                               // DW_AT_location
	.byte	24                              // DW_FORM_exprloc
	.byte	49                              // DW_AT_abstract_origin
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	24                              // Abbreviation Code
	.byte	11                              // DW_TAG_lexical_block
	.byte	1                               // DW_CHILDREN_yes
	.byte	17                              // DW_AT_low_pc
	.byte	1                               // DW_FORM_addr
	.byte	18                              // DW_AT_high_pc
	.byte	6                               // DW_FORM_data4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	25                              // Abbreviation Code
	.byte	5                               // DW_TAG_formal_parameter
	.byte	0                               // DW_CHILDREN_no
	.byte	2                               // DW_AT_location
	.byte	23                              // DW_FORM_sec_offset
	.byte	49                              // DW_AT_abstract_origin
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	26                              // Abbreviation Code
	.byte	52                              // DW_TAG_variable
	.byte	0                               // DW_CHILDREN_no
	.byte	2                               // DW_AT_location
	.byte	24                              // DW_FORM_exprloc
	.byte	3                               // DW_AT_name
	.byte	14                              // DW_FORM_strp
	.byte	58                              // DW_AT_decl_file
	.byte	11                              // DW_FORM_data1
	.byte	59                              // DW_AT_decl_line
	.byte	11                              // DW_FORM_data1
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	27                              // Abbreviation Code
	.ascii	"\211\202\001"                  // DW_TAG_GNU_call_site
	.byte	0                               // DW_CHILDREN_no
	.ascii	"\223B"                         // DW_AT_GNU_call_site_target
	.byte	24                              // DW_FORM_exprloc
	.byte	17                              // DW_AT_low_pc
	.byte	1                               // DW_FORM_addr
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	0                               // EOM(3)
	.section	.debug_info,"",@progbits
.Lcu_begin0:
	.long	.Ldebug_info_end0-.Ldebug_info_start0 // Length of Unit
.Ldebug_info_start0:
	.short	4                               // DWARF version number
	.long	.debug_abbrev                   // Offset Into Abbrev. Section
	.byte	4                               // Address Size (in bytes)
	.byte	1                               // Abbrev [1] 0xb:0x463 DW_TAG_compile_unit
	.long	.Linfo_string0                  // DW_AT_producer
	.short	12                              // DW_AT_language
	.long	.Linfo_string1                  // DW_AT_name
	.long	.Lline_table_start0             // DW_AT_stmt_list
	.long	.Linfo_string2                  // DW_AT_comp_dir
	.long	.Lfunc_begin0                   // DW_AT_low_pc
	.long	.Lfunc_end0-.Lfunc_begin0       // DW_AT_high_pc
	.byte	2                               // Abbrev [2] 0x26:0x11 DW_TAG_variable
	.long	.Linfo_string3                  // DW_AT_name
	.long	55                              // DW_AT_type
                                        // DW_AT_external
	.byte	2                               // DW_AT_decl_file
	.byte	2                               // DW_AT_decl_line
	.byte	5                               // DW_AT_location
	.byte	3
	.long	MAP_RANK_DIMS
	.byte	3                               // Abbrev [3] 0x37:0xc DW_TAG_array_type
	.long	67                              // DW_AT_type
	.byte	4                               // Abbrev [4] 0x3c:0x6 DW_TAG_subrange_type
	.long	85                              // DW_AT_type
	.byte	1                               // DW_AT_count
	.byte	0                               // End Of Children Mark
	.byte	5                               // Abbrev [5] 0x43:0xb DW_TAG_typedef
	.long	78                              // DW_AT_type
	.long	.Linfo_string5                  // DW_AT_name
	.byte	1                               // DW_AT_decl_file
	.byte	48                              // DW_AT_decl_line
	.byte	6                               // Abbrev [6] 0x4e:0x7 DW_TAG_base_type
	.long	.Linfo_string4                  // DW_AT_name
	.byte	7                               // DW_AT_encoding
	.byte	4                               // DW_AT_byte_size
	.byte	7                               // Abbrev [7] 0x55:0x7 DW_TAG_base_type
	.long	.Linfo_string6                  // DW_AT_name
	.byte	8                               // DW_AT_byte_size
	.byte	7                               // DW_AT_encoding
	.byte	2                               // Abbrev [2] 0x5c:0x11 DW_TAG_variable
	.long	.Linfo_string7                  // DW_AT_name
	.long	55                              // DW_AT_type
                                        // DW_AT_external
	.byte	2                               // DW_AT_decl_file
	.byte	3                               // DW_AT_decl_line
	.byte	5                               // DW_AT_location
	.byte	3
	.long	MAP_DPU_DIMS
	.byte	2                               // Abbrev [2] 0x6d:0x11 DW_TAG_variable
	.long	.Linfo_string8                  // DW_AT_name
	.long	55                              // DW_AT_type
                                        // DW_AT_external
	.byte	2                               // DW_AT_decl_file
	.byte	4                               // DW_AT_decl_line
	.byte	5                               // DW_AT_location
	.byte	3
	.long	MAP_THREAD_DIMS
	.byte	2                               // Abbrev [2] 0x7e:0x11 DW_TAG_variable
	.long	.Linfo_string9                  // DW_AT_name
	.long	143                             // DW_AT_type
                                        // DW_AT_external
	.byte	2                               // DW_AT_decl_file
	.byte	5                               // DW_AT_decl_line
	.byte	5                               // DW_AT_location
	.byte	3
	.long	MAP_ITER_DIMS
	.byte	3                               // Abbrev [3] 0x8f:0xc DW_TAG_array_type
	.long	67                              // DW_AT_type
	.byte	4                               // Abbrev [4] 0x94:0x6 DW_TAG_subrange_type
	.long	85                              // DW_AT_type
	.byte	2                               // DW_AT_count
	.byte	0                               // End Of Children Mark
	.byte	2                               // Abbrev [2] 0x9b:0x11 DW_TAG_variable
	.long	.Linfo_string10                 // DW_AT_name
	.long	55                              // DW_AT_type
                                        // DW_AT_external
	.byte	2                               // DW_AT_decl_file
	.byte	6                               // DW_AT_decl_line
	.byte	5                               // DW_AT_location
	.byte	3
	.long	MAP_BUFFER_SIZES
	.byte	8                               // Abbrev [8] 0xac:0x12 DW_TAG_variable
	.long	.Linfo_string11                 // DW_AT_name
	.long	190                             // DW_AT_type
                                        // DW_AT_external
	.byte	4                               // DW_AT_decl_file
	.byte	15                              // DW_AT_decl_line
	.byte	8                               // DW_AT_alignment
	.byte	5                               // DW_AT_location
	.byte	3
	.long	DPU_INPUT_ARGUMENTS
	.byte	5                               // Abbrev [5] 0xbe:0xb DW_TAG_typedef
	.long	201                             // DW_AT_type
	.long	.Linfo_string15                 // DW_AT_name
	.byte	3                               // DW_AT_decl_file
	.byte	6                               // DW_AT_decl_line
	.byte	9                               // Abbrev [9] 0xc9:0x29 DW_TAG_structure_type
	.byte	16                              // DW_AT_byte_size
	.byte	3                               // DW_AT_decl_file
	.byte	2                               // DW_AT_decl_line
	.byte	10                              // Abbrev [10] 0xcd:0xc DW_TAG_member
	.long	.Linfo_string12                 // DW_AT_name
	.long	55                              // DW_AT_type
	.byte	3                               // DW_AT_decl_file
	.byte	3                               // DW_AT_decl_line
	.byte	0                               // DW_AT_data_member_location
	.byte	10                              // Abbrev [10] 0xd9:0xc DW_TAG_member
	.long	.Linfo_string13                 // DW_AT_name
	.long	55                              // DW_AT_type
	.byte	3                               // DW_AT_decl_file
	.byte	4                               // DW_AT_decl_line
	.byte	4                               // DW_AT_data_member_location
	.byte	10                              // Abbrev [10] 0xe5:0xc DW_TAG_member
	.long	.Linfo_string14                 // DW_AT_name
	.long	143                             // DW_AT_type
	.byte	3                               // DW_AT_decl_file
	.byte	5                               // DW_AT_decl_line
	.byte	8                               // DW_AT_data_member_location
	.byte	0                               // End Of Children Mark
	.byte	8                               // Abbrev [8] 0xf2:0x12 DW_TAG_variable
	.long	.Linfo_string16                 // DW_AT_name
	.long	67                              // DW_AT_type
                                        // DW_AT_external
	.byte	4                               // DW_AT_decl_file
	.byte	16                              // DW_AT_decl_line
	.byte	8                               // DW_AT_alignment
	.byte	5                               // DW_AT_location
	.byte	3
	.long	nb_cycle
	.byte	2                               // Abbrev [2] 0x104:0x11 DW_TAG_variable
	.long	.Linfo_string17                 // DW_AT_name
	.long	277                             // DW_AT_type
                                        // DW_AT_external
	.byte	4                               // DW_AT_decl_file
	.byte	19                              // DW_AT_decl_line
	.byte	5                               // DW_AT_location
	.byte	3
	.long	__atomic_bit_barrier_my_barrier
	.byte	5                               // Abbrev [5] 0x115:0xb DW_TAG_typedef
	.long	288                             // DW_AT_type
	.long	.Linfo_string19                 // DW_AT_name
	.byte	1                               // DW_AT_decl_file
	.byte	40                              // DW_AT_decl_line
	.byte	6                               // Abbrev [6] 0x120:0x7 DW_TAG_base_type
	.long	.Linfo_string18                 // DW_AT_name
	.byte	8                               // DW_AT_encoding
	.byte	1                               // DW_AT_byte_size
	.byte	11                              // Abbrev [11] 0x127:0x5 DW_TAG_pointer_type
	.long	67                              // DW_AT_type
	.byte	12                              // Abbrev [12] 0x12c:0x1 DW_TAG_pointer_type
	.byte	11                              // Abbrev [11] 0x12d:0x5 DW_TAG_pointer_type
	.long	306                             // DW_AT_type
	.byte	13                              // Abbrev [13] 0x132:0x1 DW_TAG_const_type
	.byte	14                              // Abbrev [14] 0x133:0xc DW_TAG_subprogram
	.long	.Linfo_string20                 // DW_AT_name
	.byte	5                               // DW_AT_decl_file
	.byte	33                              // DW_AT_decl_line
	.long	319                             // DW_AT_type
	.byte	1                               // DW_AT_inline
	.byte	5                               // Abbrev [5] 0x13f:0xb DW_TAG_typedef
	.long	78                              // DW_AT_type
	.long	.Linfo_string21                 // DW_AT_name
	.byte	7                               // DW_AT_decl_file
	.byte	27                              // DW_AT_decl_line
	.byte	15                              // Abbrev [15] 0x14a:0x2a DW_TAG_subprogram
	.long	.Linfo_string22                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	33                              // DW_AT_decl_line
                                        // DW_AT_prototyped
	.byte	1                               // DW_AT_inline
	.byte	16                              // Abbrev [16] 0x152:0xb DW_TAG_formal_parameter
	.long	.Linfo_string23                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	33                              // DW_AT_decl_line
	.long	301                             // DW_AT_type
	.byte	16                              // Abbrev [16] 0x15d:0xb DW_TAG_formal_parameter
	.long	.Linfo_string24                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	33                              // DW_AT_decl_line
	.long	300                             // DW_AT_type
	.byte	16                              // Abbrev [16] 0x168:0xb DW_TAG_formal_parameter
	.long	.Linfo_string25                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	33                              // DW_AT_decl_line
	.long	78                              // DW_AT_type
	.byte	0                               // End Of Children Mark
	.byte	15                              // Abbrev [15] 0x174:0x2a DW_TAG_subprogram
	.long	.Linfo_string26                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	55                              // DW_AT_decl_line
                                        // DW_AT_prototyped
	.byte	1                               // DW_AT_inline
	.byte	16                              // Abbrev [16] 0x17c:0xb DW_TAG_formal_parameter
	.long	.Linfo_string23                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	55                              // DW_AT_decl_line
	.long	301                             // DW_AT_type
	.byte	16                              // Abbrev [16] 0x187:0xb DW_TAG_formal_parameter
	.long	.Linfo_string24                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	55                              // DW_AT_decl_line
	.long	300                             // DW_AT_type
	.byte	16                              // Abbrev [16] 0x192:0xb DW_TAG_formal_parameter
	.long	.Linfo_string25                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	55                              // DW_AT_decl_line
	.long	78                              // DW_AT_type
	.byte	0                               // End Of Children Mark
	.byte	17                              // Abbrev [17] 0x19e:0x2c8 DW_TAG_subprogram
	.long	.Lfunc_begin0                   // DW_AT_low_pc
	.long	.Lfunc_end0-.Lfunc_begin0       // DW_AT_high_pc
	.byte	1                               // DW_AT_frame_base
	.byte	102
                                        // DW_AT_GNU_all_call_sites
	.long	.Linfo_string27                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	22                              // DW_AT_decl_line
	.long	1126                            // DW_AT_type
                                        // DW_AT_external
	.byte	18                              // Abbrev [18] 0x1b3:0xf DW_TAG_variable
	.long	.Ldebug_loc0                    // DW_AT_location
	.long	.Linfo_string29                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	23                              // DW_AT_decl_line
	.long	78                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x1c2:0xf DW_TAG_variable
	.long	.Ldebug_loc1                    // DW_AT_location
	.long	.Linfo_string30                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	35                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x1d1:0xf DW_TAG_variable
	.long	.Ldebug_loc2                    // DW_AT_location
	.long	.Linfo_string31                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	34                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x1e0:0xf DW_TAG_variable
	.long	.Ldebug_loc3                    // DW_AT_location
	.long	.Linfo_string13                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	37                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x1ef:0xf DW_TAG_variable
	.long	.Ldebug_loc4                    // DW_AT_location
	.long	.Linfo_string32                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	41                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x1fe:0xf DW_TAG_variable
	.long	.Ldebug_loc5                    // DW_AT_location
	.long	.Linfo_string33                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	42                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x20d:0xf DW_TAG_variable
	.long	.Ldebug_loc6                    // DW_AT_location
	.long	.Linfo_string34                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	43                              // DW_AT_decl_line
	.long	295                             // DW_AT_type
	.byte	18                              // Abbrev [18] 0x21c:0xf DW_TAG_variable
	.long	.Ldebug_loc7                    // DW_AT_location
	.long	.Linfo_string35                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	44                              // DW_AT_decl_line
	.long	295                             // DW_AT_type
	.byte	18                              // Abbrev [18] 0x22b:0xf DW_TAG_variable
	.long	.Ldebug_loc8                    // DW_AT_location
	.long	.Linfo_string36                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	45                              // DW_AT_decl_line
	.long	295                             // DW_AT_type
	.byte	19                              // Abbrev [19] 0x23a:0xb DW_TAG_variable
	.long	.Linfo_string37                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	47                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	19                              // Abbrev [19] 0x245:0xb DW_TAG_variable
	.long	.Linfo_string38                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	58                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	19                              // Abbrev [19] 0x250:0xb DW_TAG_variable
	.long	.Linfo_string39                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	57                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	19                              // Abbrev [19] 0x25b:0xb DW_TAG_variable
	.long	.Linfo_string40                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	54                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	19                              // Abbrev [19] 0x266:0xb DW_TAG_variable
	.long	.Linfo_string41                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	53                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x271:0xf DW_TAG_variable
	.long	.Ldebug_loc9                    // DW_AT_location
	.long	.Linfo_string42                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	51                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	19                              // Abbrev [19] 0x280:0xb DW_TAG_variable
	.long	.Linfo_string43                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	50                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x28b:0xf DW_TAG_variable
	.long	.Ldebug_loc10                   // DW_AT_location
	.long	.Linfo_string44                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	48                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x29a:0xf DW_TAG_variable
	.long	.Ldebug_loc11                   // DW_AT_location
	.long	.Linfo_string45                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	61                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	19                              // Abbrev [19] 0x2a9:0xb DW_TAG_variable
	.long	.Linfo_string46                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	73                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x2b4:0xf DW_TAG_variable
	.long	.Ldebug_loc12                   // DW_AT_location
	.long	.Linfo_string47                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	69                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x2c3:0xf DW_TAG_variable
	.long	.Ldebug_loc13                   // DW_AT_location
	.long	.Linfo_string48                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	65                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x2d2:0xf DW_TAG_variable
	.long	.Ldebug_loc14                   // DW_AT_location
	.long	.Linfo_string49                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	56                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x2e1:0xf DW_TAG_variable
	.long	.Ldebug_loc15                   // DW_AT_location
	.long	.Linfo_string50                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	75                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	20                              // Abbrev [20] 0x2f0:0x10 DW_TAG_inlined_subroutine
	.long	307                             // DW_AT_abstract_origin
	.long	.Ltmp0                          // DW_AT_low_pc
	.long	.Ltmp2-.Ltmp0                   // DW_AT_high_pc
	.byte	4                               // DW_AT_call_file
	.byte	23                              // DW_AT_call_line
	.byte	29                              // DW_AT_call_column
	.byte	21                              // Abbrev [21] 0x300:0x103 DW_TAG_lexical_block
	.long	.Ldebug_ranges0                 // DW_AT_ranges
	.byte	18                              // Abbrev [18] 0x305:0xf DW_TAG_variable
	.long	.Ldebug_loc16                   // DW_AT_location
	.long	.Linfo_string51                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	76                              // DW_AT_decl_line
	.long	1126                            // DW_AT_type
	.byte	22                              // Abbrev [22] 0x314:0x26 DW_TAG_inlined_subroutine
	.long	330                             // DW_AT_abstract_origin
	.long	.Ltmp48                         // DW_AT_low_pc
	.long	.Ltmp49-.Ltmp48                 // DW_AT_high_pc
	.byte	4                               // DW_AT_call_file
	.byte	77                              // DW_AT_call_line
	.byte	5                               // DW_AT_call_column
	.byte	23                              // Abbrev [23] 0x324:0x7 DW_TAG_formal_parameter
	.byte	1                               // DW_AT_location
	.byte	81
	.long	338                             // DW_AT_abstract_origin
	.byte	23                              // Abbrev [23] 0x32b:0x7 DW_TAG_formal_parameter
	.byte	1                               // DW_AT_location
	.byte	98
	.long	349                             // DW_AT_abstract_origin
	.byte	23                              // Abbrev [23] 0x332:0x7 DW_TAG_formal_parameter
	.byte	1                               // DW_AT_location
	.byte	97
	.long	360                             // DW_AT_abstract_origin
	.byte	0                               // End Of Children Mark
	.byte	24                              // Abbrev [24] 0x33a:0xc8 DW_TAG_lexical_block
	.long	.Ltmp49                         // DW_AT_low_pc
	.long	.Ltmp80-.Ltmp49                 // DW_AT_high_pc
	.byte	18                              // Abbrev [18] 0x343:0xf DW_TAG_variable
	.long	.Ldebug_loc17                   // DW_AT_location
	.long	.Linfo_string52                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	80                              // DW_AT_decl_line
	.long	1126                            // DW_AT_type
	.byte	22                              // Abbrev [22] 0x352:0x2a DW_TAG_inlined_subroutine
	.long	372                             // DW_AT_abstract_origin
	.long	.Ltmp53                         // DW_AT_low_pc
	.long	.Ltmp57-.Ltmp53                 // DW_AT_high_pc
	.byte	4                               // DW_AT_call_file
	.byte	95                              // DW_AT_call_line
	.byte	7                               // DW_AT_call_column
	.byte	25                              // Abbrev [25] 0x362:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc18                   // DW_AT_location
	.long	380                             // DW_AT_abstract_origin
	.byte	25                              // Abbrev [25] 0x36b:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc19                   // DW_AT_location
	.long	391                             // DW_AT_abstract_origin
	.byte	23                              // Abbrev [23] 0x374:0x7 DW_TAG_formal_parameter
	.byte	1                               // DW_AT_location
	.byte	97
	.long	402                             // DW_AT_abstract_origin
	.byte	0                               // End Of Children Mark
	.byte	24                              // Abbrev [24] 0x37c:0x85 DW_TAG_lexical_block
	.long	.Ltmp62                         // DW_AT_low_pc
	.long	.Ltmp80-.Ltmp62                 // DW_AT_high_pc
	.byte	18                              // Abbrev [18] 0x385:0xf DW_TAG_variable
	.long	.Ldebug_loc20                   // DW_AT_location
	.long	.Linfo_string53                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	81                              // DW_AT_decl_line
	.long	1126                            // DW_AT_type
	.byte	24                              // Abbrev [24] 0x394:0x6c DW_TAG_lexical_block
	.long	.Ltmp67                         // DW_AT_low_pc
	.long	.Ltmp78-.Ltmp67                 // DW_AT_high_pc
	.byte	26                              // Abbrev [26] 0x39d:0xd DW_TAG_variable
	.byte	1                               // DW_AT_location
	.byte	96
	.long	.Linfo_string54                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	82                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	24                              // Abbrev [24] 0x3aa:0x55 DW_TAG_lexical_block
	.long	.Ltmp67                         // DW_AT_low_pc
	.long	.Ltmp78-.Ltmp67                 // DW_AT_high_pc
	.byte	26                              // Abbrev [26] 0x3b3:0xd DW_TAG_variable
	.byte	1                               // DW_AT_location
	.byte	101
	.long	.Linfo_string55                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	84                              // DW_AT_decl_line
	.long	1126                            // DW_AT_type
	.byte	22                              // Abbrev [22] 0x3c0:0x27 DW_TAG_inlined_subroutine
	.long	330                             // DW_AT_abstract_origin
	.long	.Ltmp67                         // DW_AT_low_pc
	.long	.Ltmp68-.Ltmp67                 // DW_AT_high_pc
	.byte	4                               // DW_AT_call_file
	.byte	85                              // DW_AT_call_line
	.byte	11                              // DW_AT_call_column
	.byte	23                              // Abbrev [23] 0x3d0:0x7 DW_TAG_formal_parameter
	.byte	1                               // DW_AT_location
	.byte	96
	.long	338                             // DW_AT_abstract_origin
	.byte	23                              // Abbrev [23] 0x3d7:0x7 DW_TAG_formal_parameter
	.byte	1                               // DW_AT_location
	.byte	99
	.long	349                             // DW_AT_abstract_origin
	.byte	23                              // Abbrev [23] 0x3de:0x8 DW_TAG_formal_parameter
	.byte	2                               // DW_AT_location
	.byte	145
	.byte	76
	.long	360                             // DW_AT_abstract_origin
	.byte	0                               // End Of Children Mark
	.byte	24                              // Abbrev [24] 0x3e7:0x17 DW_TAG_lexical_block
	.long	.Ltmp69                         // DW_AT_low_pc
	.long	.Ltmp74-.Ltmp69                 // DW_AT_high_pc
	.byte	26                              // Abbrev [26] 0x3f0:0xd DW_TAG_variable
	.byte	1                               // DW_AT_location
	.byte	100
	.long	.Linfo_string56                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	89                              // DW_AT_decl_line
	.long	1126                            // DW_AT_type
	.byte	0                               // End Of Children Mark
	.byte	0                               // End Of Children Mark
	.byte	0                               // End Of Children Mark
	.byte	0                               // End Of Children Mark
	.byte	0                               // End Of Children Mark
	.byte	0                               // End Of Children Mark
	.byte	27                              // Abbrev [27] 0x403:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp3                          // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x40a:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp4                          // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x411:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp12                         // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x418:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp14                         // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x41f:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp16                         // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x426:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp18                         // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x42d:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp20                         // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x434:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp22                         // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x43b:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp24                         // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x442:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp25                         // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x449:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp28                         // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x450:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp38                         // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x457:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp40                         // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x45e:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp70                         // DW_AT_low_pc
	.byte	0                               // End Of Children Mark
	.byte	6                               // Abbrev [6] 0x466:0x7 DW_TAG_base_type
	.long	.Linfo_string28                 // DW_AT_name
	.byte	5                               // DW_AT_encoding
	.byte	4                               // DW_AT_byte_size
	.byte	0                               // End Of Children Mark
.Ldebug_info_end0:
	.section	.debug_ranges,"",@progbits
.Ldebug_ranges0:
	.long	.Ltmp30-.Lfunc_begin0
	.long	.Ltmp31-.Lfunc_begin0
	.long	.Ltmp43-.Lfunc_begin0
	.long	.Ltmp80-.Lfunc_begin0
	.long	0
	.long	0
	.section	.debug_str,"MS",@progbits,1
.Linfo_string0:
	.asciz	"clang version 12.0.0 (https://github.com/upmem/llvm-project.git 846fdda8285dcc9b20ee5d2fec9e54dfea6a8928)" // string offset=0
.Linfo_string1:
	.asciz	"dpu.c"                         // string offset=106
.Linfo_string2:
	.asciz	"/home/hamid.farzaneh/CostModelEvaluationRepo/NewEval/Matmul/dpu" // string offset=112
.Linfo_string3:
	.asciz	"MAP_RANK_DIMS"                 // string offset=176
.Linfo_string4:
	.asciz	"unsigned int"                  // string offset=190
.Linfo_string5:
	.asciz	"uint32_t"                      // string offset=203
.Linfo_string6:
	.asciz	"__ARRAY_SIZE_TYPE__"           // string offset=212
.Linfo_string7:
	.asciz	"MAP_DPU_DIMS"                  // string offset=232
.Linfo_string8:
	.asciz	"MAP_THREAD_DIMS"               // string offset=245
.Linfo_string9:
	.asciz	"MAP_ITER_DIMS"                 // string offset=261
.Linfo_string10:
	.asciz	"MAP_BUFFER_SIZES"              // string offset=275
.Linfo_string11:
	.asciz	"DPU_INPUT_ARGUMENTS"           // string offset=292
.Linfo_string12:
	.asciz	"BUFFER_SIZES"                  // string offset=312
.Linfo_string13:
	.asciz	"THREADS"                       // string offset=325
.Linfo_string14:
	.asciz	"ITERS"                         // string offset=333
.Linfo_string15:
	.asciz	"dpu_arguments_t"               // string offset=339
.Linfo_string16:
	.asciz	"nb_cycle"                      // string offset=355
.Linfo_string17:
	.asciz	"__atomic_bit_barrier_my_barrier" // string offset=364
.Linfo_string18:
	.asciz	"unsigned char"                 // string offset=396
.Linfo_string19:
	.asciz	"uint8_t"                       // string offset=410
.Linfo_string20:
	.asciz	"me"                            // string offset=418
.Linfo_string21:
	.asciz	"sysname_t"                     // string offset=421
.Linfo_string22:
	.asciz	"mram_read"                     // string offset=431
.Linfo_string23:
	.asciz	"from"                          // string offset=441
.Linfo_string24:
	.asciz	"to"                            // string offset=446
.Linfo_string25:
	.asciz	"nb_of_bytes"                   // string offset=449
.Linfo_string26:
	.asciz	"mram_write"                    // string offset=461
.Linfo_string27:
	.asciz	"main"                          // string offset=472
.Linfo_string28:
	.asciz	"int"                           // string offset=477
.Linfo_string29:
	.asciz	"tasklet_id"                    // string offset=481
.Linfo_string30:
	.asciz	"ITER_PER_THREAD2"              // string offset=492
.Linfo_string31:
	.asciz	"ITER_PER_THREAD1"              // string offset=509
.Linfo_string32:
	.asciz	"BUFFER_COUNT"                  // string offset=526
.Linfo_string33:
	.asciz	"BUFFER_SIZE"                   // string offset=539
.Linfo_string34:
	.asciz	"cache_A"                       // string offset=551
.Linfo_string35:
	.asciz	"cache_B"                       // string offset=559
.Linfo_string36:
	.asciz	"cache_C"                       // string offset=567
.Linfo_string37:
	.asciz	"Amat_el_count"                 // string offset=575
.Linfo_string38:
	.asciz	"mram_base_addr_C"              // string offset=589
.Linfo_string39:
	.asciz	"mram_base_addr_B"              // string offset=606
.Linfo_string40:
	.asciz	"res_el_size"                   // string offset=623
.Linfo_string41:
	.asciz	"res_el_count"                  // string offset=635
.Linfo_string42:
	.asciz	"Bmat_el_size"                  // string offset=648
.Linfo_string43:
	.asciz	"Bmat_el_count"                 // string offset=661
.Linfo_string44:
	.asciz	"Amat_el_size"                  // string offset=675
.Linfo_string45:
	.asciz	"mram_temp_addr_A"              // string offset=688
.Linfo_string46:
	.asciz	"local_iter_count1"             // string offset=705
.Linfo_string47:
	.asciz	"mram_temp_addr_C"              // string offset=723
.Linfo_string48:
	.asciz	"mram_temp_addr_B"              // string offset=740
.Linfo_string49:
	.asciz	"mram_base_addr_A"              // string offset=757
.Linfo_string50:
	.asciz	"local_iter_count2"             // string offset=774
.Linfo_string51:
	.asciz	"a1"                            // string offset=792
.Linfo_string52:
	.asciz	"b1"                            // string offset=795
.Linfo_string53:
	.asciz	"b2"                            // string offset=798
.Linfo_string54:
	.asciz	"local_mram_temp_addr_B"        // string offset=801
.Linfo_string55:
	.asciz	"y"                             // string offset=824
.Linfo_string56:
	.asciz	"c"                             // string offset=826
	.addrsig
	.addrsig_sym my_barrier
	.addrsig_sym DPU_INPUT_ARGUMENTS
	.addrsig_sym __sys_used_mram_end
	.addrsig_sym nb_cycle
	.section	.debug_line,"",@progbits
.Lline_table_start0:
