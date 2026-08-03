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
	.file	2 "/home/hamid.farzaneh/CostModelEvaluationRepo/NewEval/Hst" "dpu/../generated_headers/gen_map.h"
	.file	3 "/home/hamid.farzaneh/CostModelEvaluationRepo/NewEval/Hst" "dpu/../generated_headers/common_structs.h"
	.file	4 "/home/hamid.farzaneh/CostModelEvaluationRepo/NewEval/Hst" "dpu/dpu.c"
	.section	.text.main,"ax",@progbits
	.globl	main                            // -- Begin function main
	.type	main,@function
main:                                   // @main
.Lfunc_begin0:
	.loc	4 22 0                          // dpu/dpu.c:22:0
	.cfi_sections .debug_frame
	.cfi_startproc
// %bb.0:
	.cfi_def_cfa_offset -64
	.cfi_offset 23, -8
	.cfi_offset 22, -4
	sd r22, 56, d22
	add r22, r22, 64
	.cfi_offset 15, -40
	.cfi_offset 14, -36
.Ltmp0:
	sd r22, -40, d14
	.cfi_offset 17, -48
	.cfi_offset 16, -44
	sd r22, -48, d16
	.cfi_offset 19, -56
	.cfi_offset 18, -52
	sd r22, -56, d18
	.cfi_offset 21, -64
	.cfi_offset 20, -60
	sd r22, -64, d20
.Ltmp1:
	//DEBUG_VALUE: main:tasklet_id <- $r20
	.file	5 "/opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib" "defs.h"
	.loc	5 35 12 prologue_end            // /opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/defs.h:35:12
	move r20, id, nz, .LBB0_2
.Ltmp2:
// %bb.1:
	.loc	4 25 5                          // dpu/dpu.c:25:5
	call r23, mem_reset
.Ltmp3:
.LBB0_2:
	.loc	4 32 3                          // dpu/dpu.c:32:3
	move r0, my_barrier
	call r23, barrier_wait
.Ltmp4:
	.loc	4 34 30                         // dpu/dpu.c:34:30
	move r0, DPU_INPUT_ARGUMENTS
.Ltmp5:
	//DEBUG_VALUE: main:ITER_PER_THREAD <- undef
	.loc	4 35 22                         // dpu/dpu.c:35:22
	lw r15, r0, 4
.Ltmp6:
	//DEBUG_VALUE: main:THREADS <- $r15
	.loc	4 36 7                          // dpu/dpu.c:36:7
	jgeu r20, r15, .LBB0_14
.Ltmp7:
// %bb.3:
	//DEBUG_VALUE: main:THREADS <- $r15
	.loc	4 39 27                         // dpu/dpu.c:39:27
	lw r16, zero, DPU_INPUT_ARGUMENTS
.Ltmp8:
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r16
	.loc	4 0 0 is_stmt 0                 // dpu/dpu.c:0:0
	lw r21, r0, 8
.Ltmp9:
	//DEBUG_VALUE: main:ITER_PER_THREAD <- $r21
	.loc	4 40 39 is_stmt 1               // dpu/dpu.c:40:39
	lw r19, r0, 12
.Ltmp10:
	//DEBUG_VALUE: main:BINS <- $r19
	.loc	4 41 39                         // dpu/dpu.c:41:39
	lsl r14, r16, 2
.Ltmp11:
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r14
	.loc	4 43 25                         // dpu/dpu.c:43:25
	move r0, r14
	call r23, mem_alloc
.Ltmp12:
	move r18, r0
.Ltmp13:
	//DEBUG_VALUE: main:cache_INPUT <- $r18
	.loc	4 44 38                         // dpu/dpu.c:44:38
	lsl r0, r19, 2
	sw r22, -24, r0
	.loc	4 44 23 is_stmt 0               // dpu/dpu.c:44:23
	call r23, mem_alloc
.Ltmp14:
	move r17, r0
.Ltmp15:
	//DEBUG_VALUE: main:arr_el_count <- undef
	//DEBUG_VALUE: main:cache_HST <- $r17
	.loc	4 47 39 is_stmt 1               // dpu/dpu.c:47:39
	move r0, r15
	move r1, r21
	call r23, __mulsi3
.Ltmp16:
	move r15, r0
.Ltmp17:
	//DEBUG_VALUE: main:mram_base_addr_HST <- undef
	//DEBUG_VALUE: main:arr_el_size <- $r15
	.loc	4 53 61                         // dpu/dpu.c:53:61
	move r0, r20
	move r1, r21
	call r23, __mulsi3
.Ltmp18:
	sw r22, -12, r0
	.loc	4 53 28 is_stmt 0               // dpu/dpu.c:53:28
	move r0, __sys_used_mram_end
.Ltmp19:
	//DEBUG_VALUE: main:mram_temp_addr_INPUT <- undef
	//DEBUG_VALUE: main:mram_base_addr_INPUT <- $r0
	sw r22, -16, r0
.Ltmp20:
	//DEBUG_VALUE: main:mram_base_addr_INPUT <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	.loc	4 55 48 is_stmt 1               // dpu/dpu.c:55:48
	move r0, r19
	move r1, r20
	call r23, __mulsi3
.Ltmp21:
	.loc	4 0 48 is_stmt 0                // dpu/dpu.c:0:48
	add r15, r0, r15
.Ltmp22:
	//DEBUG_VALUE: main:mram_temp_addr_HST <- undef
	.loc	4 57 47 is_stmt 1               // dpu/dpu.c:57:47
	move r0, r21
	sw r22, -28, r16
.Ltmp23:
	//DEBUG_VALUE: main:BUFFER_COUNT <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	move r1, r16
	lw r16, r22, -24
	call r23, __udiv32
.Ltmp24:
	sw r22, -32, r0
.Ltmp25:
	//DEBUG_VALUE: main:local_iter_count <- [DW_OP_constu 32, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: b <- 0
	.loc	4 61 3                          // dpu/dpu.c:61:3
	jeq r19, 0, .LBB0_5
.Ltmp26:
// %bb.4:
	//DEBUG_VALUE: main:local_iter_count <- [DW_OP_constu 32, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:BUFFER_COUNT <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_INPUT <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:ITER_PER_THREAD <- $r21
	//DEBUG_VALUE: main:BINS <- $r19
	//DEBUG_VALUE: main:cache_INPUT <- $r18
	//DEBUG_VALUE: main:cache_HST <- $r17
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r14
	//DEBUG_VALUE: b <- 0
	.loc	4 0 3 is_stmt 0                 // dpu/dpu.c:0:3
	move r1, 0
.Ltmp27:
	.loc	4 62 18 is_stmt 1               // dpu/dpu.c:62:18
	move r0, r17
	move r2, r16
	call r23, memset
.Ltmp28:
	//DEBUG_VALUE: b <- [DW_OP_plus_uconst 1, DW_OP_stack_value] undef
.LBB0_5:
	//DEBUG_VALUE: main:local_iter_count <- [DW_OP_constu 32, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:BUFFER_COUNT <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_INPUT <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:ITER_PER_THREAD <- $r21
	//DEBUG_VALUE: main:BINS <- $r19
	//DEBUG_VALUE: main:cache_INPUT <- $r18
	//DEBUG_VALUE: main:cache_HST <- $r17
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r14
	//DEBUG_VALUE: main:mram_temp_addr_INPUT <- undef
	.loc	4 0 18 is_stmt 0                // dpu/dpu.c:0:18
	lw r1, r22, -16
	lsl_add r0, r1, r15, 2
.Ltmp29:
	//DEBUG_VALUE: main:mram_temp_addr_HST <- $r0
	//DEBUG_VALUE: main:mram_temp_addr_INPUT <- undef
	sw r22, -20, r0
.Ltmp30:
	//DEBUG_VALUE: main:mram_temp_addr_HST <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	.loc	4 65 3 is_stmt 1                // dpu/dpu.c:65:3
	lw r0, r22, -28
.Ltmp31:
	//DEBUG_VALUE: r <- 0
	jgtu r0, r21, .LBB0_13
.Ltmp32:
// %bb.6:
	//DEBUG_VALUE: main:mram_temp_addr_HST <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:local_iter_count <- [DW_OP_constu 32, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:BUFFER_COUNT <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_INPUT <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:ITER_PER_THREAD <- $r21
	//DEBUG_VALUE: main:BINS <- $r19
	//DEBUG_VALUE: main:cache_INPUT <- $r18
	//DEBUG_VALUE: main:cache_HST <- $r17
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r14
	//DEBUG_VALUE: r <- 0
	//DEBUG_VALUE: main:mram_temp_addr_INPUT <- undef
	.loc	4 0 3 is_stmt 0                 // dpu/dpu.c:0:3
	lw r0, r22, -32
	jgtu r0, 1, .LBB0_8
.Ltmp33:
// %bb.7:
	//DEBUG_VALUE: main:mram_temp_addr_HST <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:local_iter_count <- [DW_OP_constu 32, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:BUFFER_COUNT <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_INPUT <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:ITER_PER_THREAD <- $r21
	//DEBUG_VALUE: main:BINS <- $r19
	//DEBUG_VALUE: main:cache_INPUT <- $r18
	//DEBUG_VALUE: main:cache_HST <- $r17
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r14
	//DEBUG_VALUE: r <- 0
	move r0, 1
	sw r22, -32, r0
.Ltmp34:
.LBB0_8:
	//DEBUG_VALUE: main:mram_temp_addr_HST <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:local_iter_count <- [DW_OP_constu 32, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:BUFFER_COUNT <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_INPUT <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:ITER_PER_THREAD <- $r21
	//DEBUG_VALUE: main:BINS <- $r19
	//DEBUG_VALUE: main:cache_INPUT <- $r18
	//DEBUG_VALUE: main:cache_HST <- $r17
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r14
	//DEBUG_VALUE: r <- 0
	lw r0, r22, -12
	lsl_add r21, r1, r0, 2
.Ltmp35:
	//DEBUG_VALUE: main:mram_temp_addr_INPUT <- $r21
	.loc	4 65 3                          // dpu/dpu.c:65:3
	lw r0, r22, -28
	jeq r0, 0, .LBB0_15
.Ltmp36:
// %bb.9:
	//DEBUG_VALUE: main:mram_temp_addr_HST <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:local_iter_count <- [DW_OP_constu 32, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:BUFFER_COUNT <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_INPUT <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_temp_addr_INPUT <- $r21
	//DEBUG_VALUE: main:BINS <- $r19
	//DEBUG_VALUE: main:cache_INPUT <- $r18
	//DEBUG_VALUE: main:cache_HST <- $r17
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r14
	//DEBUG_VALUE: r <- 0
	.loc	4 0 3                           // dpu/dpu.c:0:3
	move r15, 0
.Ltmp37:
.LBB0_10:                               // =>This Loop Header: Depth=1
                                        //     Child Loop BB0_11 Depth 2
	//DEBUG_VALUE: main:mram_temp_addr_HST <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:local_iter_count <- [DW_OP_constu 32, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:BUFFER_COUNT <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_INPUT <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_temp_addr_INPUT <- $r21
	//DEBUG_VALUE: main:BINS <- $r19
	//DEBUG_VALUE: main:cache_INPUT <- $r18
	//DEBUG_VALUE: main:cache_HST <- $r17
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r14
	//DEBUG_VALUE: main:mram_temp_addr_INPUT <- $r21
	//DEBUG_VALUE: r <- $r15
	move r0, -1
.Ltmp38:
	//DEBUG_VALUE: mram_read:nb_of_bytes <- $r14
	//DEBUG_VALUE: mram_read:to <- $r18
	//DEBUG_VALUE: mram_read:from <- $r21
	.file	6 "/opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib" "mram.h"
	.loc	6 39 24 is_stmt 1               // /opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/mram.h:39:24
	lsr_add r0, r0, r14, 3
	lsl_add r0, r18, r0, 24
	ldma r0, r21, 0
	//DEBUG_VALUE: c <- 0
	move r16, r18
	lw r20, r22, -28
.Ltmp39:
.LBB0_11:                               //   Parent Loop BB0_10 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	//DEBUG_VALUE: main:mram_temp_addr_HST <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:local_iter_count <- [DW_OP_constu 32, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:BUFFER_COUNT <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_INPUT <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_temp_addr_INPUT <- $r21
	//DEBUG_VALUE: main:BINS <- $r19
	//DEBUG_VALUE: main:cache_INPUT <- $r18
	//DEBUG_VALUE: main:cache_HST <- $r17
	//DEBUG_VALUE: r <- $r15
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r14
	//DEBUG_VALUE: c <- undef
	.loc	4 69 14                         // dpu/dpu.c:69:14
	lw r0, r16, 0
	.loc	4 69 29 is_stmt 0               // dpu/dpu.c:69:29
	move r1, r19
	call r23, __mulsi3
.Ltmp40:
	//DEBUG_VALUE: d <- undef
	.loc	4 70 7 is_stmt 1                // dpu/dpu.c:70:7
	lsr r0, r0, 10
	.loc	4 69 37                         // dpu/dpu.c:69:37
	and r0, r0, 4194300
	.loc	4 70 7                          // dpu/dpu.c:70:7
	add r0, r17, r0
	.loc	4 70 20 is_stmt 0               // dpu/dpu.c:70:20
	lw r1, r0, 0
	add r1, r1, 1
	sw r0, 0, r1
.Ltmp41:
	//DEBUG_VALUE: c <- [DW_OP_plus_uconst 1, DW_OP_stack_value] undef
	.loc	4 68 23 is_stmt 1               // dpu/dpu.c:68:23
	add r16, r16, 4
	add r20, r20, -1, nz, .LBB0_11
.Ltmp42:
// %bb.12:                              //   in Loop: Header=BB0_10 Depth=1
	//DEBUG_VALUE: main:mram_temp_addr_HST <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:local_iter_count <- [DW_OP_constu 32, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:BUFFER_COUNT <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_INPUT <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_temp_addr_INPUT <- $r21
	//DEBUG_VALUE: main:BINS <- $r19
	//DEBUG_VALUE: main:cache_INPUT <- $r18
	//DEBUG_VALUE: main:cache_HST <- $r17
	//DEBUG_VALUE: r <- $r15
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r14
	.loc	4 65 42                         // dpu/dpu.c:65:42
	add r15, r15, 1
.Ltmp43:
	//DEBUG_VALUE: r <- $r15
	.loc	4 72 26                         // dpu/dpu.c:72:26
	add r21, r21, r14
.Ltmp44:
	//DEBUG_VALUE: main:mram_temp_addr_INPUT <- $r21
	.loc	4 65 3                          // dpu/dpu.c:65:3
	lw r0, r22, -32
	jneq r15, r0, .LBB0_10
	jump .LBB0_13
.Ltmp45:
.LBB0_15:
	//DEBUG_VALUE: main:mram_temp_addr_HST <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:local_iter_count <- [DW_OP_constu 32, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:BUFFER_COUNT <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_INPUT <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_temp_addr_INPUT <- $r21
	//DEBUG_VALUE: main:BINS <- $r19
	//DEBUG_VALUE: main:cache_INPUT <- $r18
	//DEBUG_VALUE: main:cache_HST <- $r17
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r14
	//DEBUG_VALUE: r <- 0
	.loc	4 0 3 is_stmt 0                 // dpu/dpu.c:0:3
	move r0, 0
.Ltmp46:
.LBB0_16:                               // =>This Inner Loop Header: Depth=1
	//DEBUG_VALUE: main:mram_temp_addr_HST <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:local_iter_count <- [DW_OP_constu 32, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:BUFFER_COUNT <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_INPUT <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_temp_addr_INPUT <- $r21
	//DEBUG_VALUE: main:BINS <- $r19
	//DEBUG_VALUE: main:cache_INPUT <- $r18
	//DEBUG_VALUE: main:cache_HST <- $r17
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r14
	//DEBUG_VALUE: main:mram_temp_addr_INPUT <- $r21
	move r1, -1
.Ltmp47:
	//DEBUG_VALUE: mram_read:nb_of_bytes <- $r14
	//DEBUG_VALUE: mram_read:to <- $r18
	//DEBUG_VALUE: mram_read:from <- $r21
	//DEBUG_VALUE: r <- $r0
	.loc	6 39 24 is_stmt 1               // /opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/mram.h:39:24
	lsr_add r1, r1, r14, 3
	lsl_add r1, r18, r1, 24
	ldma r1, r21, 0
.Ltmp48:
	//DEBUG_VALUE: c <- 0
	.loc	4 65 42                         // dpu/dpu.c:65:42
	add r0, r0, 1
.Ltmp49:
	//DEBUG_VALUE: r <- $r0
	.loc	4 72 26                         // dpu/dpu.c:72:26
	add r21, r21, r14
.Ltmp50:
	//DEBUG_VALUE: main:mram_temp_addr_INPUT <- $r21
	.loc	4 65 3                          // dpu/dpu.c:65:3
	lw r1, r22, -32
	jneq r0, r1, .LBB0_16
.Ltmp51:
.LBB0_13:
	//DEBUG_VALUE: main:mram_temp_addr_HST <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:local_iter_count <- [DW_OP_constu 32, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:BUFFER_COUNT <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_INPUT <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:BINS <- $r19
	//DEBUG_VALUE: main:cache_INPUT <- $r18
	//DEBUG_VALUE: main:cache_HST <- $r17
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r14
	.loc	4 0 3 is_stmt 0                 // dpu/dpu.c:0:3
	move r0, -1
.Ltmp52:
	.loc	6 61 24 is_stmt 1               // /opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/mram.h:61:24
	lw r1, r22, -24
.Ltmp53:
	//DEBUG_VALUE: mram_write:nb_of_bytes <- $r1
	//DEBUG_VALUE: mram_write:from <- $r17
	//DEBUG_VALUE: mram_write:to <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	lsr_add r0, r0, r1, 3
	lsl_add r0, r17, r0, 24
	lw r1, r22, -20
.Ltmp54:
	//DEBUG_VALUE: mram_write:to <- $r1
	sdma r0, r1, 0
.Ltmp55:
.LBB0_14:
	.loc	6 0 24 is_stmt 0                // /opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/mram.h:0:24
	move r0, 0
	.loc	4 78 1 is_stmt 1                // dpu/dpu.c:78:1
	ld d20, r22, -64
	ld d18, r22, -56
	ld d16, r22, -48
	ld d14, r22, -40
	ld d22, r22, -8
	jump r23
.Ltmp56:
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
	.file	7 "/opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib" "sysdef.h"
	.section	.stack_sizes,"o",@progbits,.text.main
	.long	.Lfunc_begin0
	.byte	64
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
	.long	24                              // 0x18
	.size	MAP_THREAD_DIMS, 4

	.type	MAP_ITER_DIMS,@object           // @MAP_ITER_DIMS
	.section	.data.MAP_ITER_DIMS,"aw",@progbits
	.globl	MAP_ITER_DIMS
	.p2align	2
MAP_ITER_DIMS:
	.long	524288                          // 0x80000
	.size	MAP_ITER_DIMS, 4

	.type	MAP_BUFFER_SIZES,@object        // @MAP_BUFFER_SIZES
	.section	.data.MAP_BUFFER_SIZES,"aw",@progbits
	.globl	MAP_BUFFER_SIZES
	.p2align	2
MAP_BUFFER_SIZES:
	.long	128                             // 0x80
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
	.long	.Ltmp9-.Lfunc_begin0
	.long	.Ltmp35-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	101                             // DW_OP_reg21
	.long	0
	.long	0
.Ldebug_loc2:
	.long	.Ltmp6-.Lfunc_begin0
	.long	.Ltmp17-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	95                              // DW_OP_reg15
	.long	0
	.long	0
.Ldebug_loc3:
	.long	.Ltmp8-.Lfunc_begin0
	.long	.Ltmp23-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	96                              // DW_OP_reg16
	.long	.Ltmp23-.Lfunc_begin0
	.long	.Ltmp55-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	100                             // -28
	.long	0
	.long	0
.Ldebug_loc4:
	.long	.Ltmp10-.Lfunc_begin0
	.long	.Ltmp55-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	99                              // DW_OP_reg19
	.long	0
	.long	0
.Ldebug_loc5:
	.long	.Ltmp11-.Lfunc_begin0
	.long	.Ltmp55-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	94                              // DW_OP_reg14
	.long	0
	.long	0
.Ldebug_loc6:
	.long	.Ltmp13-.Lfunc_begin0
	.long	.Ltmp55-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	98                              // DW_OP_reg18
	.long	0
	.long	0
.Ldebug_loc7:
	.long	.Ltmp15-.Lfunc_begin0
	.long	.Ltmp55-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	97                              // DW_OP_reg17
	.long	0
	.long	0
.Ldebug_loc8:
	.long	.Ltmp17-.Lfunc_begin0
	.long	.Ltmp22-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	95                              // DW_OP_reg15
	.long	0
	.long	0
.Ldebug_loc9:
	.long	.Ltmp35-.Lfunc_begin0
	.long	.Ltmp51-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	101                             // DW_OP_reg21
	.long	0
	.long	0
.Ldebug_loc10:
	.long	.Ltmp19-.Lfunc_begin0
	.long	.Ltmp20-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	80                              // DW_OP_reg0
	.long	.Ltmp20-.Lfunc_begin0
	.long	.Ltmp55-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	112                             // -16
	.long	0
	.long	0
.Ldebug_loc11:
	.long	.Ltmp29-.Lfunc_begin0
	.long	.Ltmp30-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	80                              // DW_OP_reg0
	.long	.Ltmp30-.Lfunc_begin0
	.long	.Ltmp55-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	108                             // -20
	.long	0
	.long	0
.Ldebug_loc12:
	.long	.Ltmp25-.Lfunc_begin0
	.long	.Ltmp55-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	96                              // -32
	.long	0
	.long	0
.Ldebug_loc13:
	.long	.Ltmp25-.Lfunc_begin0
	.long	.Ltmp28-.Lfunc_begin0
	.short	3                               // Loc expr size
	.byte	17                              // DW_OP_consts
	.byte	0                               // 0
	.byte	159                             // DW_OP_stack_value
	.long	0
	.long	0
.Ldebug_loc14:
	.long	.Ltmp31-.Lfunc_begin0
	.long	.Ltmp37-.Lfunc_begin0
	.short	3                               // Loc expr size
	.byte	17                              // DW_OP_consts
	.byte	0                               // 0
	.byte	159                             // DW_OP_stack_value
	.long	.Ltmp37-.Lfunc_begin0
	.long	.Ltmp45-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	95                              // DW_OP_reg15
	.long	.Ltmp45-.Lfunc_begin0
	.long	.Ltmp46-.Lfunc_begin0
	.short	3                               // Loc expr size
	.byte	17                              // DW_OP_consts
	.byte	0                               // 0
	.byte	159                             // DW_OP_stack_value
	.long	.Ltmp47-.Lfunc_begin0
	.long	.Ltmp51-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	80                              // DW_OP_reg0
	.long	0
	.long	0
.Ldebug_loc15:
	.long	.Ltmp38-.Lfunc_begin0
	.long	.Ltmp39-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	94                              // DW_OP_reg14
	.long	.Ltmp47-.Lfunc_begin0
	.long	.Ltmp51-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	94                              // DW_OP_reg14
	.long	0
	.long	0
.Ldebug_loc16:
	.long	.Ltmp38-.Lfunc_begin0
	.long	.Ltmp39-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	98                              // DW_OP_reg18
	.long	.Ltmp47-.Lfunc_begin0
	.long	.Ltmp51-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	98                              // DW_OP_reg18
	.long	0
	.long	0
.Ldebug_loc17:
	.long	.Ltmp38-.Lfunc_begin0
	.long	.Ltmp39-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	101                             // DW_OP_reg21
	.long	.Ltmp47-.Lfunc_begin0
	.long	.Ltmp50-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	101                             // DW_OP_reg21
	.long	0
	.long	0
.Ldebug_loc18:
	.long	.Ltmp53-.Lfunc_begin0
	.long	.Ltmp54-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	0
	.long	0
.Ldebug_loc19:
	.long	.Ltmp53-.Lfunc_begin0
	.long	.Ltmp55-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	97                              // DW_OP_reg17
	.long	0
	.long	0
.Ldebug_loc20:
	.long	.Ltmp53-.Lfunc_begin0
	.long	.Ltmp54-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	108                             // -20
	.long	.Ltmp54-.Lfunc_begin0
	.long	.Ltmp55-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
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
	.byte	17                              // DW_AT_low_pc
	.byte	1                               // DW_FORM_addr
	.byte	18                              // DW_AT_high_pc
	.byte	6                               // DW_FORM_data4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	22                              // Abbreviation Code
	.byte	29                              // DW_TAG_inlined_subroutine
	.byte	1                               // DW_CHILDREN_yes
	.byte	49                              // DW_AT_abstract_origin
	.byte	19                              // DW_FORM_ref4
	.byte	85                              // DW_AT_ranges
	.byte	23                              // DW_FORM_sec_offset
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
	.byte	23                              // DW_FORM_sec_offset
	.byte	49                              // DW_AT_abstract_origin
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	24                              // Abbreviation Code
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
	.byte	25                              // Abbreviation Code
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
	.byte	1                               // Abbrev [1] 0xb:0x390 DW_TAG_compile_unit
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
	.long	55                              // DW_AT_type
                                        // DW_AT_external
	.byte	2                               // DW_AT_decl_file
	.byte	5                               // DW_AT_decl_line
	.byte	5                               // DW_AT_location
	.byte	3
	.long	MAP_ITER_DIMS
	.byte	2                               // Abbrev [2] 0x8f:0x11 DW_TAG_variable
	.long	.Linfo_string10                 // DW_AT_name
	.long	55                              // DW_AT_type
                                        // DW_AT_external
	.byte	2                               // DW_AT_decl_file
	.byte	6                               // DW_AT_decl_line
	.byte	5                               // DW_AT_location
	.byte	3
	.long	MAP_BUFFER_SIZES
	.byte	8                               // Abbrev [8] 0xa0:0x12 DW_TAG_variable
	.long	.Linfo_string11                 // DW_AT_name
	.long	178                             // DW_AT_type
                                        // DW_AT_external
	.byte	4                               // DW_AT_decl_file
	.byte	15                              // DW_AT_decl_line
	.byte	8                               // DW_AT_alignment
	.byte	5                               // DW_AT_location
	.byte	3
	.long	DPU_INPUT_ARGUMENTS
	.byte	5                               // Abbrev [5] 0xb2:0xb DW_TAG_typedef
	.long	189                             // DW_AT_type
	.long	.Linfo_string16                 // DW_AT_name
	.byte	3                               // DW_AT_decl_file
	.byte	7                               // DW_AT_decl_line
	.byte	9                               // Abbrev [9] 0xbd:0x35 DW_TAG_structure_type
	.byte	16                              // DW_AT_byte_size
	.byte	3                               // DW_AT_decl_file
	.byte	2                               // DW_AT_decl_line
	.byte	10                              // Abbrev [10] 0xc1:0xc DW_TAG_member
	.long	.Linfo_string12                 // DW_AT_name
	.long	55                              // DW_AT_type
	.byte	3                               // DW_AT_decl_file
	.byte	3                               // DW_AT_decl_line
	.byte	0                               // DW_AT_data_member_location
	.byte	10                              // Abbrev [10] 0xcd:0xc DW_TAG_member
	.long	.Linfo_string13                 // DW_AT_name
	.long	55                              // DW_AT_type
	.byte	3                               // DW_AT_decl_file
	.byte	4                               // DW_AT_decl_line
	.byte	4                               // DW_AT_data_member_location
	.byte	10                              // Abbrev [10] 0xd9:0xc DW_TAG_member
	.long	.Linfo_string14                 // DW_AT_name
	.long	55                              // DW_AT_type
	.byte	3                               // DW_AT_decl_file
	.byte	5                               // DW_AT_decl_line
	.byte	8                               // DW_AT_data_member_location
	.byte	10                              // Abbrev [10] 0xe5:0xc DW_TAG_member
	.long	.Linfo_string15                 // DW_AT_name
	.long	67                              // DW_AT_type
	.byte	3                               // DW_AT_decl_file
	.byte	6                               // DW_AT_decl_line
	.byte	12                              // DW_AT_data_member_location
	.byte	0                               // End Of Children Mark
	.byte	8                               // Abbrev [8] 0xf2:0x12 DW_TAG_variable
	.long	.Linfo_string17                 // DW_AT_name
	.long	67                              // DW_AT_type
                                        // DW_AT_external
	.byte	4                               // DW_AT_decl_file
	.byte	16                              // DW_AT_decl_line
	.byte	8                               // DW_AT_alignment
	.byte	5                               // DW_AT_location
	.byte	3
	.long	nb_cycle
	.byte	2                               // Abbrev [2] 0x104:0x11 DW_TAG_variable
	.long	.Linfo_string18                 // DW_AT_name
	.long	277                             // DW_AT_type
                                        // DW_AT_external
	.byte	4                               // DW_AT_decl_file
	.byte	19                              // DW_AT_decl_line
	.byte	5                               // DW_AT_location
	.byte	3
	.long	__atomic_bit_barrier_my_barrier
	.byte	5                               // Abbrev [5] 0x115:0xb DW_TAG_typedef
	.long	288                             // DW_AT_type
	.long	.Linfo_string20                 // DW_AT_name
	.byte	1                               // DW_AT_decl_file
	.byte	40                              // DW_AT_decl_line
	.byte	6                               // Abbrev [6] 0x120:0x7 DW_TAG_base_type
	.long	.Linfo_string19                 // DW_AT_name
	.byte	8                               // DW_AT_encoding
	.byte	1                               // DW_AT_byte_size
	.byte	11                              // Abbrev [11] 0x127:0x5 DW_TAG_pointer_type
	.long	67                              // DW_AT_type
	.byte	12                              // Abbrev [12] 0x12c:0x1 DW_TAG_pointer_type
	.byte	11                              // Abbrev [11] 0x12d:0x5 DW_TAG_pointer_type
	.long	306                             // DW_AT_type
	.byte	13                              // Abbrev [13] 0x132:0x1 DW_TAG_const_type
	.byte	14                              // Abbrev [14] 0x133:0xc DW_TAG_subprogram
	.long	.Linfo_string21                 // DW_AT_name
	.byte	5                               // DW_AT_decl_file
	.byte	33                              // DW_AT_decl_line
	.long	319                             // DW_AT_type
	.byte	1                               // DW_AT_inline
	.byte	5                               // Abbrev [5] 0x13f:0xb DW_TAG_typedef
	.long	78                              // DW_AT_type
	.long	.Linfo_string22                 // DW_AT_name
	.byte	7                               // DW_AT_decl_file
	.byte	27                              // DW_AT_decl_line
	.byte	15                              // Abbrev [15] 0x14a:0x2a DW_TAG_subprogram
	.long	.Linfo_string23                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	33                              // DW_AT_decl_line
                                        // DW_AT_prototyped
	.byte	1                               // DW_AT_inline
	.byte	16                              // Abbrev [16] 0x152:0xb DW_TAG_formal_parameter
	.long	.Linfo_string24                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	33                              // DW_AT_decl_line
	.long	301                             // DW_AT_type
	.byte	16                              // Abbrev [16] 0x15d:0xb DW_TAG_formal_parameter
	.long	.Linfo_string25                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	33                              // DW_AT_decl_line
	.long	300                             // DW_AT_type
	.byte	16                              // Abbrev [16] 0x168:0xb DW_TAG_formal_parameter
	.long	.Linfo_string26                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	33                              // DW_AT_decl_line
	.long	78                              // DW_AT_type
	.byte	0                               // End Of Children Mark
	.byte	15                              // Abbrev [15] 0x174:0x2a DW_TAG_subprogram
	.long	.Linfo_string27                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	55                              // DW_AT_decl_line
                                        // DW_AT_prototyped
	.byte	1                               // DW_AT_inline
	.byte	16                              // Abbrev [16] 0x17c:0xb DW_TAG_formal_parameter
	.long	.Linfo_string24                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	55                              // DW_AT_decl_line
	.long	301                             // DW_AT_type
	.byte	16                              // Abbrev [16] 0x187:0xb DW_TAG_formal_parameter
	.long	.Linfo_string25                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	55                              // DW_AT_decl_line
	.long	300                             // DW_AT_type
	.byte	16                              // Abbrev [16] 0x192:0xb DW_TAG_formal_parameter
	.long	.Linfo_string26                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	55                              // DW_AT_decl_line
	.long	78                              // DW_AT_type
	.byte	0                               // End Of Children Mark
	.byte	17                              // Abbrev [17] 0x19e:0x1f5 DW_TAG_subprogram
	.long	.Lfunc_begin0                   // DW_AT_low_pc
	.long	.Lfunc_end0-.Lfunc_begin0       // DW_AT_high_pc
	.byte	1                               // DW_AT_frame_base
	.byte	102
                                        // DW_AT_GNU_all_call_sites
	.long	.Linfo_string28                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	22                              // DW_AT_decl_line
	.long	915                             // DW_AT_type
                                        // DW_AT_external
	.byte	18                              // Abbrev [18] 0x1b3:0xf DW_TAG_variable
	.long	.Ldebug_loc0                    // DW_AT_location
	.long	.Linfo_string30                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	23                              // DW_AT_decl_line
	.long	78                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x1c2:0xf DW_TAG_variable
	.long	.Ldebug_loc1                    // DW_AT_location
	.long	.Linfo_string31                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	34                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x1d1:0xf DW_TAG_variable
	.long	.Ldebug_loc2                    // DW_AT_location
	.long	.Linfo_string13                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	35                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x1e0:0xf DW_TAG_variable
	.long	.Ldebug_loc3                    // DW_AT_location
	.long	.Linfo_string32                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	39                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x1ef:0xf DW_TAG_variable
	.long	.Ldebug_loc4                    // DW_AT_location
	.long	.Linfo_string15                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	40                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x1fe:0xf DW_TAG_variable
	.long	.Ldebug_loc5                    // DW_AT_location
	.long	.Linfo_string33                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	41                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x20d:0xf DW_TAG_variable
	.long	.Ldebug_loc6                    // DW_AT_location
	.long	.Linfo_string34                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	43                              // DW_AT_decl_line
	.long	295                             // DW_AT_type
	.byte	19                              // Abbrev [19] 0x21c:0xb DW_TAG_variable
	.long	.Linfo_string35                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	46                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x227:0xf DW_TAG_variable
	.long	.Ldebug_loc7                    // DW_AT_location
	.long	.Linfo_string36                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	44                              // DW_AT_decl_line
	.long	295                             // DW_AT_type
	.byte	19                              // Abbrev [19] 0x236:0xb DW_TAG_variable
	.long	.Linfo_string37                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	50                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x241:0xf DW_TAG_variable
	.long	.Ldebug_loc8                    // DW_AT_location
	.long	.Linfo_string38                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	47                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x250:0xf DW_TAG_variable
	.long	.Ldebug_loc9                    // DW_AT_location
	.long	.Linfo_string39                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	52                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x25f:0xf DW_TAG_variable
	.long	.Ldebug_loc10                   // DW_AT_location
	.long	.Linfo_string40                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	49                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x26e:0xf DW_TAG_variable
	.long	.Ldebug_loc11                   // DW_AT_location
	.long	.Linfo_string41                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	54                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x27d:0xf DW_TAG_variable
	.long	.Ldebug_loc12                   // DW_AT_location
	.long	.Linfo_string42                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	57                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	20                              // Abbrev [20] 0x28c:0x10 DW_TAG_inlined_subroutine
	.long	307                             // DW_AT_abstract_origin
	.long	.Ltmp0                          // DW_AT_low_pc
	.long	.Ltmp2-.Ltmp0                   // DW_AT_high_pc
	.byte	4                               // DW_AT_call_file
	.byte	23                              // DW_AT_call_line
	.byte	29                              // DW_AT_call_column
	.byte	21                              // Abbrev [21] 0x29c:0x19 DW_TAG_lexical_block
	.long	.Ltmp25                         // DW_AT_low_pc
	.long	.Ltmp28-.Ltmp25                 // DW_AT_high_pc
	.byte	18                              // Abbrev [18] 0x2a5:0xf DW_TAG_variable
	.long	.Ldebug_loc13                   // DW_AT_location
	.long	.Linfo_string43                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	61                              // DW_AT_decl_line
	.long	915                             // DW_AT_type
	.byte	0                               // End Of Children Mark
	.byte	21                              // Abbrev [21] 0x2b5:0x6b DW_TAG_lexical_block
	.long	.Ltmp29                         // DW_AT_low_pc
	.long	.Ltmp51-.Ltmp29                 // DW_AT_high_pc
	.byte	18                              // Abbrev [18] 0x2be:0xf DW_TAG_variable
	.long	.Ldebug_loc14                   // DW_AT_location
	.long	.Linfo_string44                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	65                              // DW_AT_decl_line
	.long	915                             // DW_AT_type
	.byte	22                              // Abbrev [22] 0x2cd:0x28 DW_TAG_inlined_subroutine
	.long	330                             // DW_AT_abstract_origin
	.long	.Ldebug_ranges0                 // DW_AT_ranges
	.byte	4                               // DW_AT_call_file
	.byte	66                              // DW_AT_call_line
	.byte	5                               // DW_AT_call_column
	.byte	23                              // Abbrev [23] 0x2d9:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc17                   // DW_AT_location
	.long	338                             // DW_AT_abstract_origin
	.byte	23                              // Abbrev [23] 0x2e2:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc16                   // DW_AT_location
	.long	349                             // DW_AT_abstract_origin
	.byte	23                              // Abbrev [23] 0x2eb:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc15                   // DW_AT_location
	.long	360                             // DW_AT_abstract_origin
	.byte	0                               // End Of Children Mark
	.byte	21                              // Abbrev [21] 0x2f5:0x2a DW_TAG_lexical_block
	.long	.Ltmp39                         // DW_AT_low_pc
	.long	.Ltmp42-.Ltmp39                 // DW_AT_high_pc
	.byte	19                              // Abbrev [19] 0x2fe:0xb DW_TAG_variable
	.long	.Linfo_string45                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	68                              // DW_AT_decl_line
	.long	915                             // DW_AT_type
	.byte	21                              // Abbrev [21] 0x309:0x15 DW_TAG_lexical_block
	.long	.Ltmp39                         // DW_AT_low_pc
	.long	.Ltmp41-.Ltmp39                 // DW_AT_high_pc
	.byte	19                              // Abbrev [19] 0x312:0xb DW_TAG_variable
	.long	.Linfo_string46                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	69                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	0                               // End Of Children Mark
	.byte	0                               // End Of Children Mark
	.byte	0                               // End Of Children Mark
	.byte	24                              // Abbrev [24] 0x320:0x2c DW_TAG_inlined_subroutine
	.long	372                             // DW_AT_abstract_origin
	.long	.Ltmp52                         // DW_AT_low_pc
	.long	.Ltmp55-.Ltmp52                 // DW_AT_high_pc
	.byte	4                               // DW_AT_call_file
	.byte	74                              // DW_AT_call_line
	.byte	3                               // DW_AT_call_column
	.byte	23                              // Abbrev [23] 0x330:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc19                   // DW_AT_location
	.long	380                             // DW_AT_abstract_origin
	.byte	23                              // Abbrev [23] 0x339:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc20                   // DW_AT_location
	.long	391                             // DW_AT_abstract_origin
	.byte	23                              // Abbrev [23] 0x342:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc18                   // DW_AT_location
	.long	402                             // DW_AT_abstract_origin
	.byte	0                               // End Of Children Mark
	.byte	25                              // Abbrev [25] 0x34c:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp3                          // DW_AT_low_pc
	.byte	25                              // Abbrev [25] 0x353:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp4                          // DW_AT_low_pc
	.byte	25                              // Abbrev [25] 0x35a:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp12                         // DW_AT_low_pc
	.byte	25                              // Abbrev [25] 0x361:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp14                         // DW_AT_low_pc
	.byte	25                              // Abbrev [25] 0x368:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp16                         // DW_AT_low_pc
	.byte	25                              // Abbrev [25] 0x36f:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp18                         // DW_AT_low_pc
	.byte	25                              // Abbrev [25] 0x376:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp21                         // DW_AT_low_pc
	.byte	25                              // Abbrev [25] 0x37d:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp24                         // DW_AT_low_pc
	.byte	25                              // Abbrev [25] 0x384:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp28                         // DW_AT_low_pc
	.byte	25                              // Abbrev [25] 0x38b:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp40                         // DW_AT_low_pc
	.byte	0                               // End Of Children Mark
	.byte	6                               // Abbrev [6] 0x393:0x7 DW_TAG_base_type
	.long	.Linfo_string29                 // DW_AT_name
	.byte	5                               // DW_AT_encoding
	.byte	4                               // DW_AT_byte_size
	.byte	0                               // End Of Children Mark
.Ldebug_info_end0:
	.section	.debug_ranges,"",@progbits
.Ldebug_ranges0:
	.long	.Ltmp38-.Lfunc_begin0
	.long	.Ltmp39-.Lfunc_begin0
	.long	.Ltmp47-.Lfunc_begin0
	.long	.Ltmp48-.Lfunc_begin0
	.long	0
	.long	0
	.section	.debug_str,"MS",@progbits,1
.Linfo_string0:
	.asciz	"clang version 12.0.0 (https://github.com/upmem/llvm-project.git 846fdda8285dcc9b20ee5d2fec9e54dfea6a8928)" // string offset=0
.Linfo_string1:
	.asciz	"dpu/dpu.c"                     // string offset=106
.Linfo_string2:
	.asciz	"/home/hamid.farzaneh/CostModelEvaluationRepo/NewEval/Hst" // string offset=116
.Linfo_string3:
	.asciz	"MAP_RANK_DIMS"                 // string offset=173
.Linfo_string4:
	.asciz	"unsigned int"                  // string offset=187
.Linfo_string5:
	.asciz	"uint32_t"                      // string offset=200
.Linfo_string6:
	.asciz	"__ARRAY_SIZE_TYPE__"           // string offset=209
.Linfo_string7:
	.asciz	"MAP_DPU_DIMS"                  // string offset=229
.Linfo_string8:
	.asciz	"MAP_THREAD_DIMS"               // string offset=242
.Linfo_string9:
	.asciz	"MAP_ITER_DIMS"                 // string offset=258
.Linfo_string10:
	.asciz	"MAP_BUFFER_SIZES"              // string offset=272
.Linfo_string11:
	.asciz	"DPU_INPUT_ARGUMENTS"           // string offset=289
.Linfo_string12:
	.asciz	"BUFFER_SIZES"                  // string offset=309
.Linfo_string13:
	.asciz	"THREADS"                       // string offset=322
.Linfo_string14:
	.asciz	"ITERS"                         // string offset=330
.Linfo_string15:
	.asciz	"BINS"                          // string offset=336
.Linfo_string16:
	.asciz	"dpu_arguments_t"               // string offset=341
.Linfo_string17:
	.asciz	"nb_cycle"                      // string offset=357
.Linfo_string18:
	.asciz	"__atomic_bit_barrier_my_barrier" // string offset=366
.Linfo_string19:
	.asciz	"unsigned char"                 // string offset=398
.Linfo_string20:
	.asciz	"uint8_t"                       // string offset=412
.Linfo_string21:
	.asciz	"me"                            // string offset=420
.Linfo_string22:
	.asciz	"sysname_t"                     // string offset=423
.Linfo_string23:
	.asciz	"mram_read"                     // string offset=433
.Linfo_string24:
	.asciz	"from"                          // string offset=443
.Linfo_string25:
	.asciz	"to"                            // string offset=448
.Linfo_string26:
	.asciz	"nb_of_bytes"                   // string offset=451
.Linfo_string27:
	.asciz	"mram_write"                    // string offset=463
.Linfo_string28:
	.asciz	"main"                          // string offset=474
.Linfo_string29:
	.asciz	"int"                           // string offset=479
.Linfo_string30:
	.asciz	"tasklet_id"                    // string offset=483
.Linfo_string31:
	.asciz	"ITER_PER_THREAD"               // string offset=494
.Linfo_string32:
	.asciz	"BUFFER_COUNT"                  // string offset=510
.Linfo_string33:
	.asciz	"BUFFER_SIZE"                   // string offset=523
.Linfo_string34:
	.asciz	"cache_INPUT"                   // string offset=535
.Linfo_string35:
	.asciz	"arr_el_count"                  // string offset=547
.Linfo_string36:
	.asciz	"cache_HST"                     // string offset=560
.Linfo_string37:
	.asciz	"mram_base_addr_HST"            // string offset=570
.Linfo_string38:
	.asciz	"arr_el_size"                   // string offset=589
.Linfo_string39:
	.asciz	"mram_temp_addr_INPUT"          // string offset=601
.Linfo_string40:
	.asciz	"mram_base_addr_INPUT"          // string offset=622
.Linfo_string41:
	.asciz	"mram_temp_addr_HST"            // string offset=643
.Linfo_string42:
	.asciz	"local_iter_count"              // string offset=662
.Linfo_string43:
	.asciz	"b"                             // string offset=679
.Linfo_string44:
	.asciz	"r"                             // string offset=681
.Linfo_string45:
	.asciz	"c"                             // string offset=683
.Linfo_string46:
	.asciz	"d"                             // string offset=685
	.addrsig
	.addrsig_sym my_barrier
	.addrsig_sym DPU_INPUT_ARGUMENTS
	.addrsig_sym __sys_used_mram_end
	.addrsig_sym nb_cycle
	.section	.debug_line,"",@progbits
.Lline_table_start0:
