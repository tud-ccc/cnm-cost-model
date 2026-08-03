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
	.file	2 "/home/hamid.farzaneh/CostModelEvaluationRepo/NewEval/Gemm" "dpu/../generated_headers/gen_map.h"
	.file	3 "/home/hamid.farzaneh/CostModelEvaluationRepo/NewEval/Gemm" "dpu/../generated_headers/common_structs.h"
	.file	4 "/home/hamid.farzaneh/CostModelEvaluationRepo/NewEval/Gemm" "dpu/dpu.c"
	.section	.text.main,"ax",@progbits
	.globl	main                            // -- Begin function main
	.type	main,@function
main:                                   // @main
.Lfunc_begin0:
	.loc	4 22 0                          // dpu/dpu.c:22:0
	.cfi_sections .debug_frame
	.cfi_startproc
// %bb.0:
	.cfi_def_cfa_offset -120
	.cfi_offset 23, -8
	.cfi_offset 22, -4
	sd r22, 112, d22
	add r22, r22, 120
	.cfi_offset 15, -96
	.cfi_offset 14, -92
.Ltmp0:
	sd r22, -96, d14
	.cfi_offset 17, -104
	.cfi_offset 16, -100
	sd r22, -104, d16
	.cfi_offset 19, -112
	.cfi_offset 18, -108
	sd r22, -112, d18
	.cfi_offset 21, -120
	.cfi_offset 20, -116
	sd r22, -120, d20
.Ltmp1:
	//DEBUG_VALUE: main:tasklet_id <- $r14
	.file	5 "/opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib" "defs.h"
	.loc	5 35 12 prologue_end            // /opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/defs.h:35:12
	move r14, id, nz, .LBB0_2
.Ltmp2:
// %bb.1:
	.loc	4 25 5                          // dpu/dpu.c:25:5
	call r23, mem_reset
.Ltmp3:
.LBB0_2:
	.loc	4 39 3                          // dpu/dpu.c:39:3
	move r0, my_barrier
	call r23, barrier_wait
.Ltmp4:
	.loc	4 41 22                         // dpu/dpu.c:41:22
	move r0, DPU_INPUT_ARGUMENTS
	lw r16, r0, 4
.Ltmp5:
	//DEBUG_VALUE: main:THREADS <- $r16
	.loc	4 42 7                          // dpu/dpu.c:42:7
	jgeu r14, r16, .LBB0_20
.Ltmp6:
// %bb.3:
	//DEBUG_VALUE: main:THREADS <- $r16
	.loc	4 45 24                         // dpu/dpu.c:45:24
	lw r17, r0, 8
.Ltmp7:
	//DEBUG_VALUE: main:ITERS_ROW <- $r17
	.loc	4 49 27                         // dpu/dpu.c:49:27
	lw r15, zero, DPU_INPUT_ARGUMENTS
.Ltmp8:
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r15
	.loc	4 46 16                         // dpu/dpu.c:46:16
	lw r21, r0, 12
.Ltmp9:
	//DEBUG_VALUE: main:K <- $r21
	.loc	4 47 16                         // dpu/dpu.c:47:16
	lw r0, r0, 16
.Ltmp10:
	//DEBUG_VALUE: main:N <- $r0
	sw r22, -20, r0
.Ltmp11:
	//DEBUG_VALUE: main:N <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	.loc	4 50 39                         // dpu/dpu.c:50:39
	lsl r18, r15, 2
.Ltmp12:
	//DEBUG_VALUE: main:total_rows <- undef
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r18
	.loc	4 54 21                         // dpu/dpu.c:54:21
	move r0, r18
	call r23, mem_alloc
.Ltmp13:
	move r19, r0
.Ltmp14:
	//DEBUG_VALUE: main:cache_A <- $r19
	.loc	4 55 21                         // dpu/dpu.c:55:21
	move r0, r18
	call r23, mem_alloc
.Ltmp15:
	move r20, r0
.Ltmp16:
	//DEBUG_VALUE: main:cache_B <- $r20
	.loc	4 56 21                         // dpu/dpu.c:56:21
	move r0, r18
	call r23, mem_alloc
.Ltmp17:
	sw r22, -80, r0
.Ltmp18:
	//DEBUG_VALUE: main:cache_C <- [DW_OP_constu 80, DW_OP_minus] [$r22+0]
	.loc	4 58 38                         // dpu/dpu.c:58:38
	move r0, r16
	move r1, r17
	call r23, __mulsi3
.Ltmp19:
	.loc	4 58 42 is_stmt 0               // dpu/dpu.c:58:42
	move r1, r21
	call r23, __mulsi3
.Ltmp20:
	move r16, r0
.Ltmp21:
	//DEBUG_VALUE: main:mram_base_addr_C <- undef
	//DEBUG_VALUE: main:mram_base_addr_B <- undef
	//DEBUG_VALUE: main:mram_base_addr_A <- undef
	//DEBUG_VALUE: main:matA_el_size <- undef
	//DEBUG_VALUE: main:matB_el_size <- [DW_OP_constu 4, DW_OP_mul, DW_OP_stack_value] $r21
	.loc	4 72 38 is_stmt 1               // dpu/dpu.c:72:38
	move r0, r17
	move r1, r14
	call r23, __mulsi3
.Ltmp22:
	move r14, r0
.Ltmp23:
	//DEBUG_VALUE: main:mram_tile_addr_C <- undef
	//DEBUG_VALUE: main:mram_tile_addr_A <- undef
	sw r22, -40, r21
.Ltmp24:
	//DEBUG_VALUE: main:matB_el_size <- [DW_OP_constu 40, DW_OP_minus, DW_OP_deref_size 4, DW_OP_constu 4, DW_OP_mul, DW_OP_stack_value] $r22
	//DEBUG_VALUE: main:K <- [DW_OP_constu 40, DW_OP_minus] [$r22+0]
	.loc	4 80 29                         // dpu/dpu.c:80:29
	move r0, r21
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r18
	move r1, r15
	call r23, __udiv32
.Ltmp25:
	sw r22, -60, r0
.Ltmp26:
	//DEBUG_VALUE: main:num_k_chunks <- [DW_OP_constu 60, DW_OP_minus] [$r22+0]
	.loc	4 0 29 is_stmt 0                // dpu/dpu.c:0:29
	lw r0, r22, -20
	//DEBUG_VALUE: main:N <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
.Ltmp27:
	//DEBUG_VALUE: main:N <- $r0
	.loc	4 81 28 is_stmt 1               // dpu/dpu.c:81:28
	move r1, r15
	call r23, __udiv32
.Ltmp28:
	sw r22, -36, r0
.Ltmp29:
	//DEBUG_VALUE: main:write_addr_C <- undef
	//DEBUG_VALUE: main:row_addr_A <- undef
	//DEBUG_VALUE: main:num_n_tiles <- [DW_OP_constu 36, DW_OP_minus] [$r22+0]
	sw r22, -16, r17
.Ltmp30:
	//DEBUG_VALUE: row <- 0
	//DEBUG_VALUE: main:ITERS_ROW <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	.loc	4 86 3                          // dpu/dpu.c:86:3
	jeq r17, 0, .LBB0_20
.Ltmp31:
// %bb.4:
	//DEBUG_VALUE: main:ITERS_ROW <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_n_tiles <- [DW_OP_constu 36, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_k_chunks <- [DW_OP_constu 60, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:K <- [DW_OP_constu 40, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:matB_el_size <- [DW_OP_constu 40, DW_OP_minus, DW_OP_deref_size 4, DW_OP_constu 4, DW_OP_mul, DW_OP_stack_value] $r22
	//DEBUG_VALUE: main:cache_C <- [DW_OP_constu 80, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:cache_B <- $r20
	//DEBUG_VALUE: main:cache_A <- $r19
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r18
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r15
	//DEBUG_VALUE: main:N <- $r0
	.loc	4 0 3 is_stmt 0                 // dpu/dpu.c:0:3
	move r17, __sys_used_mram_end
.Ltmp32:
	//DEBUG_VALUE: main:mram_base_addr_A <- $r17
	lsl_add r21, r17, r16, 2
.Ltmp33:
	//DEBUG_VALUE: main:mram_base_addr_B <- $r21
	lw r16, r22, -40
	add r0, r14, r16
.Ltmp34:
	lw r1, r22, -20
	call r23, __mulsi3
.Ltmp35:
	sw r22, -12, r21
.Ltmp36:
	//DEBUG_VALUE: main:mram_base_addr_B <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	.loc	4 74 24 is_stmt 1               // dpu/dpu.c:74:24
	lsl_add r21, r21, r0, 2
.Ltmp37:
	//DEBUG_VALUE: main:write_addr_C <- $r21
	//DEBUG_VALUE: main:mram_tile_addr_C <- $r21
	.loc	4 72 50                         // dpu/dpu.c:72:50
	lsl r16, r16, 2
	.loc	4 72 55 is_stmt 0               // dpu/dpu.c:72:55
	move r0, r16
	move r1, r14
	call r23, __mulsi3
.Ltmp38:
	.loc	4 72 24                         // dpu/dpu.c:72:24
	add r0, r0, r17
.Ltmp39:
	//DEBUG_VALUE: main:row_addr_A <- $r0
	//DEBUG_VALUE: main:mram_tile_addr_A <- $r0
	sw r22, -28, r0
.Ltmp40:
	//DEBUG_VALUE: main:row_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_tile_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	sw r22, -76, r16
	move r0, r16
	move r1, r15
	call r23, __mulsi3
.Ltmp41:
	sw r22, -32, r0
	move r0, 0
	sw r22, -24, r0
	sw r22, -56, r18
	jump .LBB0_5
.Ltmp42:
.LBB0_19:                               //   in Loop: Header=BB0_5 Depth=1
	//DEBUG_VALUE: row <- [DW_OP_constu 24, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_tile_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:row_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_B <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:ITERS_ROW <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_n_tiles <- [DW_OP_constu 36, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_k_chunks <- [DW_OP_constu 60, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:K <- [DW_OP_constu 40, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:matB_el_size <- [DW_OP_constu 40, DW_OP_minus, DW_OP_deref_size 4, DW_OP_constu 4, DW_OP_mul, DW_OP_stack_value] $r22
	//DEBUG_VALUE: main:cache_C <- [DW_OP_constu 80, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:write_addr_C <- $r21
	//DEBUG_VALUE: main:cache_B <- $r20
	//DEBUG_VALUE: main:cache_A <- $r19
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r15
	.loc	4 0 24                          // dpu/dpu.c:0:24
	lw r2, r22, -24
.Ltmp43:
	.loc	4 86 46 is_stmt 1               // dpu/dpu.c:86:46
	add r2, r2, 1
.Ltmp44:
	//DEBUG_VALUE: row <- $r2
	//DEBUG_VALUE: main:write_addr_C <- $r21
	.loc	4 123 16                        // dpu/dpu.c:123:16
	lw r0, r22, -76
	lw r1, r22, -28
	add r1, r1, r0
.Ltmp45:
	//DEBUG_VALUE: main:row_addr_A <- $r1
	sw r22, -28, r1
.Ltmp46:
	//DEBUG_VALUE: main:row_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	.loc	4 86 3                          // dpu/dpu.c:86:3
	lw r0, r22, -16
	sw r22, -24, r2
.Ltmp47:
	//DEBUG_VALUE: row <- [DW_OP_constu 24, DW_OP_minus] [$r22+0]
	jeq r2, r0, .LBB0_20
.Ltmp48:
.LBB0_5:                                // =>This Loop Header: Depth=1
                                        //     Child Loop BB0_7 Depth 2
                                        //       Child Loop BB0_12 Depth 3
                                        //         Child Loop BB0_13 Depth 4
                                        //           Child Loop BB0_14 Depth 5
                                        //       Child Loop BB0_10 Depth 3
	//DEBUG_VALUE: main:mram_tile_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:row_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_B <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:ITERS_ROW <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_n_tiles <- [DW_OP_constu 36, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_k_chunks <- [DW_OP_constu 60, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:K <- [DW_OP_constu 40, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:matB_el_size <- [DW_OP_constu 40, DW_OP_minus, DW_OP_deref_size 4, DW_OP_constu 4, DW_OP_mul, DW_OP_stack_value] $r22
	//DEBUG_VALUE: main:cache_C <- [DW_OP_constu 80, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:write_addr_C <- $r21
	//DEBUG_VALUE: main:cache_B <- $r20
	//DEBUG_VALUE: main:cache_A <- $r19
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r15
	//DEBUG_VALUE: main:row_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:write_addr_C <- $r21
	//DEBUG_VALUE: row <- [DW_OP_constu 24, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: n_tile <- 0
	//DEBUG_VALUE: ntile_row0_addr_B <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	.loc	4 89 5                          // dpu/dpu.c:89:5
	lw r0, r22, -20
	jgtu r15, r0, .LBB0_19
.Ltmp49:
// %bb.6:                               //   in Loop: Header=BB0_5 Depth=1
	//DEBUG_VALUE: ntile_row0_addr_B <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: row <- [DW_OP_constu 24, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_tile_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:row_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_B <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:ITERS_ROW <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_n_tiles <- [DW_OP_constu 36, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_k_chunks <- [DW_OP_constu 60, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:K <- [DW_OP_constu 40, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:matB_el_size <- [DW_OP_constu 40, DW_OP_minus, DW_OP_deref_size 4, DW_OP_constu 4, DW_OP_mul, DW_OP_stack_value] $r22
	//DEBUG_VALUE: main:cache_C <- [DW_OP_constu 80, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:write_addr_C <- $r21
	//DEBUG_VALUE: main:cache_B <- $r20
	//DEBUG_VALUE: main:cache_A <- $r19
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r15
	//DEBUG_VALUE: n_tile <- 0
	.loc	4 0 5 is_stmt 0                 // dpu/dpu.c:0:5
	move r1, 0
	lw r0, r22, -12
	sw r22, -52, r0
	jump .LBB0_7
.Ltmp50:
.LBB0_18:                               //   in Loop: Header=BB0_7 Depth=2
	//DEBUG_VALUE: ntile_row0_addr_B <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: row <- [DW_OP_constu 24, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_tile_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:row_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_B <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:ITERS_ROW <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_n_tiles <- [DW_OP_constu 36, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_k_chunks <- [DW_OP_constu 60, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:K <- [DW_OP_constu 40, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:matB_el_size <- [DW_OP_constu 40, DW_OP_minus, DW_OP_deref_size 4, DW_OP_constu 4, DW_OP_mul, DW_OP_stack_value] $r22
	//DEBUG_VALUE: main:cache_C <- [DW_OP_constu 80, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:cache_B <- $r20
	//DEBUG_VALUE: main:cache_A <- $r19
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r15
	move r0, -1
.Ltmp51:
	//DEBUG_VALUE: mram_write:nb_of_bytes <- $r18
	//DEBUG_VALUE: mram_write:from <- [DW_OP_constu 80, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: mram_write:to <- [DW_OP_constu 48, DW_OP_minus] [$r22+0]
	.file	6 "/opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib" "mram.h"
	.loc	6 61 24 is_stmt 1               // /opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/mram.h:61:24
	lsr_add r0, r0, r18, 3
	lw r1, r22, -80
.Ltmp52:
	//DEBUG_VALUE: mram_write:from <- $r1
	lsl_add r0, r1, r0, 24
.Ltmp53:
	//DEBUG_VALUE: mram_write:from <- [DW_OP_constu 80, DW_OP_minus] [$r22+0]
	.loc	6 0 24 is_stmt 0                // /opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/mram.h:0:24
	lw r21, r22, -48
.Ltmp54:
	//DEBUG_VALUE: mram_write:to <- $r21
	.loc	6 61 24                         // /opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/mram.h:61:24
	sdma r0, r21, 0
.Ltmp55:
	.loc	4 119 20 is_stmt 1              // dpu/dpu.c:119:20
	add r21, r21, r18
.Ltmp56:
	//DEBUG_VALUE: main:write_addr_C <- $r21
	.loc	4 0 20 is_stmt 0                // dpu/dpu.c:0:20
	lw r1, r22, -44
.Ltmp57:
	.loc	4 89 59 is_stmt 1               // dpu/dpu.c:89:59
	add r1, r1, 1
.Ltmp58:
	//DEBUG_VALUE: n_tile <- $r1
	.loc	4 120 25                        // dpu/dpu.c:120:25
	lw r0, r22, -32
	lw r2, r22, -52
	add r2, r2, r0
.Ltmp59:
	//DEBUG_VALUE: ntile_row0_addr_B <- $r2
	sw r22, -52, r2
.Ltmp60:
	//DEBUG_VALUE: ntile_row0_addr_B <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	.loc	4 89 5                          // dpu/dpu.c:89:5
	lw r0, r22, -36
	jgeu r1, r0, .LBB0_19
.Ltmp61:
.LBB0_7:                                //   Parent Loop BB0_5 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB0_12 Depth 3
                                        //         Child Loop BB0_13 Depth 4
                                        //           Child Loop BB0_14 Depth 5
                                        //       Child Loop BB0_10 Depth 3
	//DEBUG_VALUE: row <- [DW_OP_constu 24, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_tile_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:row_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_B <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:ITERS_ROW <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_n_tiles <- [DW_OP_constu 36, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_k_chunks <- [DW_OP_constu 60, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:K <- [DW_OP_constu 40, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:matB_el_size <- [DW_OP_constu 40, DW_OP_minus, DW_OP_deref_size 4, DW_OP_constu 4, DW_OP_mul, DW_OP_stack_value] $r22
	//DEBUG_VALUE: main:cache_C <- [DW_OP_constu 80, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:write_addr_C <- $r21
	//DEBUG_VALUE: main:cache_B <- $r20
	//DEBUG_VALUE: main:cache_A <- $r19
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r15
	//DEBUG_VALUE: main:write_addr_C <- $r21
	//DEBUG_VALUE: n_tile <- $r1
	//DEBUG_VALUE: ntile_row0_addr_B <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: col <- 0
	sw r22, -48, r21
	sw r22, -44, r1
	.loc	4 90 7                          // dpu/dpu.c:90:7
	jeq r15, 0, .LBB0_8
.Ltmp62:
// %bb.17:                              //   in Loop: Header=BB0_7 Depth=2
	//DEBUG_VALUE: ntile_row0_addr_B <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: row <- [DW_OP_constu 24, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_tile_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:row_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_B <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:ITERS_ROW <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_n_tiles <- [DW_OP_constu 36, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_k_chunks <- [DW_OP_constu 60, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:K <- [DW_OP_constu 40, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:matB_el_size <- [DW_OP_constu 40, DW_OP_minus, DW_OP_deref_size 4, DW_OP_constu 4, DW_OP_mul, DW_OP_stack_value] $r22
	//DEBUG_VALUE: main:cache_C <- [DW_OP_constu 80, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:write_addr_C <- $r21
	//DEBUG_VALUE: main:cache_B <- $r20
	//DEBUG_VALUE: main:cache_A <- $r19
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r15
	//DEBUG_VALUE: n_tile <- $r1
	//DEBUG_VALUE: col <- 0
	.loc	4 0 7 is_stmt 0                 // dpu/dpu.c:0:7
	move r14, 0
.Ltmp63:
	.loc	4 91 22 is_stmt 1               // dpu/dpu.c:91:22
	lw r0, r22, -80
	move r1, r14
.Ltmp64:
	move r2, r18
	call r23, memset
.Ltmp65:
	//DEBUG_VALUE: chunk_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: k_chunk_addr_B <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: col <- [DW_OP_plus_uconst 1, DW_OP_stack_value] undef
	.loc	4 97 7                          // dpu/dpu.c:97:7
	lw r0, r22, -40
.Ltmp66:
	//DEBUG_VALUE: k_chunk <- 0
	jgtu r15, r0, .LBB0_18
.Ltmp67:
// %bb.11:                              //   in Loop: Header=BB0_7 Depth=2
	//DEBUG_VALUE: k_chunk_addr_B <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: chunk_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: ntile_row0_addr_B <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: row <- [DW_OP_constu 24, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_tile_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:row_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_B <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:ITERS_ROW <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_n_tiles <- [DW_OP_constu 36, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_k_chunks <- [DW_OP_constu 60, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:K <- [DW_OP_constu 40, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:matB_el_size <- [DW_OP_constu 40, DW_OP_minus, DW_OP_deref_size 4, DW_OP_constu 4, DW_OP_mul, DW_OP_stack_value] $r22
	//DEBUG_VALUE: main:cache_C <- [DW_OP_constu 80, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:write_addr_C <- $r21
	//DEBUG_VALUE: main:cache_B <- $r20
	//DEBUG_VALUE: main:cache_A <- $r19
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r15
	//DEBUG_VALUE: k_chunk <- 0
	.loc	4 0 7 is_stmt 0                 // dpu/dpu.c:0:7
	lw r21, r22, -52
.Ltmp68:
	lw r1, r22, -28
.Ltmp69:
.LBB0_12:                               //   Parent Loop BB0_5 Depth=1
                                        //     Parent Loop BB0_7 Depth=2
                                        // =>    This Loop Header: Depth=3
                                        //         Child Loop BB0_13 Depth 4
                                        //           Child Loop BB0_14 Depth 5
	//DEBUG_VALUE: ntile_row0_addr_B <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: row <- [DW_OP_constu 24, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_tile_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:row_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_B <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:ITERS_ROW <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_n_tiles <- [DW_OP_constu 36, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_k_chunks <- [DW_OP_constu 60, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:K <- [DW_OP_constu 40, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:matB_el_size <- [DW_OP_constu 40, DW_OP_minus, DW_OP_deref_size 4, DW_OP_constu 4, DW_OP_mul, DW_OP_stack_value] $r22
	//DEBUG_VALUE: main:cache_C <- [DW_OP_constu 80, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:cache_B <- $r20
	//DEBUG_VALUE: main:cache_A <- $r19
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r15
	//DEBUG_VALUE: k_chunk <- $r14
	//DEBUG_VALUE: k_chunk_addr_B <- $r21
	//DEBUG_VALUE: chunk_addr_A <- $r1
	sw r22, -72, r14
.Ltmp70:
	//DEBUG_VALUE: k_chunk <- [DW_OP_constu 72, DW_OP_minus] [$r22+0]
	move r0, -1
.Ltmp71:
	//DEBUG_VALUE: mram_read:nb_of_bytes <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: mram_read:to <- $r19
	//DEBUG_VALUE: mram_read:from <- $r1
	.loc	6 39 24 is_stmt 1               // /opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/mram.h:39:24
	lsr_add r0, r0, r18, 3
	sw r22, -88, r0
	lsl_add r0, r19, r0, 24
	move r17, 0
.Ltmp72:
	//DEBUG_VALUE: row_addr_B <- $r21
	sw r22, -64, r1
.Ltmp73:
	//DEBUG_VALUE: mram_read:from <- [DW_OP_constu 64, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: chunk_addr_A <- [DW_OP_constu 64, DW_OP_minus] [$r22+0]
	ldma r0, r1, 0
	sw r22, -84, r17
	sw r22, -68, r21
.Ltmp74:
	//DEBUG_VALUE: col <- 0
	//DEBUG_VALUE: row_addr_B <- [DW_OP_constu 68, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: k_chunk_addr_B <- [DW_OP_constu 68, DW_OP_minus] [$r22+0]
.LBB0_13:                               //   Parent Loop BB0_5 Depth=1
                                        //     Parent Loop BB0_7 Depth=2
                                        //       Parent Loop BB0_12 Depth=3
                                        // =>      This Loop Header: Depth=4
                                        //           Child Loop BB0_14 Depth 5
	//DEBUG_VALUE: k_chunk_addr_B <- [DW_OP_constu 68, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: chunk_addr_A <- [DW_OP_constu 64, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: k_chunk <- [DW_OP_constu 72, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: ntile_row0_addr_B <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: row <- [DW_OP_constu 24, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_tile_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:row_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_B <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:ITERS_ROW <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_n_tiles <- [DW_OP_constu 36, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_k_chunks <- [DW_OP_constu 60, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:K <- [DW_OP_constu 40, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:matB_el_size <- [DW_OP_constu 40, DW_OP_minus, DW_OP_deref_size 4, DW_OP_constu 4, DW_OP_mul, DW_OP_stack_value] $r22
	//DEBUG_VALUE: main:cache_C <- [DW_OP_constu 80, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:cache_B <- $r20
	//DEBUG_VALUE: main:cache_A <- $r19
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r15
	//DEBUG_VALUE: row_addr_B <- $r21
	//DEBUG_VALUE: col <- $r17
	//DEBUG_VALUE: mram_read:from <- $r21
	//DEBUG_VALUE: mram_read:to <- $r20
	//DEBUG_VALUE: mram_read:nb_of_bytes <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	.loc	6 39 24 is_stmt 0               // /opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/mram.h:39:24
	lw r0, r22, -88
	lsl_add r0, r20, r0, 24
	ldma r0, r21, 0
	lw r0, r22, -80
	lsl_add r16, r0, r17, 2
.Ltmp75:
	//DEBUG_VALUE: c <- 0
	.loc	4 109 26 is_stmt 1              // dpu/dpu.c:109:26
	lw r14, r16, 0
	lw r18, r22, -84
.Ltmp76:
.LBB0_14:                               //   Parent Loop BB0_5 Depth=1
                                        //     Parent Loop BB0_7 Depth=2
                                        //       Parent Loop BB0_12 Depth=3
                                        //         Parent Loop BB0_13 Depth=4
                                        // =>        This Inner Loop Header: Depth=5
	//DEBUG_VALUE: k_chunk_addr_B <- [DW_OP_constu 68, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: chunk_addr_A <- [DW_OP_constu 64, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: k_chunk <- [DW_OP_constu 72, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: ntile_row0_addr_B <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: row <- [DW_OP_constu 24, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_tile_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:row_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_B <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:ITERS_ROW <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_n_tiles <- [DW_OP_constu 36, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_k_chunks <- [DW_OP_constu 60, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:K <- [DW_OP_constu 40, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:matB_el_size <- [DW_OP_constu 40, DW_OP_minus, DW_OP_deref_size 4, DW_OP_constu 4, DW_OP_mul, DW_OP_stack_value] $r22
	//DEBUG_VALUE: main:cache_C <- [DW_OP_constu 80, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: row_addr_B <- $r21
	//DEBUG_VALUE: main:cache_B <- $r20
	//DEBUG_VALUE: main:cache_A <- $r19
	//DEBUG_VALUE: col <- $r17
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r15
	//DEBUG_VALUE: c <- $r18
	.loc	4 109 29 is_stmt 0              // dpu/dpu.c:109:29
	lsl_add r0, r19, r18, 2
	lw r1, r0, 0
	.loc	4 109 42                        // dpu/dpu.c:109:42
	lsl_add r0, r20, r18, 2
	lw r0, r0, 0
	.loc	4 109 40                        // dpu/dpu.c:109:40
	call r23, __mulsi3
.Ltmp77:
	.loc	4 109 26                        // dpu/dpu.c:109:26
	add r14, r14, r0
.Ltmp78:
	.loc	4 108 51 is_stmt 1              // dpu/dpu.c:108:51
	add r18, r18, 1
.Ltmp79:
	//DEBUG_VALUE: c <- $r18
	.loc	4 109 26                        // dpu/dpu.c:109:26
	sw r16, 0, r14
.Ltmp80:
	.loc	4 108 11                        // dpu/dpu.c:108:11
	jneq r15, r18, .LBB0_14
.Ltmp81:
// %bb.15:                              //   in Loop: Header=BB0_13 Depth=4
	//DEBUG_VALUE: k_chunk_addr_B <- [DW_OP_constu 68, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: chunk_addr_A <- [DW_OP_constu 64, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: k_chunk <- [DW_OP_constu 72, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: ntile_row0_addr_B <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: row <- [DW_OP_constu 24, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_tile_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:row_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_B <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:ITERS_ROW <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_n_tiles <- [DW_OP_constu 36, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_k_chunks <- [DW_OP_constu 60, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:K <- [DW_OP_constu 40, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:matB_el_size <- [DW_OP_constu 40, DW_OP_minus, DW_OP_deref_size 4, DW_OP_constu 4, DW_OP_mul, DW_OP_stack_value] $r22
	//DEBUG_VALUE: main:cache_C <- [DW_OP_constu 80, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: row_addr_B <- $r21
	//DEBUG_VALUE: main:cache_B <- $r20
	//DEBUG_VALUE: main:cache_A <- $r19
	//DEBUG_VALUE: col <- $r17
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r15
	.loc	4 105 55                        // dpu/dpu.c:105:55
	add r17, r17, 1
.Ltmp82:
	//DEBUG_VALUE: col <- $r17
	.loc	4 111 22                        // dpu/dpu.c:111:22
	lw r0, r22, -76
	add r21, r21, r0
.Ltmp83:
	//DEBUG_VALUE: row_addr_B <- $r21
	.loc	4 105 9                         // dpu/dpu.c:105:9
	jneq r17, r15, .LBB0_13
.Ltmp84:
// %bb.16:                              //   in Loop: Header=BB0_12 Depth=3
	//DEBUG_VALUE: k_chunk_addr_B <- [DW_OP_constu 68, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: chunk_addr_A <- [DW_OP_constu 64, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: k_chunk <- [DW_OP_constu 72, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: ntile_row0_addr_B <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: row <- [DW_OP_constu 24, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_tile_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:row_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_B <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:ITERS_ROW <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_n_tiles <- [DW_OP_constu 36, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_k_chunks <- [DW_OP_constu 60, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:K <- [DW_OP_constu 40, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:matB_el_size <- [DW_OP_constu 40, DW_OP_minus, DW_OP_deref_size 4, DW_OP_constu 4, DW_OP_mul, DW_OP_stack_value] $r22
	//DEBUG_VALUE: main:cache_C <- [DW_OP_constu 80, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: row_addr_B <- $r21
	//DEBUG_VALUE: main:cache_B <- $r20
	//DEBUG_VALUE: main:cache_A <- $r19
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r15
	.loc	4 0 9 is_stmt 0                 // dpu/dpu.c:0:9
	lw r18, r22, -56
	lw r1, r22, -64
	.loc	4 114 22 is_stmt 1              // dpu/dpu.c:114:22
	add r1, r1, r18
.Ltmp85:
	//DEBUG_VALUE: chunk_addr_A <- $r1
	.loc	4 0 22 is_stmt 0                // dpu/dpu.c:0:22
	lw r14, r22, -72
.Ltmp86:
	.loc	4 97 65 is_stmt 1               // dpu/dpu.c:97:65
	add r14, r14, 1
.Ltmp87:
	//DEBUG_VALUE: k_chunk <- $r14
	.loc	4 0 65 is_stmt 0                // dpu/dpu.c:0:65
	lw r21, r22, -68
.Ltmp88:
	.loc	4 115 24 is_stmt 1              // dpu/dpu.c:115:24
	add r21, r21, r18
.Ltmp89:
	//DEBUG_VALUE: k_chunk_addr_B <- $r21
	.loc	4 97 7                          // dpu/dpu.c:97:7
	lw r0, r22, -60
	jltu r14, r0, .LBB0_12
	jump .LBB0_18
.Ltmp90:
.LBB0_8:                                //   in Loop: Header=BB0_7 Depth=2
	//DEBUG_VALUE: ntile_row0_addr_B <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: row <- [DW_OP_constu 24, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_tile_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:row_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_B <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:ITERS_ROW <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_n_tiles <- [DW_OP_constu 36, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_k_chunks <- [DW_OP_constu 60, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:K <- [DW_OP_constu 40, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:matB_el_size <- [DW_OP_constu 40, DW_OP_minus, DW_OP_deref_size 4, DW_OP_constu 4, DW_OP_mul, DW_OP_stack_value] $r22
	//DEBUG_VALUE: main:cache_C <- [DW_OP_constu 80, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:write_addr_C <- $r21
	//DEBUG_VALUE: main:cache_B <- $r20
	//DEBUG_VALUE: main:cache_A <- $r19
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r15
	//DEBUG_VALUE: n_tile <- $r1
	//DEBUG_VALUE: k_chunk <- 0
	//DEBUG_VALUE: k_chunk_addr_B <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: chunk_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	lw r0, r22, -40
	jgtu r15, r0, .LBB0_18
.Ltmp91:
// %bb.9:                               //   in Loop: Header=BB0_7 Depth=2
	//DEBUG_VALUE: k_chunk_addr_B <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: chunk_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: ntile_row0_addr_B <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: row <- [DW_OP_constu 24, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_tile_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:row_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_B <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:ITERS_ROW <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_n_tiles <- [DW_OP_constu 36, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_k_chunks <- [DW_OP_constu 60, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:K <- [DW_OP_constu 40, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:matB_el_size <- [DW_OP_constu 40, DW_OP_minus, DW_OP_deref_size 4, DW_OP_constu 4, DW_OP_mul, DW_OP_stack_value] $r22
	//DEBUG_VALUE: main:cache_C <- [DW_OP_constu 80, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:write_addr_C <- $r21
	//DEBUG_VALUE: main:cache_B <- $r20
	//DEBUG_VALUE: main:cache_A <- $r19
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r15
	//DEBUG_VALUE: n_tile <- $r1
	//DEBUG_VALUE: k_chunk <- 0
	.loc	4 0 7 is_stmt 0                 // dpu/dpu.c:0:7
	move r0, 0
	lw r1, r22, -28
.Ltmp92:
.LBB0_10:                               //   Parent Loop BB0_5 Depth=1
                                        //     Parent Loop BB0_7 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	//DEBUG_VALUE: ntile_row0_addr_B <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: row <- [DW_OP_constu 24, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_tile_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:row_addr_A <- [DW_OP_constu 28, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:mram_base_addr_B <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:ITERS_ROW <- [DW_OP_constu 16, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_n_tiles <- [DW_OP_constu 36, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:num_k_chunks <- [DW_OP_constu 60, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:K <- [DW_OP_constu 40, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:matB_el_size <- [DW_OP_constu 40, DW_OP_minus, DW_OP_deref_size 4, DW_OP_constu 4, DW_OP_mul, DW_OP_stack_value] $r22
	//DEBUG_VALUE: main:cache_C <- [DW_OP_constu 80, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:write_addr_C <- $r21
	//DEBUG_VALUE: main:cache_B <- $r20
	//DEBUG_VALUE: main:cache_A <- $r19
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r15
	//DEBUG_VALUE: k_chunk_addr_B <- undef
	move r2, -1
.Ltmp93:
	//DEBUG_VALUE: mram_read:nb_of_bytes <- $r18
	//DEBUG_VALUE: mram_read:to <- $r19
	//DEBUG_VALUE: mram_read:from <- $r1
	//DEBUG_VALUE: chunk_addr_A <- $r1
	//DEBUG_VALUE: k_chunk <- $r0
	.loc	6 39 24 is_stmt 1               // /opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/mram.h:39:24
	lsr_add r2, r2, r18, 3
	lsl_add r2, r19, r2, 24
	ldma r2, r1, 0
.Ltmp94:
	//DEBUG_VALUE: col <- 0
	//DEBUG_VALUE: row_addr_B <- undef
	.loc	4 97 65                         // dpu/dpu.c:97:65
	add r0, r0, 1
.Ltmp95:
	//DEBUG_VALUE: k_chunk <- $r0
	.loc	4 114 22                        // dpu/dpu.c:114:22
	add r1, r1, r18
.Ltmp96:
	//DEBUG_VALUE: chunk_addr_A <- $r1
	.loc	4 97 7                          // dpu/dpu.c:97:7
	lw r2, r22, -60
	jltu r0, r2, .LBB0_10
	jump .LBB0_18
.Ltmp97:
.LBB0_20:
	.loc	4 0 7 is_stmt 0                 // dpu/dpu.c:0:7
	move r0, 0
	.loc	4 134 1 is_stmt 1               // dpu/dpu.c:134:1
	ld d20, r22, -120
	ld d18, r22, -112
	ld d16, r22, -104
	ld d14, r22, -96
	ld d22, r22, -8
	jump r23
.Ltmp98:
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
	.file	7 "/opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib" "sysdef.h"
	.section	.stack_sizes,"o",@progbits,.text.main
	.long	.Lfunc_begin0
	.byte	120
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
	.long	4                               // 0x4
	.size	MAP_THREAD_DIMS, 4

	.type	MAP_ITER_DIMS,@object           // @MAP_ITER_DIMS
	.section	.data.MAP_ITER_DIMS,"aw",@progbits
	.globl	MAP_ITER_DIMS
	.p2align	2
MAP_ITER_DIMS:
	.long	512                             // 0x200
	.long	512                             // 0x200
	.long	512                             // 0x200
	.size	MAP_ITER_DIMS, 12

	.type	MAP_BUFFER_SIZES,@object        // @MAP_BUFFER_SIZES
	.section	.data.MAP_BUFFER_SIZES,"aw",@progbits
	.globl	MAP_BUFFER_SIZES
	.p2align	2
MAP_BUFFER_SIZES:
	.long	8                               // 0x8
	.size	MAP_BUFFER_SIZES, 4

	.type	DPU_INPUT_ARGUMENTS,@object     // @DPU_INPUT_ARGUMENTS
	.section	.dpu_host,"aw",@progbits
	.globl	DPU_INPUT_ARGUMENTS
	.p2align	3
DPU_INPUT_ARGUMENTS:
	.zero	20
	.size	DPU_INPUT_ARGUMENTS, 20

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
	.byte	94                              // DW_OP_reg14
	.long	0
	.long	0
.Ldebug_loc1:
	.long	.Ltmp5-.Lfunc_begin0
	.long	.Ltmp21-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	96                              // DW_OP_reg16
	.long	0
	.long	0
.Ldebug_loc2:
	.long	.Ltmp7-.Lfunc_begin0
	.long	.Ltmp30-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	97                              // DW_OP_reg17
	.long	.Ltmp30-.Lfunc_begin0
	.long	.Ltmp97-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	112                             // -16
	.long	0
	.long	0
.Ldebug_loc3:
	.long	.Ltmp8-.Lfunc_begin0
	.long	.Ltmp97-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	95                              // DW_OP_reg15
	.long	0
	.long	0
.Ldebug_loc4:
	.long	.Ltmp9-.Lfunc_begin0
	.long	.Ltmp24-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	101                             // DW_OP_reg21
	.long	.Ltmp24-.Lfunc_begin0
	.long	.Ltmp97-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	88                              // -40
	.long	0
	.long	0
.Ldebug_loc5:
	.long	.Ltmp10-.Lfunc_begin0
	.long	.Ltmp11-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	80                              // DW_OP_reg0
	.long	.Ltmp11-.Lfunc_begin0
	.long	.Ltmp27-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	108                             // -20
	.long	.Ltmp27-.Lfunc_begin0
	.long	.Ltmp34-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	80                              // DW_OP_reg0
	.long	0
	.long	0
.Ldebug_loc6:
	.long	.Ltmp12-.Lfunc_begin0
	.long	.Ltmp42-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	98                              // DW_OP_reg18
	.long	0
	.long	0
.Ldebug_loc7:
	.long	.Ltmp14-.Lfunc_begin0
	.long	.Ltmp97-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	99                              // DW_OP_reg19
	.long	0
	.long	0
.Ldebug_loc8:
	.long	.Ltmp16-.Lfunc_begin0
	.long	.Ltmp97-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	100                             // DW_OP_reg20
	.long	0
	.long	0
.Ldebug_loc9:
	.long	.Ltmp18-.Lfunc_begin0
	.long	.Ltmp97-.Lfunc_begin0
	.short	3                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	176                             // -80
	.byte	127                             // 
	.long	0
	.long	0
.Ldebug_loc10:
	.long	.Ltmp33-.Lfunc_begin0
	.long	.Ltmp36-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	101                             // DW_OP_reg21
	.long	.Ltmp36-.Lfunc_begin0
	.long	.Ltmp97-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	116                             // -12
	.long	0
	.long	0
.Ldebug_loc11:
	.long	.Ltmp32-.Lfunc_begin0
	.long	.Ltmp42-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	97                              // DW_OP_reg17
	.long	0
	.long	0
.Ldebug_loc12:
	.long	.Ltmp21-.Lfunc_begin0
	.long	.Ltmp24-.Lfunc_begin0
	.short	5                               // Loc expr size
	.byte	133                             // DW_OP_breg21
	.byte	0                               // 0
	.byte	52                              // DW_OP_lit4
	.byte	30                              // DW_OP_mul
	.byte	159                             // DW_OP_stack_value
	.long	.Ltmp24-.Lfunc_begin0
	.long	.Ltmp97-.Lfunc_begin0
	.short	7                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	88                              // -40
	.byte	148                             // DW_OP_deref_size
	.byte	4                               // 
	.byte	52                              // DW_OP_lit4
	.byte	30                              // DW_OP_mul
	.byte	159                             // DW_OP_stack_value
	.long	0
	.long	0
.Ldebug_loc13:
	.long	.Ltmp37-.Lfunc_begin0
	.long	.Ltmp42-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	101                             // DW_OP_reg21
	.long	0
	.long	0
.Ldebug_loc14:
	.long	.Ltmp39-.Lfunc_begin0
	.long	.Ltmp40-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	80                              // DW_OP_reg0
	.long	.Ltmp40-.Lfunc_begin0
	.long	.Ltmp97-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	100                             // -28
	.long	0
	.long	0
.Ldebug_loc15:
	.long	.Ltmp26-.Lfunc_begin0
	.long	.Ltmp97-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	68                              // -60
	.long	0
	.long	0
.Ldebug_loc16:
	.long	.Ltmp37-.Lfunc_begin0
	.long	.Ltmp50-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	101                             // DW_OP_reg21
	.long	.Ltmp56-.Lfunc_begin0
	.long	.Ltmp68-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	101                             // DW_OP_reg21
	.long	.Ltmp90-.Lfunc_begin0
	.long	.Ltmp97-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	101                             // DW_OP_reg21
	.long	0
	.long	0
.Ldebug_loc17:
	.long	.Ltmp39-.Lfunc_begin0
	.long	.Ltmp40-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	80                              // DW_OP_reg0
	.long	.Ltmp40-.Lfunc_begin0
	.long	.Ltmp45-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	100                             // -28
	.long	.Ltmp45-.Lfunc_begin0
	.long	.Ltmp46-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	.Ltmp46-.Lfunc_begin0
	.long	.Ltmp97-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	100                             // -28
	.long	0
	.long	0
.Ldebug_loc18:
	.long	.Ltmp29-.Lfunc_begin0
	.long	.Ltmp97-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	92                              // -36
	.long	0
	.long	0
.Ldebug_loc19:
	.long	.Ltmp30-.Lfunc_begin0
	.long	.Ltmp31-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	48                              // DW_OP_lit0
	.byte	159                             // DW_OP_stack_value
	.long	.Ltmp42-.Lfunc_begin0
	.long	.Ltmp44-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	104                             // -24
	.long	.Ltmp44-.Lfunc_begin0
	.long	.Ltmp47-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	82                              // DW_OP_reg2
	.long	.Ltmp47-.Lfunc_begin0
	.long	.Ltmp97-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	104                             // -24
	.long	0
	.long	0
.Ldebug_loc20:
	.long	.Ltmp48-.Lfunc_begin0
	.long	.Ltmp50-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	48                              // DW_OP_lit0
	.byte	159                             // DW_OP_stack_value
	.long	.Ltmp58-.Lfunc_begin0
	.long	.Ltmp64-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	.Ltmp90-.Lfunc_begin0
	.long	.Ltmp92-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	0
	.long	0
.Ldebug_loc21:
	.long	.Ltmp48-.Lfunc_begin0
	.long	.Ltmp50-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	116                             // -12
	.long	.Ltmp50-.Lfunc_begin0
	.long	.Ltmp59-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	76                              // -52
	.long	.Ltmp59-.Lfunc_begin0
	.long	.Ltmp60-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	82                              // DW_OP_reg2
	.long	.Ltmp60-.Lfunc_begin0
	.long	.Ltmp97-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	76                              // -52
	.long	0
	.long	0
.Ldebug_loc22:
	.long	.Ltmp51-.Lfunc_begin0
	.long	.Ltmp52-.Lfunc_begin0
	.short	3                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	176                             // -80
	.byte	127                             // 
	.long	.Ltmp52-.Lfunc_begin0
	.long	.Ltmp53-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	.Ltmp53-.Lfunc_begin0
	.long	.Ltmp61-.Lfunc_begin0
	.short	3                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	176                             // -80
	.byte	127                             // 
	.long	0
	.long	0
.Ldebug_loc23:
	.long	.Ltmp51-.Lfunc_begin0
	.long	.Ltmp54-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	80                              // -48
	.long	.Ltmp54-.Lfunc_begin0
	.long	.Ltmp56-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	101                             // DW_OP_reg21
	.long	0
	.long	0
.Ldebug_loc24:
	.long	.Ltmp61-.Lfunc_begin0
	.long	.Ltmp65-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	48                              // DW_OP_lit0
	.byte	159                             // DW_OP_stack_value
	.long	0
	.long	0
.Ldebug_loc25:
	.long	.Ltmp65-.Lfunc_begin0
	.long	.Ltmp69-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	100                             // -28
	.long	.Ltmp69-.Lfunc_begin0
	.long	.Ltmp73-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	.Ltmp73-.Lfunc_begin0
	.long	.Ltmp85-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	64                              // -64
	.long	.Ltmp85-.Lfunc_begin0
	.long	.Ltmp90-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	.Ltmp90-.Lfunc_begin0
	.long	.Ltmp92-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	100                             // -28
	.long	.Ltmp93-.Lfunc_begin0
	.long	.Ltmp97-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	0
	.long	0
.Ldebug_loc26:
	.long	.Ltmp65-.Lfunc_begin0
	.long	.Ltmp69-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	76                              // -52
	.long	.Ltmp69-.Lfunc_begin0
	.long	.Ltmp74-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	101                             // DW_OP_reg21
	.long	.Ltmp74-.Lfunc_begin0
	.long	.Ltmp89-.Lfunc_begin0
	.short	3                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	188                             // -68
	.byte	127                             // 
	.long	.Ltmp89-.Lfunc_begin0
	.long	.Ltmp90-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	101                             // DW_OP_reg21
	.long	.Ltmp90-.Lfunc_begin0
	.long	.Ltmp92-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	76                              // -52
	.long	0
	.long	0
.Ldebug_loc27:
	.long	.Ltmp66-.Lfunc_begin0
	.long	.Ltmp69-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	48                              // DW_OP_lit0
	.byte	159                             // DW_OP_stack_value
	.long	.Ltmp69-.Lfunc_begin0
	.long	.Ltmp70-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	94                              // DW_OP_reg14
	.long	.Ltmp70-.Lfunc_begin0
	.long	.Ltmp87-.Lfunc_begin0
	.short	3                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	184                             // -72
	.byte	127                             // 
	.long	.Ltmp87-.Lfunc_begin0
	.long	.Ltmp90-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	94                              // DW_OP_reg14
	.long	.Ltmp90-.Lfunc_begin0
	.long	.Ltmp92-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	48                              // DW_OP_lit0
	.byte	159                             // DW_OP_stack_value
	.long	.Ltmp93-.Lfunc_begin0
	.long	.Ltmp97-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	80                              // DW_OP_reg0
	.long	0
	.long	0
.Ldebug_loc28:
	.long	.Ltmp71-.Lfunc_begin0
	.long	.Ltmp74-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	72                              // -56
	.long	.Ltmp93-.Lfunc_begin0
	.long	.Ltmp97-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	98                              // DW_OP_reg18
	.long	0
	.long	0
.Ldebug_loc29:
	.long	.Ltmp71-.Lfunc_begin0
	.long	.Ltmp74-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	99                              // DW_OP_reg19
	.long	.Ltmp93-.Lfunc_begin0
	.long	.Ltmp97-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	99                              // DW_OP_reg19
	.long	0
	.long	0
.Ldebug_loc30:
	.long	.Ltmp71-.Lfunc_begin0
	.long	.Ltmp73-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	.Ltmp73-.Lfunc_begin0
	.long	.Ltmp74-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	64                              // -64
	.long	.Ltmp93-.Lfunc_begin0
	.long	.Ltmp96-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	0
	.long	0
.Ldebug_loc31:
	.long	.Ltmp72-.Lfunc_begin0
	.long	.Ltmp88-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	101                             // DW_OP_reg21
	.long	0
	.long	0
.Ldebug_loc32:
	.long	.Ltmp75-.Lfunc_begin0
	.long	.Ltmp76-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	48                              // DW_OP_lit0
	.byte	159                             // DW_OP_stack_value
	.long	.Ltmp76-.Lfunc_begin0
	.long	.Ltmp81-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	98                              // DW_OP_reg18
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
	.byte	11                              // DW_TAG_lexical_block
	.byte	1                               // DW_CHILDREN_yes
	.byte	17                              // DW_AT_low_pc
	.byte	1                               // DW_FORM_addr
	.byte	18                              // DW_AT_high_pc
	.byte	6                               // DW_FORM_data4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	23                              // Abbreviation Code
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
	.byte	24                              // Abbreviation Code
	.byte	5                               // DW_TAG_formal_parameter
	.byte	0                               // DW_CHILDREN_no
	.byte	2                               // DW_AT_location
	.byte	23                              // DW_FORM_sec_offset
	.byte	49                              // DW_AT_abstract_origin
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	25                              // Abbreviation Code
	.byte	5                               // DW_TAG_formal_parameter
	.byte	0                               // DW_CHILDREN_no
	.byte	2                               // DW_AT_location
	.byte	24                              // DW_FORM_exprloc
	.byte	49                              // DW_AT_abstract_origin
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	26                              // Abbreviation Code
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
	.byte	27                              // Abbreviation Code
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
	.byte	28                              // Abbreviation Code
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
	.byte	1                               // Abbrev [1] 0xb:0x4bf DW_TAG_compile_unit
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
	.byte	3                               // DW_AT_count
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
	.byte	20                              // DW_AT_byte_size
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
	.byte	55                              // DW_AT_decl_line
                                        // DW_AT_prototyped
	.byte	1                               // DW_AT_inline
	.byte	16                              // Abbrev [16] 0x152:0xb DW_TAG_formal_parameter
	.long	.Linfo_string23                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	55                              // DW_AT_decl_line
	.long	301                             // DW_AT_type
	.byte	16                              // Abbrev [16] 0x15d:0xb DW_TAG_formal_parameter
	.long	.Linfo_string24                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	55                              // DW_AT_decl_line
	.long	300                             // DW_AT_type
	.byte	16                              // Abbrev [16] 0x168:0xb DW_TAG_formal_parameter
	.long	.Linfo_string25                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	55                              // DW_AT_decl_line
	.long	78                              // DW_AT_type
	.byte	0                               // End Of Children Mark
	.byte	15                              // Abbrev [15] 0x174:0x2a DW_TAG_subprogram
	.long	.Linfo_string26                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	33                              // DW_AT_decl_line
                                        // DW_AT_prototyped
	.byte	1                               // DW_AT_inline
	.byte	16                              // Abbrev [16] 0x17c:0xb DW_TAG_formal_parameter
	.long	.Linfo_string23                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	33                              // DW_AT_decl_line
	.long	301                             // DW_AT_type
	.byte	16                              // Abbrev [16] 0x187:0xb DW_TAG_formal_parameter
	.long	.Linfo_string24                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	33                              // DW_AT_decl_line
	.long	300                             // DW_AT_type
	.byte	16                              // Abbrev [16] 0x192:0xb DW_TAG_formal_parameter
	.long	.Linfo_string25                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	33                              // DW_AT_decl_line
	.long	78                              // DW_AT_type
	.byte	0                               // End Of Children Mark
	.byte	17                              // Abbrev [17] 0x19e:0x324 DW_TAG_subprogram
	.long	.Lfunc_begin0                   // DW_AT_low_pc
	.long	.Lfunc_end0-.Lfunc_begin0       // DW_AT_high_pc
	.byte	1                               // DW_AT_frame_base
	.byte	102
                                        // DW_AT_GNU_all_call_sites
	.long	.Linfo_string27                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	22                              // DW_AT_decl_line
	.long	1218                            // DW_AT_type
                                        // DW_AT_external
	.byte	18                              // Abbrev [18] 0x1b3:0xf DW_TAG_variable
	.long	.Ldebug_loc0                    // DW_AT_location
	.long	.Linfo_string29                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	23                              // DW_AT_decl_line
	.long	78                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x1c2:0xf DW_TAG_variable
	.long	.Ldebug_loc1                    // DW_AT_location
	.long	.Linfo_string13                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	41                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x1d1:0xf DW_TAG_variable
	.long	.Ldebug_loc2                    // DW_AT_location
	.long	.Linfo_string30                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	45                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x1e0:0xf DW_TAG_variable
	.long	.Ldebug_loc3                    // DW_AT_location
	.long	.Linfo_string31                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	49                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x1ef:0xf DW_TAG_variable
	.long	.Ldebug_loc4                    // DW_AT_location
	.long	.Linfo_string32                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	46                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x1fe:0xf DW_TAG_variable
	.long	.Ldebug_loc5                    // DW_AT_location
	.long	.Linfo_string33                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	47                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	19                              // Abbrev [19] 0x20d:0xb DW_TAG_variable
	.long	.Linfo_string34                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	52                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x218:0xf DW_TAG_variable
	.long	.Ldebug_loc6                    // DW_AT_location
	.long	.Linfo_string35                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	50                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x227:0xf DW_TAG_variable
	.long	.Ldebug_loc7                    // DW_AT_location
	.long	.Linfo_string36                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	54                              // DW_AT_decl_line
	.long	295                             // DW_AT_type
	.byte	18                              // Abbrev [18] 0x236:0xf DW_TAG_variable
	.long	.Ldebug_loc8                    // DW_AT_location
	.long	.Linfo_string37                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	55                              // DW_AT_decl_line
	.long	295                             // DW_AT_type
	.byte	18                              // Abbrev [18] 0x245:0xf DW_TAG_variable
	.long	.Ldebug_loc9                    // DW_AT_location
	.long	.Linfo_string38                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	56                              // DW_AT_decl_line
	.long	295                             // DW_AT_type
	.byte	19                              // Abbrev [19] 0x254:0xb DW_TAG_variable
	.long	.Linfo_string39                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	68                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x25f:0xf DW_TAG_variable
	.long	.Ldebug_loc10                   // DW_AT_location
	.long	.Linfo_string40                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	66                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x26e:0xf DW_TAG_variable
	.long	.Ldebug_loc11                   // DW_AT_location
	.long	.Linfo_string41                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	65                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	19                              // Abbrev [19] 0x27d:0xb DW_TAG_variable
	.long	.Linfo_string42                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	58                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x288:0xf DW_TAG_variable
	.long	.Ldebug_loc12                   // DW_AT_location
	.long	.Linfo_string43                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	63                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x297:0xf DW_TAG_variable
	.long	.Ldebug_loc13                   // DW_AT_location
	.long	.Linfo_string44                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	73                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x2a6:0xf DW_TAG_variable
	.long	.Ldebug_loc14                   // DW_AT_location
	.long	.Linfo_string45                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	71                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x2b5:0xf DW_TAG_variable
	.long	.Ldebug_loc15                   // DW_AT_location
	.long	.Linfo_string46                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	80                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x2c4:0xf DW_TAG_variable
	.long	.Ldebug_loc16                   // DW_AT_location
	.long	.Linfo_string47                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	84                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x2d3:0xf DW_TAG_variable
	.long	.Ldebug_loc17                   // DW_AT_location
	.long	.Linfo_string48                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	83                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x2e2:0xf DW_TAG_variable
	.long	.Ldebug_loc18                   // DW_AT_location
	.long	.Linfo_string49                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	81                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	20                              // Abbrev [20] 0x2f1:0x10 DW_TAG_inlined_subroutine
	.long	307                             // DW_AT_abstract_origin
	.long	.Ltmp0                          // DW_AT_low_pc
	.long	.Ltmp2-.Ltmp0                   // DW_AT_high_pc
	.byte	4                               // DW_AT_call_file
	.byte	23                              // DW_AT_call_line
	.byte	29                              // DW_AT_call_column
	.byte	21                              // Abbrev [21] 0x301:0x157 DW_TAG_lexical_block
	.long	.Ldebug_ranges4                 // DW_AT_ranges
	.byte	18                              // Abbrev [18] 0x306:0xf DW_TAG_variable
	.long	.Ldebug_loc19                   // DW_AT_location
	.long	.Linfo_string50                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	86                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	21                              // Abbrev [21] 0x315:0x142 DW_TAG_lexical_block
	.long	.Ldebug_ranges3                 // DW_AT_ranges
	.byte	18                              // Abbrev [18] 0x31a:0xf DW_TAG_variable
	.long	.Ldebug_loc21                   // DW_AT_location
	.long	.Linfo_string52                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	87                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	22                              // Abbrev [22] 0x329:0x12d DW_TAG_lexical_block
	.long	.Ltmp48                         // DW_AT_low_pc
	.long	.Ltmp97-.Ltmp48                 // DW_AT_high_pc
	.byte	18                              // Abbrev [18] 0x332:0xf DW_TAG_variable
	.long	.Ldebug_loc20                   // DW_AT_location
	.long	.Linfo_string51                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	89                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	21                              // Abbrev [21] 0x341:0x114 DW_TAG_lexical_block
	.long	.Ldebug_ranges2                 // DW_AT_ranges
	.byte	18                              // Abbrev [18] 0x346:0xf DW_TAG_variable
	.long	.Ldebug_loc25                   // DW_AT_location
	.long	.Linfo_string54                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	94                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x355:0xf DW_TAG_variable
	.long	.Ldebug_loc26                   // DW_AT_location
	.long	.Linfo_string55                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	95                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	23                              // Abbrev [23] 0x364:0x2a DW_TAG_inlined_subroutine
	.long	330                             // DW_AT_abstract_origin
	.long	.Ltmp51                         // DW_AT_low_pc
	.long	.Ltmp55-.Ltmp51                 // DW_AT_high_pc
	.byte	4                               // DW_AT_call_file
	.byte	118                             // DW_AT_call_line
	.byte	7                               // DW_AT_call_column
	.byte	24                              // Abbrev [24] 0x374:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc22                   // DW_AT_location
	.long	338                             // DW_AT_abstract_origin
	.byte	24                              // Abbrev [24] 0x37d:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc23                   // DW_AT_location
	.long	349                             // DW_AT_abstract_origin
	.byte	25                              // Abbrev [25] 0x386:0x7 DW_TAG_formal_parameter
	.byte	1                               // DW_AT_location
	.byte	98
	.long	360                             // DW_AT_abstract_origin
	.byte	0                               // End Of Children Mark
	.byte	22                              // Abbrev [22] 0x38e:0x19 DW_TAG_lexical_block
	.long	.Ltmp61                         // DW_AT_low_pc
	.long	.Ltmp65-.Ltmp61                 // DW_AT_high_pc
	.byte	18                              // Abbrev [18] 0x397:0xf DW_TAG_variable
	.long	.Ldebug_loc24                   // DW_AT_location
	.long	.Linfo_string53                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	90                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	0                               // End Of Children Mark
	.byte	22                              // Abbrev [22] 0x3a7:0xad DW_TAG_lexical_block
	.long	.Ltmp65                         // DW_AT_low_pc
	.long	.Ltmp97-.Ltmp65                 // DW_AT_high_pc
	.byte	18                              // Abbrev [18] 0x3b0:0xf DW_TAG_variable
	.long	.Ldebug_loc27                   // DW_AT_location
	.long	.Linfo_string56                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	97                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	21                              // Abbrev [21] 0x3bf:0x94 DW_TAG_lexical_block
	.long	.Ldebug_ranges1                 // DW_AT_ranges
	.byte	18                              // Abbrev [18] 0x3c4:0xf DW_TAG_variable
	.long	.Ldebug_loc31                   // DW_AT_location
	.long	.Linfo_string57                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	104                             // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	26                              // Abbrev [26] 0x3d3:0x28 DW_TAG_inlined_subroutine
	.long	372                             // DW_AT_abstract_origin
	.long	.Ldebug_ranges0                 // DW_AT_ranges
	.byte	4                               // DW_AT_call_file
	.byte	101                             // DW_AT_call_line
	.byte	9                               // DW_AT_call_column
	.byte	24                              // Abbrev [24] 0x3df:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc30                   // DW_AT_location
	.long	380                             // DW_AT_abstract_origin
	.byte	24                              // Abbrev [24] 0x3e8:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc29                   // DW_AT_location
	.long	391                             // DW_AT_abstract_origin
	.byte	24                              // Abbrev [24] 0x3f1:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc28                   // DW_AT_location
	.long	402                             // DW_AT_abstract_origin
	.byte	0                               // End Of Children Mark
	.byte	22                              // Abbrev [22] 0x3fb:0x57 DW_TAG_lexical_block
	.long	.Ltmp74                         // DW_AT_low_pc
	.long	.Ltmp84-.Ltmp74                 // DW_AT_high_pc
	.byte	27                              // Abbrev [27] 0x404:0xd DW_TAG_variable
	.byte	1                               // DW_AT_location
	.byte	97
	.long	.Linfo_string53                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	105                             // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	23                              // Abbrev [23] 0x411:0x27 DW_TAG_inlined_subroutine
	.long	372                             // DW_AT_abstract_origin
	.long	.Ltmp74                         // DW_AT_low_pc
	.long	.Ltmp75-.Ltmp74                 // DW_AT_high_pc
	.byte	4                               // DW_AT_call_file
	.byte	106                             // DW_AT_call_line
	.byte	11                              // DW_AT_call_column
	.byte	25                              // Abbrev [25] 0x421:0x7 DW_TAG_formal_parameter
	.byte	1                               // DW_AT_location
	.byte	101
	.long	380                             // DW_AT_abstract_origin
	.byte	25                              // Abbrev [25] 0x428:0x7 DW_TAG_formal_parameter
	.byte	1                               // DW_AT_location
	.byte	100
	.long	391                             // DW_AT_abstract_origin
	.byte	25                              // Abbrev [25] 0x42f:0x8 DW_TAG_formal_parameter
	.byte	2                               // DW_AT_location
	.byte	145
	.byte	72
	.long	402                             // DW_AT_abstract_origin
	.byte	0                               // End Of Children Mark
	.byte	22                              // Abbrev [22] 0x438:0x19 DW_TAG_lexical_block
	.long	.Ltmp75                         // DW_AT_low_pc
	.long	.Ltmp81-.Ltmp75                 // DW_AT_high_pc
	.byte	18                              // Abbrev [18] 0x441:0xf DW_TAG_variable
	.long	.Ldebug_loc32                   // DW_AT_location
	.long	.Linfo_string58                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	108                             // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	0                               // End Of Children Mark
	.byte	0                               // End Of Children Mark
	.byte	0                               // End Of Children Mark
	.byte	0                               // End Of Children Mark
	.byte	0                               // End Of Children Mark
	.byte	0                               // End Of Children Mark
	.byte	0                               // End Of Children Mark
	.byte	0                               // End Of Children Mark
	.byte	28                              // Abbrev [28] 0x458:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp3                          // DW_AT_low_pc
	.byte	28                              // Abbrev [28] 0x45f:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp4                          // DW_AT_low_pc
	.byte	28                              // Abbrev [28] 0x466:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp13                         // DW_AT_low_pc
	.byte	28                              // Abbrev [28] 0x46d:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp15                         // DW_AT_low_pc
	.byte	28                              // Abbrev [28] 0x474:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp17                         // DW_AT_low_pc
	.byte	28                              // Abbrev [28] 0x47b:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp19                         // DW_AT_low_pc
	.byte	28                              // Abbrev [28] 0x482:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp20                         // DW_AT_low_pc
	.byte	28                              // Abbrev [28] 0x489:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp22                         // DW_AT_low_pc
	.byte	28                              // Abbrev [28] 0x490:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp25                         // DW_AT_low_pc
	.byte	28                              // Abbrev [28] 0x497:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp28                         // DW_AT_low_pc
	.byte	28                              // Abbrev [28] 0x49e:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp35                         // DW_AT_low_pc
	.byte	28                              // Abbrev [28] 0x4a5:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp38                         // DW_AT_low_pc
	.byte	28                              // Abbrev [28] 0x4ac:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp41                         // DW_AT_low_pc
	.byte	28                              // Abbrev [28] 0x4b3:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp65                         // DW_AT_low_pc
	.byte	28                              // Abbrev [28] 0x4ba:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp77                         // DW_AT_low_pc
	.byte	0                               // End Of Children Mark
	.byte	6                               // Abbrev [6] 0x4c2:0x7 DW_TAG_base_type
	.long	.Linfo_string28                 // DW_AT_name
	.byte	5                               // DW_AT_encoding
	.byte	4                               // DW_AT_byte_size
	.byte	0                               // End Of Children Mark
.Ldebug_info_end0:
	.section	.debug_ranges,"",@progbits
.Ldebug_ranges0:
	.long	.Ltmp71-.Lfunc_begin0
	.long	.Ltmp74-.Lfunc_begin0
	.long	.Ltmp93-.Lfunc_begin0
	.long	.Ltmp94-.Lfunc_begin0
	.long	0
	.long	0
.Ldebug_ranges1:
	.long	.Ltmp71-.Lfunc_begin0
	.long	.Ltmp86-.Lfunc_begin0
	.long	.Ltmp88-.Lfunc_begin0
	.long	.Ltmp89-.Lfunc_begin0
	.long	.Ltmp93-.Lfunc_begin0
	.long	.Ltmp94-.Lfunc_begin0
	.long	.Ltmp95-.Lfunc_begin0
	.long	.Ltmp96-.Lfunc_begin0
	.long	0
	.long	0
.Ldebug_ranges2:
	.long	.Ltmp51-.Lfunc_begin0
	.long	.Ltmp57-.Lfunc_begin0
	.long	.Ltmp58-.Lfunc_begin0
	.long	.Ltmp59-.Lfunc_begin0
	.long	.Ltmp61-.Lfunc_begin0
	.long	.Ltmp97-.Lfunc_begin0
	.long	0
	.long	0
.Ldebug_ranges3:
	.long	.Ltmp44-.Lfunc_begin0
	.long	.Ltmp45-.Lfunc_begin0
	.long	.Ltmp48-.Lfunc_begin0
	.long	.Ltmp97-.Lfunc_begin0
	.long	0
	.long	0
.Ldebug_ranges4:
	.long	.Ltmp30-.Lfunc_begin0
	.long	.Ltmp31-.Lfunc_begin0
	.long	.Ltmp43-.Lfunc_begin0
	.long	.Ltmp97-.Lfunc_begin0
	.long	0
	.long	0
	.section	.debug_str,"MS",@progbits,1
.Linfo_string0:
	.asciz	"clang version 12.0.0 (https://github.com/upmem/llvm-project.git 846fdda8285dcc9b20ee5d2fec9e54dfea6a8928)" // string offset=0
.Linfo_string1:
	.asciz	"dpu/dpu.c"                     // string offset=106
.Linfo_string2:
	.asciz	"/home/hamid.farzaneh/CostModelEvaluationRepo/NewEval/Gemm" // string offset=116
.Linfo_string3:
	.asciz	"MAP_RANK_DIMS"                 // string offset=174
.Linfo_string4:
	.asciz	"unsigned int"                  // string offset=188
.Linfo_string5:
	.asciz	"uint32_t"                      // string offset=201
.Linfo_string6:
	.asciz	"__ARRAY_SIZE_TYPE__"           // string offset=210
.Linfo_string7:
	.asciz	"MAP_DPU_DIMS"                  // string offset=230
.Linfo_string8:
	.asciz	"MAP_THREAD_DIMS"               // string offset=243
.Linfo_string9:
	.asciz	"MAP_ITER_DIMS"                 // string offset=259
.Linfo_string10:
	.asciz	"MAP_BUFFER_SIZES"              // string offset=273
.Linfo_string11:
	.asciz	"DPU_INPUT_ARGUMENTS"           // string offset=290
.Linfo_string12:
	.asciz	"BUFFER_SIZES"                  // string offset=310
.Linfo_string13:
	.asciz	"THREADS"                       // string offset=323
.Linfo_string14:
	.asciz	"ITERS"                         // string offset=331
.Linfo_string15:
	.asciz	"dpu_arguments_t"               // string offset=337
.Linfo_string16:
	.asciz	"nb_cycle"                      // string offset=353
.Linfo_string17:
	.asciz	"__atomic_bit_barrier_my_barrier" // string offset=362
.Linfo_string18:
	.asciz	"unsigned char"                 // string offset=394
.Linfo_string19:
	.asciz	"uint8_t"                       // string offset=408
.Linfo_string20:
	.asciz	"me"                            // string offset=416
.Linfo_string21:
	.asciz	"sysname_t"                     // string offset=419
.Linfo_string22:
	.asciz	"mram_write"                    // string offset=429
.Linfo_string23:
	.asciz	"from"                          // string offset=440
.Linfo_string24:
	.asciz	"to"                            // string offset=445
.Linfo_string25:
	.asciz	"nb_of_bytes"                   // string offset=448
.Linfo_string26:
	.asciz	"mram_read"                     // string offset=460
.Linfo_string27:
	.asciz	"main"                          // string offset=470
.Linfo_string28:
	.asciz	"int"                           // string offset=475
.Linfo_string29:
	.asciz	"tasklet_id"                    // string offset=479
.Linfo_string30:
	.asciz	"ITERS_ROW"                     // string offset=490
.Linfo_string31:
	.asciz	"BUFFER_COUNT"                  // string offset=500
.Linfo_string32:
	.asciz	"K"                             // string offset=513
.Linfo_string33:
	.asciz	"N"                             // string offset=515
.Linfo_string34:
	.asciz	"total_rows"                    // string offset=517
.Linfo_string35:
	.asciz	"BUFFER_SIZE"                   // string offset=528
.Linfo_string36:
	.asciz	"cache_A"                       // string offset=540
.Linfo_string37:
	.asciz	"cache_B"                       // string offset=548
.Linfo_string38:
	.asciz	"cache_C"                       // string offset=556
.Linfo_string39:
	.asciz	"mram_base_addr_C"              // string offset=564
.Linfo_string40:
	.asciz	"mram_base_addr_B"              // string offset=581
.Linfo_string41:
	.asciz	"mram_base_addr_A"              // string offset=598
.Linfo_string42:
	.asciz	"matA_el_size"                  // string offset=615
.Linfo_string43:
	.asciz	"matB_el_size"                  // string offset=628
.Linfo_string44:
	.asciz	"mram_tile_addr_C"              // string offset=641
.Linfo_string45:
	.asciz	"mram_tile_addr_A"              // string offset=658
.Linfo_string46:
	.asciz	"num_k_chunks"                  // string offset=675
.Linfo_string47:
	.asciz	"write_addr_C"                  // string offset=688
.Linfo_string48:
	.asciz	"row_addr_A"                    // string offset=701
.Linfo_string49:
	.asciz	"num_n_tiles"                   // string offset=712
.Linfo_string50:
	.asciz	"row"                           // string offset=724
.Linfo_string51:
	.asciz	"n_tile"                        // string offset=728
.Linfo_string52:
	.asciz	"ntile_row0_addr_B"             // string offset=735
.Linfo_string53:
	.asciz	"col"                           // string offset=753
.Linfo_string54:
	.asciz	"chunk_addr_A"                  // string offset=757
.Linfo_string55:
	.asciz	"k_chunk_addr_B"                // string offset=770
.Linfo_string56:
	.asciz	"k_chunk"                       // string offset=785
.Linfo_string57:
	.asciz	"row_addr_B"                    // string offset=793
.Linfo_string58:
	.asciz	"c"                             // string offset=804
	.addrsig
	.addrsig_sym my_barrier
	.addrsig_sym DPU_INPUT_ARGUMENTS
	.addrsig_sym __sys_used_mram_end
	.addrsig_sym nb_cycle
	.section	.debug_line,"",@progbits
.Lline_table_start0:
