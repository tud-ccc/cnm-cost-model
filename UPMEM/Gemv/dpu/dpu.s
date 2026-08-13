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
	.file	1 "/opt/upmem/upmem-2025.1.0-Linux-x86_64/bin/../share/upmem/include/stdlib" "stdint.h"
	.file	2 "/home/hamid.farzaneh/CostModelEvaluationRepo/NewEval/Gemv" "dpu/../generated_headers/gen_map.h"
	.file	3 "/home/hamid.farzaneh/CostModelEvaluationRepo/NewEval/Gemv" "dpu/../generated_headers/common_structs.h"
	.file	4 "/home/hamid.farzaneh/CostModelEvaluationRepo/NewEval/Gemv" "dpu/dpu.c"
	.section	.text.main,"ax",@progbits
	.globl	main                            // -- Begin function main
	.type	main,@function
main:                                   // @main
.Lfunc_begin0:
	.loc	4 22 0                          // dpu/dpu.c:22:0
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
	//DEBUG_VALUE: main:tasklet_id <- $r17
	.file	5 "/opt/upmem/upmem-2025.1.0-Linux-x86_64/bin/../share/upmem/include/syslib" "defs.h"
	.loc	5 35 12 prologue_end            // /opt/upmem/upmem-2025.1.0-Linux-x86_64/bin/../share/upmem/include/syslib/defs.h:35:12
	move r17, id, nz, .LBB0_2
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
	.loc	4 45 27                         // dpu/dpu.c:45:27
	lw r20, zero, DPU_INPUT_ARGUMENTS
.Ltmp5:
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r20
	.loc	4 42 24                         // dpu/dpu.c:42:24
	lw r15, r0, 8
.Ltmp6:
	//DEBUG_VALUE: main:ITERS_ROW <- $r15
	.loc	4 41 22                         // dpu/dpu.c:41:22
	lw r14, r0, 4
.Ltmp7:
	//DEBUG_VALUE: main:THREADS <- $r14
	.loc	4 43 22                         // dpu/dpu.c:43:22
	lw r21, r0, 12
.Ltmp8:
	//DEBUG_VALUE: main:VEC_LEN <- $r21
	.loc	4 46 39                         // dpu/dpu.c:46:39
	lsl r18, r20, 2
.Ltmp9:
	//DEBUG_VALUE: main:total_cols <- $r21
	//DEBUG_VALUE: main:total_rows <- undef
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r18
	.loc	4 59 54                         // dpu/dpu.c:59:54
	and r0, r15, 1
	.loc	4 59 41 is_stmt 0               // dpu/dpu.c:59:41
	add r0, r0, r15
.Ltmp10:
	//DEBUG_VALUE: main:padded_iters_row <- $r0
	sw r22, -52, r0
.Ltmp11:
	//DEBUG_VALUE: main:padded_iters_row <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	.loc	4 61 21 is_stmt 1               // dpu/dpu.c:61:21
	move r0, r18
	call r23, mem_alloc
.Ltmp12:
	move r19, r0
.Ltmp13:
	//DEBUG_VALUE: main:cache_A <- $r19
	.loc	4 62 21                         // dpu/dpu.c:62:21
	move r0, r18
	call r23, mem_alloc
.Ltmp14:
	sw r22, -56, r0
.Ltmp15:
	//DEBUG_VALUE: main:cache_B <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	.loc	4 63 21                         // dpu/dpu.c:63:21
	move r0, r18
	call r23, mem_alloc
.Ltmp16:
	move r16, r0
.Ltmp17:
	//DEBUG_VALUE: main:cache_C <- $r16
	.loc	4 48 33                         // dpu/dpu.c:48:33
	move r0, r21
	move r1, r15
	call r23, __mulsi3
.Ltmp18:
	.loc	4 65 37                         // dpu/dpu.c:65:37
	lsl r0, r0, 2
	sw r22, -40, r0
	.loc	4 65 50 is_stmt 0               // dpu/dpu.c:65:50
	move r1, r14
	call r23, __mulsi3
.Ltmp19:
	move r14, r0
.Ltmp20:
	//DEBUG_VALUE: main:mram_tile_addr_A <- undef
	//DEBUG_VALUE: main:mram_base_addr_C <- undef
	//DEBUG_VALUE: main:mram_base_addr_B <- undef
	//DEBUG_VALUE: main:mram_base_addr_A <- undef
	//DEBUG_VALUE: main:vec_el_size <- undef
	//DEBUG_VALUE: main:mat_el_size <- $r14
	.loc	4 76 58 is_stmt 1               // dpu/dpu.c:76:58
	move r0, r17
	lw r1, r22, -52
	call r23, __mulsi3
.Ltmp21:
	sw r22, -52, r0
.Ltmp22:
	//DEBUG_VALUE: main:mram_tile_addr_C <- undef
	.loc	4 78 40                         // dpu/dpu.c:78:40
	move r0, r21
	move r1, r20
	call r23, __udiv32
.Ltmp23:
	move r2, r0
.Ltmp24:
	//DEBUG_VALUE: main:pending_rows <- 0
	//DEBUG_VALUE: main:write_addr_C <- undef
	//DEBUG_VALUE: main:row_addr_A <- undef
	//DEBUG_VALUE: main:num_col_chunks <- $r2
	//DEBUG_VALUE: rr <- 0
	.loc	4 90 3                          // dpu/dpu.c:90:3
	jeq r15, 0, .LBB0_10
.Ltmp25:
// %bb.3:
	//DEBUG_VALUE: main:cache_B <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:padded_iters_row <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:total_cols <- $r21
	//DEBUG_VALUE: main:VEC_LEN <- $r21
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r20
	//DEBUG_VALUE: main:cache_A <- $r19
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r18
	//DEBUG_VALUE: main:cache_C <- $r16
	//DEBUG_VALUE: main:ITERS_ROW <- $r15
	//DEBUG_VALUE: main:mat_el_size <- $r14
	//DEBUG_VALUE: main:num_col_chunks <- $r2
	//DEBUG_VALUE: rr <- 0
	//DEBUG_VALUE: main:pending_rows <- 0
	//DEBUG_VALUE: main:vec_el_size <- undef
	.loc	4 0 0 is_stmt 0                 // dpu/dpu.c:0:0
	move r0, __sys_used_mram_end
.Ltmp26:
	//DEBUG_VALUE: main:mram_base_addr_A <- $r0
	add r1, r14, r0
.Ltmp27:
	//DEBUG_VALUE: main:mram_base_addr_B <- $r1
	lw r0, r22, -52
.Ltmp28:
	lsl r0, r0, 2
	lsl_add r0, r0, r21, 2
	add r14, r0, r1
.Ltmp29:
	//DEBUG_VALUE: main:write_addr_C <- $r14
	//DEBUG_VALUE: main:row_addr_A <- undef
	//DEBUG_VALUE: main:mram_tile_addr_C <- $r14
	//DEBUG_VALUE: main:mram_tile_addr_A <- undef
	.loc	4 90 3                          // dpu/dpu.c:90:3
	jleu r20, r21, .LBB0_11
.Ltmp30:
// %bb.4:
	//DEBUG_VALUE: main:cache_B <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:padded_iters_row <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:total_cols <- $r21
	//DEBUG_VALUE: main:VEC_LEN <- $r21
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r20
	//DEBUG_VALUE: main:cache_A <- $r19
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r18
	//DEBUG_VALUE: main:cache_C <- $r16
	//DEBUG_VALUE: main:ITERS_ROW <- $r15
	//DEBUG_VALUE: main:mram_tile_addr_C <- $r14
	//DEBUG_VALUE: main:write_addr_C <- $r14
	//DEBUG_VALUE: main:num_col_chunks <- $r2
	//DEBUG_VALUE: main:mram_base_addr_B <- $r1
	//DEBUG_VALUE: rr <- 0
	//DEBUG_VALUE: main:pending_rows <- 0
	.loc	4 0 3                           // dpu/dpu.c:0:3
	move r1, 0, true, .LBB0_5
.Ltmp31:
.LBB0_7:                                //   in Loop: Header=BB0_5 Depth=1
	//DEBUG_VALUE: main:cache_B <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:padded_iters_row <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:total_cols <- $r21
	//DEBUG_VALUE: main:VEC_LEN <- $r21
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r20
	//DEBUG_VALUE: main:cache_A <- $r19
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r18
	//DEBUG_VALUE: main:cache_C <- $r16
	//DEBUG_VALUE: main:write_addr_C <- $r14
	//DEBUG_VALUE: main:num_col_chunks <- $r2
	//DEBUG_VALUE: main:row_addr_A <- undef
	//DEBUG_VALUE: main:write_addr_C <- $r14
	//DEBUG_VALUE: main:pending_rows <- $r1
	//DEBUG_VALUE: rr <- [DW_OP_plus_uconst 1, DW_OP_stack_value] undef
	.loc	4 90 28                         // dpu/dpu.c:90:28
	add r15, r15, -1, z, .LBB0_8
.Ltmp32:
.LBB0_5:                                // =>This Inner Loop Header: Depth=1
	//DEBUG_VALUE: main:cache_B <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:padded_iters_row <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:total_cols <- $r21
	//DEBUG_VALUE: main:VEC_LEN <- $r21
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r20
	//DEBUG_VALUE: main:cache_A <- $r19
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r18
	//DEBUG_VALUE: main:cache_C <- $r16
	//DEBUG_VALUE: main:write_addr_C <- $r14
	//DEBUG_VALUE: main:num_col_chunks <- $r2
	//DEBUG_VALUE: main:row_addr_A <- undef
	//DEBUG_VALUE: main:write_addr_C <- $r14
	//DEBUG_VALUE: main:pending_rows <- $r1
	//DEBUG_VALUE: rr <- undef
	//DEBUG_VALUE: chunk <- 0
	//DEBUG_VALUE: chunk_addr_B <- undef
	//DEBUG_VALUE: chunk_addr_A <- undef
	//DEBUG_VALUE: sum <- 0
	.loc	4 107 5 is_stmt 1               // dpu/dpu.c:107:5
	lsl_add r0, r16, r1, 2
	.loc	4 108 17                        // dpu/dpu.c:108:17
	add r1, r1, 1
.Ltmp33:
	//DEBUG_VALUE: main:pending_rows <- $r1
	.loc	4 107 27                        // dpu/dpu.c:107:27
	sw r0, 0, 0
	jneq r1, r20, .LBB0_7
.Ltmp34:
// %bb.6:                               //   in Loop: Header=BB0_5 Depth=1
	//DEBUG_VALUE: main:cache_B <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:padded_iters_row <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:total_cols <- $r21
	//DEBUG_VALUE: main:VEC_LEN <- $r21
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r20
	//DEBUG_VALUE: main:cache_A <- $r19
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r18
	//DEBUG_VALUE: main:cache_C <- $r16
	//DEBUG_VALUE: main:write_addr_C <- $r14
	//DEBUG_VALUE: main:num_col_chunks <- $r2
	//DEBUG_VALUE: main:pending_rows <- $r1
	//DEBUG_VALUE: sum <- 0
	.loc	4 0 27 is_stmt 0                // dpu/dpu.c:0:27
	move r0, -1
.Ltmp35:
	//DEBUG_VALUE: mram_write:nb_of_bytes <- $r18
	//DEBUG_VALUE: mram_write:from <- $r16
	//DEBUG_VALUE: mram_write:to <- $r14
	.file	6 "/opt/upmem/upmem-2025.1.0-Linux-x86_64/bin/../share/upmem/include/syslib" "mram.h"
	.loc	6 66 24 is_stmt 1               // /opt/upmem/upmem-2025.1.0-Linux-x86_64/bin/../share/upmem/include/syslib/mram.h:66:24
	lsr_add r0, r0, r18, 3
	lsl_add r0, r16, r0, 24
	sdma r0, r14, 0
.Ltmp36:
	.loc	4 113 20                        // dpu/dpu.c:113:20
	add r14, r14, r18
.Ltmp37:
	//DEBUG_VALUE: main:write_addr_C <- $r14
	//DEBUG_VALUE: main:pending_rows <- 0
	.loc	4 0 20 is_stmt 0                // dpu/dpu.c:0:20
	move r1, 0, true, .LBB0_7
.Ltmp38:
.LBB0_11:
	//DEBUG_VALUE: main:cache_B <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:padded_iters_row <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:total_cols <- $r21
	//DEBUG_VALUE: main:VEC_LEN <- $r21
	//DEBUG_VALUE: main:BUFFER_COUNT <- $r20
	//DEBUG_VALUE: main:cache_A <- $r19
	//DEBUG_VALUE: main:BUFFER_SIZE <- $r18
	//DEBUG_VALUE: main:cache_C <- $r16
	//DEBUG_VALUE: main:ITERS_ROW <- $r15
	//DEBUG_VALUE: main:mram_tile_addr_C <- $r14
	//DEBUG_VALUE: main:write_addr_C <- $r14
	//DEBUG_VALUE: main:num_col_chunks <- $r2
	//DEBUG_VALUE: main:mram_base_addr_B <- $r1
	//DEBUG_VALUE: main:pending_rows <- 0
	sw r22, -12, r1
	lsl r0, r21, 2
.Ltmp39:
	//DEBUG_VALUE: main:vec_el_size <- $r0
	sw r22, -20, r0
.Ltmp40:
	//DEBUG_VALUE: main:vec_el_size <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	.loc	4 74 64 is_stmt 1               // dpu/dpu.c:74:64
	lw r0, r22, -40
	move r1, r17
.Ltmp41:
	sw r22, -52, r2
	call r23, __mulsi3
.Ltmp42:
	.loc	4 0 64 is_stmt 0                // dpu/dpu.c:0:64
	lw r7, r22, -52
	.loc	4 74 24                         // dpu/dpu.c:74:24
	move r1, __sys_used_mram_end
	move r3, 0
	add r9, r0, r1
.Ltmp43:
	//DEBUG_VALUE: main:row_addr_A <- $r9
	//DEBUG_VALUE: main:mram_tile_addr_A <- $r9
	.loc	4 0 24                          // dpu/dpu.c:0:24
	move r1, r3
	sw r22, -16, r15
	sw r22, -24, r16
	lw r6, r22, -56
	sw r22, -48, r20
	sw r22, -44, r19
	jump .LBB0_12
.Ltmp44:
.LBB0_17:                               //   in Loop: Header=BB0_12 Depth=1
	//DEBUG_VALUE: main:vec_el_size <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:cache_B <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:padded_iters_row <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:pending_rows <- $r1
	move r0, -1
.Ltmp45:
	//DEBUG_VALUE: mram_write:nb_of_bytes <- $r18
	//DEBUG_VALUE: mram_write:from <- [DW_OP_constu 24, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: mram_write:to <- [DW_OP_constu 36, DW_OP_minus] [$r22+0]
	.loc	6 66 24 is_stmt 1               // /opt/upmem/upmem-2025.1.0-Linux-x86_64/bin/../share/upmem/include/syslib/mram.h:66:24
	lsr_add r0, r0, r18, 3
	lsl_add r0, r16, r0, 24
	lw r14, r22, -36
.Ltmp46:
	//DEBUG_VALUE: mram_write:to <- $r14
	sdma r0, r14, 0
.Ltmp47:
	.loc	4 113 20                        // dpu/dpu.c:113:20
	add r14, r14, r18
.Ltmp48:
	//DEBUG_VALUE: main:write_addr_C <- $r14
	.loc	4 0 20 is_stmt 0                // dpu/dpu.c:0:20
	move r1, 0
.Ltmp49:
	//DEBUG_VALUE: main:pending_rows <- 0
	lw r0, r22, -16
.Ltmp50:
.LBB0_18:                               //   in Loop: Header=BB0_12 Depth=1
	//DEBUG_VALUE: main:vec_el_size <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:cache_B <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:padded_iters_row <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	lw r3, r22, -32
	lw r9, r22, -28
	.loc	4 90 43 is_stmt 1               // dpu/dpu.c:90:43
	add r3, r3, 1
.Ltmp51:
	//DEBUG_VALUE: rr <- $r3
	//DEBUG_VALUE: main:row_addr_A <- $r9
	.loc	4 0 0 is_stmt 0                 // dpu/dpu.c:0:0
	lw r2, r22, -20
	add r9, r9, r2
.Ltmp52:
	//DEBUG_VALUE: main:pending_rows <- $r1
	//DEBUG_VALUE: main:write_addr_C <- $r14
	//DEBUG_VALUE: main:row_addr_A <- $r9
	//DEBUG_VALUE: rr <- $r3
	.loc	4 90 3                          // dpu/dpu.c:90:3
	jeq r3, r0, .LBB0_8
.Ltmp53:
.LBB0_12:                               // =>This Loop Header: Depth=1
                                        //     Child Loop BB0_14 Depth 2
                                        //       Child Loop BB0_15 Depth 3
                                        //     Child Loop BB0_20 Depth 2
	//DEBUG_VALUE: main:vec_el_size <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:cache_B <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:padded_iters_row <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:write_addr_C <- $r14
	//DEBUG_VALUE: main:row_addr_A <- $r9
	//DEBUG_VALUE: main:row_addr_A <- $r9
	//DEBUG_VALUE: main:write_addr_C <- $r14
	//DEBUG_VALUE: main:pending_rows <- $r1
	//DEBUG_VALUE: rr <- $r3
	//DEBUG_VALUE: chunk <- 0
	//DEBUG_VALUE: chunk_addr_B <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: chunk_addr_A <- $r9
	//DEBUG_VALUE: sum <- 0
	sw r22, -36, r14
	sw r22, -40, r1
	sw r22, -32, r3
	sw r22, -28, r9
	.loc	4 95 5 is_stmt 1                // dpu/dpu.c:95:5
	jeq r20, 0, .LBB0_19
.Ltmp54:
// %bb.13:                              //   in Loop: Header=BB0_12 Depth=1
	//DEBUG_VALUE: chunk_addr_B <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:vec_el_size <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:cache_B <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:padded_iters_row <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:write_addr_C <- $r14
	//DEBUG_VALUE: chunk_addr_A <- $r9
	//DEBUG_VALUE: main:row_addr_A <- $r9
	//DEBUG_VALUE: rr <- $r3
	//DEBUG_VALUE: main:pending_rows <- $r1
	//DEBUG_VALUE: sum <- 0
	//DEBUG_VALUE: chunk <- 0
	.loc	4 0 5 is_stmt 0                 // dpu/dpu.c:0:5
	move r21, 0
	lw r14, r22, -12
.Ltmp55:
	move r15, r9
	move r17, r21
.Ltmp56:
.LBB0_14:                               //   Parent Loop BB0_12 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB0_15 Depth 3
	//DEBUG_VALUE: main:vec_el_size <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:cache_B <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:padded_iters_row <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:row_addr_A <- $r9
	//DEBUG_VALUE: rr <- $r3
	//DEBUG_VALUE: chunk_addr_B <- $r14
	//DEBUG_VALUE: chunk_addr_A <- $r15
	//DEBUG_VALUE: sum <- $r17
	move r0, -1
	move r16, r18
.Ltmp57:
	//DEBUG_VALUE: mram_read:from <- $r15
	//DEBUG_VALUE: chunk <- $r21
	.loc	6 45 24 is_stmt 1               // /opt/upmem/upmem-2025.1.0-Linux-x86_64/bin/../share/upmem/include/syslib/mram.h:45:24
	lsr_add r0, r0, r18, 3
.Ltmp58:
	//DEBUG_VALUE: mram_read:nb_of_bytes <- $r16
	//DEBUG_VALUE: mram_read:to <- $r19
	lsl_add r1, r19, r0, 24
	ldma r1, r15, 0
.Ltmp59:
	//DEBUG_VALUE: mram_read:nb_of_bytes <- $r16
	//DEBUG_VALUE: mram_read:to <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: mram_read:from <- $r14
	.loc	6 45 24 is_stmt 0               // /opt/upmem/upmem-2025.1.0-Linux-x86_64/bin/../share/upmem/include/syslib/mram.h:45:24
	lsl_add r0, r6, r0, 24
	ldma r0, r14, 0
	move r18, r19
	move r19, r6
.Ltmp60:
	//DEBUG_VALUE: c <- 0
.LBB0_15:                               //   Parent Loop BB0_12 Depth=1
                                        //     Parent Loop BB0_14 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	//DEBUG_VALUE: main:vec_el_size <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:cache_B <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:padded_iters_row <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: chunk <- $r21
	//DEBUG_VALUE: sum <- $r17
	//DEBUG_VALUE: chunk_addr_A <- $r15
	//DEBUG_VALUE: chunk_addr_B <- $r14
	//DEBUG_VALUE: main:row_addr_A <- $r9
	//DEBUG_VALUE: rr <- $r3
	//DEBUG_VALUE: sum <- $r17
	//DEBUG_VALUE: c <- undef
	.loc	4 101 16 is_stmt 1              // dpu/dpu.c:101:16
	lw r1, r18, 0
	.loc	4 101 29 is_stmt 0              // dpu/dpu.c:101:29
	lw r0, r19, 0
	.loc	4 101 27                        // dpu/dpu.c:101:27
	call r23, __mulsi3
.Ltmp61:
	.loc	4 101 13                        // dpu/dpu.c:101:13
	add r17, r0, r17
.Ltmp62:
	//DEBUG_VALUE: c <- [DW_OP_plus_uconst 1, DW_OP_stack_value] undef
	//DEBUG_VALUE: sum <- $r17
	.loc	4 100 30 is_stmt 1              // dpu/dpu.c:100:30
	add r19, r19, 4
	add r18, r18, 4
	add r20, r20, -1, nz, .LBB0_15
.Ltmp63:
// %bb.16:                              //   in Loop: Header=BB0_14 Depth=2
	//DEBUG_VALUE: main:vec_el_size <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:cache_B <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:padded_iters_row <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: chunk <- $r21
	//DEBUG_VALUE: sum <- $r17
	//DEBUG_VALUE: chunk_addr_A <- $r15
	//DEBUG_VALUE: chunk_addr_B <- $r14
	//DEBUG_VALUE: main:row_addr_A <- $r9
	//DEBUG_VALUE: rr <- $r3
	.loc	4 0 30 is_stmt 0                // dpu/dpu.c:0:30
	move r18, r16
	.loc	4 103 20 is_stmt 1              // dpu/dpu.c:103:20
	add r15, r15, r18
.Ltmp64:
	//DEBUG_VALUE: chunk_addr_A <- $r15
	.loc	4 95 59                         // dpu/dpu.c:95:59
	add r21, r21, 1
.Ltmp65:
	//DEBUG_VALUE: sum <- $r17
	//DEBUG_VALUE: chunk <- $r21
	.loc	4 104 20                        // dpu/dpu.c:104:20
	add r14, r14, r18
.Ltmp66:
	//DEBUG_VALUE: chunk_addr_B <- $r14
	.loc	4 0 20 is_stmt 0                // dpu/dpu.c:0:20
	lw r20, r22, -48
	lw r19, r22, -44
	lw r6, r22, -56
	lw r7, r22, -52
.Ltmp67:
	.loc	4 95 5 is_stmt 1                // dpu/dpu.c:95:5
	jltu r21, r7, .LBB0_14
	jump .LBB0_21
.Ltmp68:
.LBB0_19:                               //   in Loop: Header=BB0_12 Depth=1
	//DEBUG_VALUE: chunk_addr_B <- [DW_OP_constu 12, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:vec_el_size <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:cache_B <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:padded_iters_row <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:write_addr_C <- $r14
	//DEBUG_VALUE: chunk_addr_A <- $r9
	//DEBUG_VALUE: main:row_addr_A <- $r9
	//DEBUG_VALUE: rr <- $r3
	//DEBUG_VALUE: main:pending_rows <- $r1
	//DEBUG_VALUE: sum <- 0
	//DEBUG_VALUE: chunk <- 0
	.loc	4 0 5 is_stmt 0                 // dpu/dpu.c:0:5
	move r17, 0
	move r0, r17
	move r1, r17
.Ltmp69:
	lw r8, r22, -12
.Ltmp70:
.LBB0_20:                               //   Parent Loop BB0_12 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	//DEBUG_VALUE: main:vec_el_size <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:cache_B <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:padded_iters_row <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:write_addr_C <- $r14
	//DEBUG_VALUE: main:row_addr_A <- $r9
	//DEBUG_VALUE: sum <- 0
	//DEBUG_VALUE: chunk <- $r1
	//DEBUG_VALUE: chunk_addr_B <- undef
	//DEBUG_VALUE: chunk_addr_A <- undef
	//DEBUG_VALUE: sum <- 0
	.loc	4 96 17 is_stmt 1               // dpu/dpu.c:96:17
	add r2, r8, r0
	add r3, r9, r0
	move r4, -1
.Ltmp71:
	.loc	6 45 24                         // /opt/upmem/upmem-2025.1.0-Linux-x86_64/bin/../share/upmem/include/syslib/mram.h:45:24
	lsr_add r4, r4, r18, 3
.Ltmp72:
	//DEBUG_VALUE: mram_read:nb_of_bytes <- $r18
	//DEBUG_VALUE: mram_read:to <- $r19
	//DEBUG_VALUE: mram_read:from <- $r3
	lsl_add r5, r19, r4, 24
	ldma r5, r3, 0
.Ltmp73:
	//DEBUG_VALUE: mram_read:nb_of_bytes <- $r18
	//DEBUG_VALUE: mram_read:to <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: mram_read:from <- $r2
	.loc	6 45 24 is_stmt 0               // /opt/upmem/upmem-2025.1.0-Linux-x86_64/bin/../share/upmem/include/syslib/mram.h:45:24
	lsl_add r3, r6, r4, 24
.Ltmp74:
	ldma r3, r2, 0
.Ltmp75:
	//DEBUG_VALUE: c <- 0
	//DEBUG_VALUE: chunk_addr_A <- undef
	//DEBUG_VALUE: chunk_addr_B <- undef
	.loc	4 95 59 is_stmt 1               // dpu/dpu.c:95:59
	add r1, r1, 1
.Ltmp76:
	//DEBUG_VALUE: chunk <- $r1
	.loc	4 0 59 is_stmt 0                // dpu/dpu.c:0:59
	add r0, r0, r18
.Ltmp77:
	.loc	4 95 5                          // dpu/dpu.c:95:5
	jltu r1, r7, .LBB0_20
.Ltmp78:
.LBB0_21:                               //   in Loop: Header=BB0_12 Depth=1
	//DEBUG_VALUE: main:vec_el_size <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:cache_B <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:padded_iters_row <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:row_addr_A <- $r9
	.loc	4 0 5                           // dpu/dpu.c:0:5
	lw r16, r22, -24
	lw r1, r22, -40
	.loc	4 107 5 is_stmt 1               // dpu/dpu.c:107:5
	lsl_add r0, r16, r1, 2
	.loc	4 108 17                        // dpu/dpu.c:108:17
	add r1, r1, 1
.Ltmp79:
	//DEBUG_VALUE: main:pending_rows <- $r1
	//DEBUG_VALUE: main:row_addr_A <- undef
	.loc	4 107 27                        // dpu/dpu.c:107:27
	sw r0, 0, r17
	.loc	4 111 9                         // dpu/dpu.c:111:9
	jeq r1, r20, .LBB0_17
.Ltmp80:
// %bb.22:                              //   in Loop: Header=BB0_12 Depth=1
	//DEBUG_VALUE: main:vec_el_size <- [DW_OP_constu 20, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:cache_B <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:padded_iters_row <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:pending_rows <- $r1
	.loc	4 0 9 is_stmt 0                 // dpu/dpu.c:0:9
	lw r0, r22, -16
	lw r14, r22, -36
	jump .LBB0_18
.Ltmp81:
.LBB0_8:
	//DEBUG_VALUE: main:cache_B <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:padded_iters_row <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:write_addr_C <- $r14
	//DEBUG_VALUE: main:pending_rows <- $r1
	jeq r1, 0, .LBB0_10
.Ltmp82:
// %bb.9:
	//DEBUG_VALUE: main:cache_B <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:padded_iters_row <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:write_addr_C <- $r14
	//DEBUG_VALUE: main:pending_rows <- $r1
	.loc	4 124 57 is_stmt 1              // dpu/dpu.c:124:57
	and r0, r1, 1
	.loc	4 124 41 is_stmt 0              // dpu/dpu.c:124:41
	add r0, r0, r1
.Ltmp83:
	//DEBUG_VALUE: write_count <- $r0
	.loc	4 126 28 is_stmt 1              // dpu/dpu.c:126:28
	lsl r0, r0, 2
.Ltmp84:
	.loc	4 0 28 is_stmt 0                // dpu/dpu.c:0:28
	move r1, -1
.Ltmp85:
	//DEBUG_VALUE: mram_write:to <- $r14
	//DEBUG_VALUE: mram_write:from <- $r16
	//DEBUG_VALUE: mram_write:nb_of_bytes <- $r0
	.loc	6 66 24 is_stmt 1               // /opt/upmem/upmem-2025.1.0-Linux-x86_64/bin/../share/upmem/include/syslib/mram.h:66:24
	lsr_add r0, r1, r0, 3
.Ltmp86:
	lsl_add r0, r16, r0, 24
	sdma r0, r14, 0
.Ltmp87:
.LBB0_10:
	//DEBUG_VALUE: main:cache_B <- [DW_OP_constu 56, DW_OP_minus] [$r22+0]
	//DEBUG_VALUE: main:padded_iters_row <- [DW_OP_constu 52, DW_OP_minus] [$r22+0]
	.loc	6 0 24 is_stmt 0                // /opt/upmem/upmem-2025.1.0-Linux-x86_64/bin/../share/upmem/include/syslib/mram.h:0:24
	move r0, 0
	.loc	4 137 1 is_stmt 1               // dpu/dpu.c:137:1
	ld d20, r22, -88
	ld d18, r22, -80
	ld d16, r22, -72
	ld d14, r22, -64
	ld d22, r22, -8
.Ltmp88:
	jump r23
.Ltmp89:
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
	.file	7 "/opt/upmem/upmem-2025.1.0-Linux-x86_64/bin/../share/upmem/include/syslib" "sysdef.h"
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
	.long	32                              // 0x20
	.long	1024                            // 0x400
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
	.byte	97                              // DW_OP_reg17
	.long	0
	.long	0
.Ldebug_loc1:
	.long	.Ltmp5-.Lfunc_begin0
	.long	.Ltmp44-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	100                             // DW_OP_reg20
	.long	0
	.long	0
.Ldebug_loc2:
	.long	.Ltmp6-.Lfunc_begin0
	.long	.Ltmp31-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	95                              // DW_OP_reg15
	.long	.Ltmp38-.Lfunc_begin0
	.long	.Ltmp44-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	95                              // DW_OP_reg15
	.long	0
	.long	0
.Ldebug_loc3:
	.long	.Ltmp7-.Lfunc_begin0
	.long	.Ltmp20-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	94                              // DW_OP_reg14
	.long	0
	.long	0
.Ldebug_loc4:
	.long	.Ltmp8-.Lfunc_begin0
	.long	.Ltmp44-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	101                             // DW_OP_reg21
	.long	0
	.long	0
.Ldebug_loc5:
	.long	.Ltmp9-.Lfunc_begin0
	.long	.Ltmp44-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	101                             // DW_OP_reg21
	.long	0
	.long	0
.Ldebug_loc6:
	.long	.Ltmp9-.Lfunc_begin0
	.long	.Ltmp44-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	98                              // DW_OP_reg18
	.long	0
	.long	0
.Ldebug_loc7:
	.long	.Ltmp10-.Lfunc_begin0
	.long	.Ltmp11-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	80                              // DW_OP_reg0
	.long	.Ltmp11-.Lfunc_begin0
	.long	.Ltmp88-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	76                              // -52
	.long	0
	.long	0
.Ldebug_loc8:
	.long	.Ltmp13-.Lfunc_begin0
	.long	.Ltmp44-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	99                              // DW_OP_reg19
	.long	0
	.long	0
.Ldebug_loc9:
	.long	.Ltmp15-.Lfunc_begin0
	.long	.Ltmp88-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	72                              // -56
	.long	0
	.long	0
.Ldebug_loc10:
	.long	.Ltmp17-.Lfunc_begin0
	.long	.Ltmp44-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	96                              // DW_OP_reg16
	.long	0
	.long	0
.Ldebug_loc11:
	.long	.Ltmp43-.Lfunc_begin0
	.long	.Ltmp44-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	89                              // DW_OP_reg9
	.long	0
	.long	0
.Ldebug_loc12:
	.long	.Ltmp27-.Lfunc_begin0
	.long	.Ltmp31-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	.Ltmp38-.Lfunc_begin0
	.long	.Ltmp41-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	0
	.long	0
.Ldebug_loc13:
	.long	.Ltmp26-.Lfunc_begin0
	.long	.Ltmp28-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	80                              // DW_OP_reg0
	.long	0
	.long	0
.Ldebug_loc14:
	.long	.Ltmp39-.Lfunc_begin0
	.long	.Ltmp40-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	80                              // DW_OP_reg0
	.long	.Ltmp40-.Lfunc_begin0
	.long	.Ltmp81-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	108                             // -20
	.long	0
	.long	0
.Ldebug_loc15:
	.long	.Ltmp20-.Lfunc_begin0
	.long	.Ltmp29-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	94                              // DW_OP_reg14
	.long	0
	.long	0
.Ldebug_loc16:
	.long	.Ltmp29-.Lfunc_begin0
	.long	.Ltmp31-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	94                              // DW_OP_reg14
	.long	.Ltmp38-.Lfunc_begin0
	.long	.Ltmp44-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	94                              // DW_OP_reg14
	.long	0
	.long	0
.Ldebug_loc17:
	.long	.Ltmp24-.Lfunc_begin0
	.long	.Ltmp31-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	48                              // DW_OP_lit0
	.byte	159                             // DW_OP_stack_value
	.long	.Ltmp31-.Lfunc_begin0
	.long	.Ltmp37-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	.Ltmp37-.Lfunc_begin0
	.long	.Ltmp44-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	48                              // DW_OP_lit0
	.byte	159                             // DW_OP_stack_value
	.long	.Ltmp44-.Lfunc_begin0
	.long	.Ltmp49-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	.Ltmp49-.Lfunc_begin0
	.long	.Ltmp50-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	48                              // DW_OP_lit0
	.byte	159                             // DW_OP_stack_value
	.long	.Ltmp52-.Lfunc_begin0
	.long	.Ltmp56-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	.Ltmp68-.Lfunc_begin0
	.long	.Ltmp69-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	.Ltmp79-.Lfunc_begin0
	.long	.Ltmp85-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	0
	.long	0
.Ldebug_loc18:
	.long	.Ltmp29-.Lfunc_begin0
	.long	.Ltmp44-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	94                              // DW_OP_reg14
	.long	.Ltmp48-.Lfunc_begin0
	.long	.Ltmp50-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	94                              // DW_OP_reg14
	.long	.Ltmp52-.Lfunc_begin0
	.long	.Ltmp55-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	94                              // DW_OP_reg14
	.long	.Ltmp68-.Lfunc_begin0
	.long	.Ltmp78-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	94                              // DW_OP_reg14
	.long	.Ltmp81-.Lfunc_begin0
	.long	.Ltmp87-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	94                              // DW_OP_reg14
	.long	0
	.long	0
.Ldebug_loc19:
	.long	.Ltmp43-.Lfunc_begin0
	.long	.Ltmp44-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	89                              // DW_OP_reg9
	.long	.Ltmp51-.Lfunc_begin0
	.long	.Ltmp79-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	89                              // DW_OP_reg9
	.long	0
	.long	0
.Ldebug_loc20:
	.long	.Ltmp24-.Lfunc_begin0
	.long	.Ltmp44-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	82                              // DW_OP_reg2
	.long	0
	.long	0
.Ldebug_loc21:
	.long	.Ltmp24-.Lfunc_begin0
	.long	.Ltmp31-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	48                              // DW_OP_lit0
	.byte	159                             // DW_OP_stack_value
	.long	.Ltmp51-.Lfunc_begin0
	.long	.Ltmp70-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	83                              // DW_OP_reg3
	.long	0
	.long	0
.Ldebug_loc22:
	.long	.Ltmp53-.Lfunc_begin0
	.long	.Ltmp56-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	48                              // DW_OP_lit0
	.byte	159                             // DW_OP_stack_value
	.long	.Ltmp57-.Lfunc_begin0
	.long	.Ltmp68-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	101                             // DW_OP_reg21
	.long	.Ltmp68-.Lfunc_begin0
	.long	.Ltmp70-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	48                              // DW_OP_lit0
	.byte	159                             // DW_OP_stack_value
	.long	.Ltmp70-.Lfunc_begin0
	.long	.Ltmp78-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	0
	.long	0
.Ldebug_loc23:
	.long	.Ltmp53-.Lfunc_begin0
	.long	.Ltmp56-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	116                             // -12
	.long	.Ltmp56-.Lfunc_begin0
	.long	.Ltmp68-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	94                              // DW_OP_reg14
	.long	.Ltmp68-.Lfunc_begin0
	.long	.Ltmp70-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	116                             // -12
	.long	0
	.long	0
.Ldebug_loc24:
	.long	.Ltmp53-.Lfunc_begin0
	.long	.Ltmp56-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	89                              // DW_OP_reg9
	.long	.Ltmp56-.Lfunc_begin0
	.long	.Ltmp68-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	95                              // DW_OP_reg15
	.long	.Ltmp68-.Lfunc_begin0
	.long	.Ltmp70-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	89                              // DW_OP_reg9
	.long	0
	.long	0
.Ldebug_loc25:
	.long	.Ltmp32-.Lfunc_begin0
	.long	.Ltmp38-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	48                              // DW_OP_lit0
	.byte	159                             // DW_OP_stack_value
	.long	.Ltmp53-.Lfunc_begin0
	.long	.Ltmp56-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	48                              // DW_OP_lit0
	.byte	159                             // DW_OP_stack_value
	.long	.Ltmp56-.Lfunc_begin0
	.long	.Ltmp68-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	97                              // DW_OP_reg17
	.long	.Ltmp68-.Lfunc_begin0
	.long	.Ltmp78-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	48                              // DW_OP_lit0
	.byte	159                             // DW_OP_stack_value
	.long	0
	.long	0
.Ldebug_loc26:
	.long	.Ltmp35-.Lfunc_begin0
	.long	.Ltmp38-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	98                              // DW_OP_reg18
	.long	.Ltmp45-.Lfunc_begin0
	.long	.Ltmp50-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	98                              // DW_OP_reg18
	.long	0
	.long	0
.Ldebug_loc27:
	.long	.Ltmp35-.Lfunc_begin0
	.long	.Ltmp38-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	96                              // DW_OP_reg16
	.long	.Ltmp45-.Lfunc_begin0
	.long	.Ltmp50-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	104                             // -24
	.long	0
	.long	0
.Ldebug_loc28:
	.long	.Ltmp35-.Lfunc_begin0
	.long	.Ltmp37-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	94                              // DW_OP_reg14
	.long	.Ltmp45-.Lfunc_begin0
	.long	.Ltmp46-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	92                              // -36
	.long	.Ltmp46-.Lfunc_begin0
	.long	.Ltmp48-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	94                              // DW_OP_reg14
	.long	0
	.long	0
.Ldebug_loc29:
	.long	.Ltmp57-.Lfunc_begin0
	.long	.Ltmp60-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	95                              // DW_OP_reg15
	.long	.Ltmp72-.Lfunc_begin0
	.long	.Ltmp74-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	83                              // DW_OP_reg3
	.long	0
	.long	0
.Ldebug_loc30:
	.long	.Ltmp58-.Lfunc_begin0
	.long	.Ltmp60-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	96                              // DW_OP_reg16
	.long	.Ltmp72-.Lfunc_begin0
	.long	.Ltmp78-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	98                              // DW_OP_reg18
	.long	0
	.long	0
.Ldebug_loc31:
	.long	.Ltmp58-.Lfunc_begin0
	.long	.Ltmp60-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	99                              // DW_OP_reg19
	.long	.Ltmp72-.Lfunc_begin0
	.long	.Ltmp78-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	99                              // DW_OP_reg19
	.long	0
	.long	0
.Ldebug_loc32:
	.long	.Ltmp59-.Lfunc_begin0
	.long	.Ltmp60-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	96                              // DW_OP_reg16
	.long	.Ltmp73-.Lfunc_begin0
	.long	.Ltmp78-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	98                              // DW_OP_reg18
	.long	0
	.long	0
.Ldebug_loc33:
	.long	.Ltmp59-.Lfunc_begin0
	.long	.Ltmp60-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	72                              // -56
	.long	.Ltmp73-.Lfunc_begin0
	.long	.Ltmp78-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	134                             // DW_OP_breg22
	.byte	72                              // -56
	.long	0
	.long	0
.Ldebug_loc34:
	.long	.Ltmp59-.Lfunc_begin0
	.long	.Ltmp60-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	94                              // DW_OP_reg14
	.long	.Ltmp73-.Lfunc_begin0
	.long	.Ltmp78-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	82                              // DW_OP_reg2
	.long	0
	.long	0
.Ldebug_loc35:
	.long	.Ltmp83-.Lfunc_begin0
	.long	.Ltmp84-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	80                              // DW_OP_reg0
	.long	0
	.long	0
.Ldebug_loc36:
	.long	.Ltmp85-.Lfunc_begin0
	.long	.Ltmp86-.Lfunc_begin0
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
	.byte	11                              // DW_TAG_lexical_block
	.byte	1                               // DW_CHILDREN_yes
	.byte	17                              // DW_AT_low_pc
	.byte	1                               // DW_FORM_addr
	.byte	18                              // DW_AT_high_pc
	.byte	6                               // DW_FORM_data4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	25                              // Abbreviation Code
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
	.byte	26                              // Abbreviation Code
	.byte	5                               // DW_TAG_formal_parameter
	.byte	0                               // DW_CHILDREN_no
	.byte	2                               // DW_AT_location
	.byte	24                              // DW_FORM_exprloc
	.byte	49                              // DW_AT_abstract_origin
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
	.byte	1                               // Abbrev [1] 0xb:0x48e DW_TAG_compile_unit
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
	.byte	60                              // DW_AT_decl_line
                                        // DW_AT_prototyped
	.byte	1                               // DW_AT_inline
	.byte	16                              // Abbrev [16] 0x152:0xb DW_TAG_formal_parameter
	.long	.Linfo_string23                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	60                              // DW_AT_decl_line
	.long	301                             // DW_AT_type
	.byte	16                              // Abbrev [16] 0x15d:0xb DW_TAG_formal_parameter
	.long	.Linfo_string24                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	60                              // DW_AT_decl_line
	.long	300                             // DW_AT_type
	.byte	16                              // Abbrev [16] 0x168:0xb DW_TAG_formal_parameter
	.long	.Linfo_string25                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	60                              // DW_AT_decl_line
	.long	78                              // DW_AT_type
	.byte	0                               // End Of Children Mark
	.byte	15                              // Abbrev [15] 0x174:0x2a DW_TAG_subprogram
	.long	.Linfo_string26                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	39                              // DW_AT_decl_line
                                        // DW_AT_prototyped
	.byte	1                               // DW_AT_inline
	.byte	16                              // Abbrev [16] 0x17c:0xb DW_TAG_formal_parameter
	.long	.Linfo_string23                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	39                              // DW_AT_decl_line
	.long	301                             // DW_AT_type
	.byte	16                              // Abbrev [16] 0x187:0xb DW_TAG_formal_parameter
	.long	.Linfo_string24                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	39                              // DW_AT_decl_line
	.long	300                             // DW_AT_type
	.byte	16                              // Abbrev [16] 0x192:0xb DW_TAG_formal_parameter
	.long	.Linfo_string25                 // DW_AT_name
	.byte	6                               // DW_AT_decl_file
	.byte	39                              // DW_AT_decl_line
	.long	78                              // DW_AT_type
	.byte	0                               // End Of Children Mark
	.byte	17                              // Abbrev [17] 0x19e:0x2f3 DW_TAG_subprogram
	.long	.Lfunc_begin0                   // DW_AT_low_pc
	.long	.Lfunc_end0-.Lfunc_begin0       // DW_AT_high_pc
	.byte	1                               // DW_AT_frame_base
	.byte	102
                                        // DW_AT_GNU_all_call_sites
	.long	.Linfo_string27                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	22                              // DW_AT_decl_line
	.long	1169                            // DW_AT_type
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
	.byte	45                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x1d1:0xf DW_TAG_variable
	.long	.Ldebug_loc2                    // DW_AT_location
	.long	.Linfo_string31                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	42                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x1e0:0xf DW_TAG_variable
	.long	.Ldebug_loc3                    // DW_AT_location
	.long	.Linfo_string13                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	41                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x1ef:0xf DW_TAG_variable
	.long	.Ldebug_loc4                    // DW_AT_location
	.long	.Linfo_string32                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	43                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x1fe:0xf DW_TAG_variable
	.long	.Ldebug_loc5                    // DW_AT_location
	.long	.Linfo_string33                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	49                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	19                              // Abbrev [19] 0x20d:0xb DW_TAG_variable
	.long	.Linfo_string34                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	48                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x218:0xf DW_TAG_variable
	.long	.Ldebug_loc6                    // DW_AT_location
	.long	.Linfo_string35                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	46                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x227:0xf DW_TAG_variable
	.long	.Ldebug_loc7                    // DW_AT_location
	.long	.Linfo_string36                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	59                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x236:0xf DW_TAG_variable
	.long	.Ldebug_loc8                    // DW_AT_location
	.long	.Linfo_string37                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	61                              // DW_AT_decl_line
	.long	295                             // DW_AT_type
	.byte	18                              // Abbrev [18] 0x245:0xf DW_TAG_variable
	.long	.Ldebug_loc9                    // DW_AT_location
	.long	.Linfo_string38                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	62                              // DW_AT_decl_line
	.long	295                             // DW_AT_type
	.byte	18                              // Abbrev [18] 0x254:0xf DW_TAG_variable
	.long	.Ldebug_loc10                   // DW_AT_location
	.long	.Linfo_string39                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	63                              // DW_AT_decl_line
	.long	295                             // DW_AT_type
	.byte	18                              // Abbrev [18] 0x263:0xf DW_TAG_variable
	.long	.Ldebug_loc11                   // DW_AT_location
	.long	.Linfo_string40                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	73                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	19                              // Abbrev [19] 0x272:0xb DW_TAG_variable
	.long	.Linfo_string41                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	70                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x27d:0xf DW_TAG_variable
	.long	.Ldebug_loc12                   // DW_AT_location
	.long	.Linfo_string42                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	69                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x28c:0xf DW_TAG_variable
	.long	.Ldebug_loc13                   // DW_AT_location
	.long	.Linfo_string43                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	68                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x29b:0xf DW_TAG_variable
	.long	.Ldebug_loc14                   // DW_AT_location
	.long	.Linfo_string44                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	66                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x2aa:0xf DW_TAG_variable
	.long	.Ldebug_loc15                   // DW_AT_location
	.long	.Linfo_string45                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	65                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x2b9:0xf DW_TAG_variable
	.long	.Ldebug_loc16                   // DW_AT_location
	.long	.Linfo_string46                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	75                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x2c8:0xf DW_TAG_variable
	.long	.Ldebug_loc17                   // DW_AT_location
	.long	.Linfo_string47                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	82                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x2d7:0xf DW_TAG_variable
	.long	.Ldebug_loc18                   // DW_AT_location
	.long	.Linfo_string48                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	81                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x2e6:0xf DW_TAG_variable
	.long	.Ldebug_loc19                   // DW_AT_location
	.long	.Linfo_string49                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	80                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x2f5:0xf DW_TAG_variable
	.long	.Ldebug_loc20                   // DW_AT_location
	.long	.Linfo_string50                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	78                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	20                              // Abbrev [20] 0x304:0x10 DW_TAG_inlined_subroutine
	.long	307                             // DW_AT_abstract_origin
	.long	.Ltmp0                          // DW_AT_low_pc
	.long	.Ltmp2-.Ltmp0                   // DW_AT_high_pc
	.byte	4                               // DW_AT_call_file
	.byte	23                              // DW_AT_call_line
	.byte	29                              // DW_AT_call_column
	.byte	21                              // Abbrev [21] 0x314:0xee DW_TAG_lexical_block
	.long	.Ldebug_ranges4                 // DW_AT_ranges
	.byte	18                              // Abbrev [18] 0x319:0xf DW_TAG_variable
	.long	.Ldebug_loc21                   // DW_AT_location
	.long	.Linfo_string51                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	90                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	21                              // Abbrev [21] 0x328:0xd9 DW_TAG_lexical_block
	.long	.Ldebug_ranges3                 // DW_AT_ranges
	.byte	18                              // Abbrev [18] 0x32d:0xf DW_TAG_variable
	.long	.Ldebug_loc23                   // DW_AT_location
	.long	.Linfo_string53                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	93                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x33c:0xf DW_TAG_variable
	.long	.Ldebug_loc24                   // DW_AT_location
	.long	.Linfo_string54                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	92                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x34b:0xf DW_TAG_variable
	.long	.Ldebug_loc25                   // DW_AT_location
	.long	.Linfo_string55                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	91                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	22                              // Abbrev [22] 0x35a:0x28 DW_TAG_inlined_subroutine
	.long	330                             // DW_AT_abstract_origin
	.long	.Ldebug_ranges0                 // DW_AT_ranges
	.byte	4                               // DW_AT_call_file
	.byte	112                             // DW_AT_call_line
	.byte	7                               // DW_AT_call_column
	.byte	23                              // Abbrev [23] 0x366:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc27                   // DW_AT_location
	.long	338                             // DW_AT_abstract_origin
	.byte	23                              // Abbrev [23] 0x36f:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc28                   // DW_AT_location
	.long	349                             // DW_AT_abstract_origin
	.byte	23                              // Abbrev [23] 0x378:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc26                   // DW_AT_location
	.long	360                             // DW_AT_abstract_origin
	.byte	0                               // End Of Children Mark
	.byte	24                              // Abbrev [24] 0x382:0x7e DW_TAG_lexical_block
	.long	.Ltmp53                         // DW_AT_low_pc
	.long	.Ltmp78-.Ltmp53                 // DW_AT_high_pc
	.byte	18                              // Abbrev [18] 0x38b:0xf DW_TAG_variable
	.long	.Ldebug_loc22                   // DW_AT_location
	.long	.Linfo_string52                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	95                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	22                              // Abbrev [22] 0x39a:0x28 DW_TAG_inlined_subroutine
	.long	372                             // DW_AT_abstract_origin
	.long	.Ldebug_ranges1                 // DW_AT_ranges
	.byte	4                               // DW_AT_call_file
	.byte	96                              // DW_AT_call_line
	.byte	7                               // DW_AT_call_column
	.byte	23                              // Abbrev [23] 0x3a6:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc29                   // DW_AT_location
	.long	380                             // DW_AT_abstract_origin
	.byte	23                              // Abbrev [23] 0x3af:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc31                   // DW_AT_location
	.long	391                             // DW_AT_abstract_origin
	.byte	23                              // Abbrev [23] 0x3b8:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc30                   // DW_AT_location
	.long	402                             // DW_AT_abstract_origin
	.byte	0                               // End Of Children Mark
	.byte	22                              // Abbrev [22] 0x3c2:0x28 DW_TAG_inlined_subroutine
	.long	372                             // DW_AT_abstract_origin
	.long	.Ldebug_ranges2                 // DW_AT_ranges
	.byte	4                               // DW_AT_call_file
	.byte	98                              // DW_AT_call_line
	.byte	7                               // DW_AT_call_column
	.byte	23                              // Abbrev [23] 0x3ce:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc34                   // DW_AT_location
	.long	380                             // DW_AT_abstract_origin
	.byte	23                              // Abbrev [23] 0x3d7:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc33                   // DW_AT_location
	.long	391                             // DW_AT_abstract_origin
	.byte	23                              // Abbrev [23] 0x3e0:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc32                   // DW_AT_location
	.long	402                             // DW_AT_abstract_origin
	.byte	0                               // End Of Children Mark
	.byte	24                              // Abbrev [24] 0x3ea:0x15 DW_TAG_lexical_block
	.long	.Ltmp60                         // DW_AT_low_pc
	.long	.Ltmp63-.Ltmp60                 // DW_AT_high_pc
	.byte	19                              // Abbrev [19] 0x3f3:0xb DW_TAG_variable
	.long	.Linfo_string56                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	100                             // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	0                               // End Of Children Mark
	.byte	0                               // End Of Children Mark
	.byte	0                               // End Of Children Mark
	.byte	0                               // End Of Children Mark
	.byte	24                              // Abbrev [24] 0x402:0x41 DW_TAG_lexical_block
	.long	.Ltmp82                         // DW_AT_low_pc
	.long	.Ltmp87-.Ltmp82                 // DW_AT_high_pc
	.byte	18                              // Abbrev [18] 0x40b:0xf DW_TAG_variable
	.long	.Ldebug_loc35                   // DW_AT_location
	.long	.Linfo_string57                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	124                             // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	25                              // Abbrev [25] 0x41a:0x28 DW_TAG_inlined_subroutine
	.long	330                             // DW_AT_abstract_origin
	.long	.Ltmp85                         // DW_AT_low_pc
	.long	.Ltmp87-.Ltmp85                 // DW_AT_high_pc
	.byte	4                               // DW_AT_call_file
	.byte	125                             // DW_AT_call_line
	.byte	5                               // DW_AT_call_column
	.byte	26                              // Abbrev [26] 0x42a:0x7 DW_TAG_formal_parameter
	.byte	1                               // DW_AT_location
	.byte	96
	.long	338                             // DW_AT_abstract_origin
	.byte	26                              // Abbrev [26] 0x431:0x7 DW_TAG_formal_parameter
	.byte	1                               // DW_AT_location
	.byte	94
	.long	349                             // DW_AT_abstract_origin
	.byte	23                              // Abbrev [23] 0x438:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc36                   // DW_AT_location
	.long	360                             // DW_AT_abstract_origin
	.byte	0                               // End Of Children Mark
	.byte	0                               // End Of Children Mark
	.byte	27                              // Abbrev [27] 0x443:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp3                          // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x44a:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp4                          // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x451:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp12                         // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x458:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp14                         // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x45f:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp16                         // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x466:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp18                         // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x46d:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp19                         // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x474:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp21                         // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x47b:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp23                         // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x482:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp42                         // DW_AT_low_pc
	.byte	27                              // Abbrev [27] 0x489:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp61                         // DW_AT_low_pc
	.byte	0                               // End Of Children Mark
	.byte	6                               // Abbrev [6] 0x491:0x7 DW_TAG_base_type
	.long	.Linfo_string28                 // DW_AT_name
	.byte	5                               // DW_AT_encoding
	.byte	4                               // DW_AT_byte_size
	.byte	0                               // End Of Children Mark
.Ldebug_info_end0:
	.section	.debug_ranges,"",@progbits
.Ldebug_ranges0:
	.long	.Ltmp35-.Lfunc_begin0
	.long	.Ltmp36-.Lfunc_begin0
	.long	.Ltmp45-.Lfunc_begin0
	.long	.Ltmp47-.Lfunc_begin0
	.long	0
	.long	0
.Ldebug_ranges1:
	.long	.Ltmp57-.Lfunc_begin0
	.long	.Ltmp59-.Lfunc_begin0
	.long	.Ltmp71-.Lfunc_begin0
	.long	.Ltmp73-.Lfunc_begin0
	.long	0
	.long	0
.Ldebug_ranges2:
	.long	.Ltmp59-.Lfunc_begin0
	.long	.Ltmp60-.Lfunc_begin0
	.long	.Ltmp73-.Lfunc_begin0
	.long	.Ltmp75-.Lfunc_begin0
	.long	0
	.long	0
.Ldebug_ranges3:
	.long	.Ltmp32-.Lfunc_begin0
	.long	.Ltmp38-.Lfunc_begin0
	.long	.Ltmp45-.Lfunc_begin0
	.long	.Ltmp50-.Lfunc_begin0
	.long	.Ltmp53-.Lfunc_begin0
	.long	.Ltmp80-.Lfunc_begin0
	.long	0
	.long	0
.Ldebug_ranges4:
	.long	.Ltmp24-.Lfunc_begin0
	.long	.Ltmp25-.Lfunc_begin0
	.long	.Ltmp29-.Lfunc_begin0
	.long	.Ltmp38-.Lfunc_begin0
	.long	.Ltmp45-.Lfunc_begin0
	.long	.Ltmp51-.Lfunc_begin0
	.long	.Ltmp52-.Lfunc_begin0
	.long	.Ltmp80-.Lfunc_begin0
	.long	0
	.long	0
	.section	.debug_str,"MS",@progbits,1
.Linfo_string0:
	.asciz	"clang version 12.0.0 (https://github.com/upmem/llvm-project.git 02a1f3ffa0f15d3c21b60ffc12196131c3e3cffa)" // string offset=0
.Linfo_string1:
	.asciz	"dpu/dpu.c"                     // string offset=106
.Linfo_string2:
	.asciz	"/home/hamid.farzaneh/CostModelEvaluationRepo/NewEval/Gemv" // string offset=116
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
	.asciz	"BUFFER_COUNT"                  // string offset=490
.Linfo_string31:
	.asciz	"ITERS_ROW"                     // string offset=503
.Linfo_string32:
	.asciz	"VEC_LEN"                       // string offset=513
.Linfo_string33:
	.asciz	"total_cols"                    // string offset=521
.Linfo_string34:
	.asciz	"total_rows"                    // string offset=532
.Linfo_string35:
	.asciz	"BUFFER_SIZE"                   // string offset=543
.Linfo_string36:
	.asciz	"padded_iters_row"              // string offset=555
.Linfo_string37:
	.asciz	"cache_A"                       // string offset=572
.Linfo_string38:
	.asciz	"cache_B"                       // string offset=580
.Linfo_string39:
	.asciz	"cache_C"                       // string offset=588
.Linfo_string40:
	.asciz	"mram_tile_addr_A"              // string offset=596
.Linfo_string41:
	.asciz	"mram_base_addr_C"              // string offset=613
.Linfo_string42:
	.asciz	"mram_base_addr_B"              // string offset=630
.Linfo_string43:
	.asciz	"mram_base_addr_A"              // string offset=647
.Linfo_string44:
	.asciz	"vec_el_size"                   // string offset=664
.Linfo_string45:
	.asciz	"mat_el_size"                   // string offset=676
.Linfo_string46:
	.asciz	"mram_tile_addr_C"              // string offset=688
.Linfo_string47:
	.asciz	"pending_rows"                  // string offset=705
.Linfo_string48:
	.asciz	"write_addr_C"                  // string offset=718
.Linfo_string49:
	.asciz	"row_addr_A"                    // string offset=731
.Linfo_string50:
	.asciz	"num_col_chunks"                // string offset=742
.Linfo_string51:
	.asciz	"rr"                            // string offset=757
.Linfo_string52:
	.asciz	"chunk"                         // string offset=760
.Linfo_string53:
	.asciz	"chunk_addr_B"                  // string offset=766
.Linfo_string54:
	.asciz	"chunk_addr_A"                  // string offset=779
.Linfo_string55:
	.asciz	"sum"                           // string offset=792
.Linfo_string56:
	.asciz	"c"                             // string offset=796
.Linfo_string57:
	.asciz	"write_count"                   // string offset=798
	.addrsig
	.addrsig_sym my_barrier
	.addrsig_sym DPU_INPUT_ARGUMENTS
	.addrsig_sym __sys_used_mram_end
	.addrsig_sym nb_cycle
	.section	.debug_line,"",@progbits
.Lline_table_start0:
