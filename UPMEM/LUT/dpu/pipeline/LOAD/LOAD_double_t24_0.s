	.text
	.file	"task.c"
	.file	1 "/opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/stdlib" "stdint.h"
	.file	2 "/home/hamid.farzaneh/CostModelEvaluationRepo/LUT" "./dpu/task.c"
	.file	3 "/home/hamid.farzaneh/CostModelEvaluationRepo/LUT" "./dpu/../support/common.h"
	.section	.text.main,"ax",@progbits
	.globl	main                            // -- Begin function main
	.type	main,@function
main:                                   // @main
.Lfunc_begin0:
	.loc	2 16 0                          // ./dpu/task.c:16:0
	.cfi_sections .debug_frame
	.cfi_startproc
// %bb.0:
	.cfi_def_cfa_offset -32
	.cfi_offset 23, -8
	.cfi_offset 22, -4
	sd r22, 24, d22
	add r22, r22, 32
	.cfi_offset 15, -16
	.cfi_offset 14, -12
.Ltmp0:
	sd r22, -16, d14
	.cfi_offset 17, -24
	.cfi_offset 16, -20
	sd r22, -24, d16
	.cfi_offset 19, -32
	.cfi_offset 18, -28
	sd r22, -32, d18
	.loc	2 17 39 prologue_end            // ./dpu/task.c:17:39
	move r0, DPU_INPUT_ARGUMENTS
	.loc	2 20 46                         // ./dpu/task.c:20:46
	lw r17, r0, 4
.Ltmp1:
	//DEBUG_VALUE: main:b_offset <- undef
	//DEBUG_VALUE: main:mram_base_addr_B <- undef
	//DEBUG_VALUE: main:buffer_size <- $r17
	.loc	2 17 39                         // ./dpu/task.c:17:39
	lw r18, zero, DPU_INPUT_ARGUMENTS
.Ltmp2:
	//DEBUG_VALUE: main:ITER <- $r18
	.loc	2 19 43                         // ./dpu/task.c:19:43
	lw r19, r0, 16
.Ltmp3:
	//DEBUG_VALUE: main:arr_size <- $r19
	.loc	2 25 43                         // ./dpu/task.c:25:43
	lsl r14, r17, 3
	.loc	2 25 21 is_stmt 0               // ./dpu/task.c:25:21
	move r0, r14
	call r23, mem_alloc
.Ltmp4:
	move r15, r0
.Ltmp5:
	//DEBUG_VALUE: main:cache_A <- $r15
	.loc	2 26 21 is_stmt 1               // ./dpu/task.c:26:21
	move r0, r14
	call r23, mem_alloc
.Ltmp6:
	move r16, r0
.Ltmp7:
	//DEBUG_VALUE: main:cache_B <- $r16
	.loc	2 27 21                         // ./dpu/task.c:27:21
	move r0, r14
	call r23, mem_alloc
.Ltmp8:
	//DEBUG_VALUE: main:cache_C <- $r0
	//DEBUG_VALUE: i <- 0
	.loc	2 32 3                          // ./dpu/task.c:32:3
	jeq r18, 0, .LBB0_4
.Ltmp9:
// %bb.1:
	//DEBUG_VALUE: main:arr_size <- $r19
	//DEBUG_VALUE: main:ITER <- $r18
	//DEBUG_VALUE: main:buffer_size <- $r17
	//DEBUG_VALUE: main:cache_B <- $r16
	//DEBUG_VALUE: main:cache_A <- $r15
	//DEBUG_VALUE: main:cache_C <- $r0
	//DEBUG_VALUE: i <- 0
	.loc	2 0 3 is_stmt 0                 // ./dpu/task.c:0:3
	move r1, __sys_used_mram_end
	lsl_add r1, r1, r19, 3
.Ltmp10:
	//DEBUG_VALUE: main:b_offset <- $r1
	//DEBUG_VALUE: main:mram_base_addr_B <- $r1
	move r2, 0, true, .LBB0_2
.Ltmp11:
.LBB0_3:                                //   in Loop: Header=BB0_2 Depth=1
	//DEBUG_VALUE: main:arr_size <- $r19
	//DEBUG_VALUE: main:ITER <- $r18
	//DEBUG_VALUE: main:buffer_size <- $r17
	//DEBUG_VALUE: main:cache_B <- $r16
	//DEBUG_VALUE: main:cache_A <- $r15
	//DEBUG_VALUE: i <- $r2
	//DEBUG_VALUE: main:mram_base_addr_B <- $r1
	//DEBUG_VALUE: main:b_offset <- $r1
	//DEBUG_VALUE: main:cache_C <- $r0
	.loc	2 32 35                         // ./dpu/task.c:32:35
	add r2, r2, 1
.Ltmp12:
	//DEBUG_VALUE: i <- $r2
	.loc	2 32 3                          // ./dpu/task.c:32:3
	jeq r2, r18, .LBB0_4
.Ltmp13:
.LBB0_2:                                // =>This Loop Header: Depth=1
                                        //     Child Loop BB0_5 Depth 2
	//DEBUG_VALUE: main:arr_size <- $r19
	//DEBUG_VALUE: main:ITER <- $r18
	//DEBUG_VALUE: main:buffer_size <- $r17
	//DEBUG_VALUE: main:cache_B <- $r16
	//DEBUG_VALUE: main:cache_A <- $r15
	//DEBUG_VALUE: main:mram_base_addr_B <- $r1
	//DEBUG_VALUE: main:b_offset <- $r1
	//DEBUG_VALUE: main:cache_C <- $r0
	.loc	2 0 3                           // ./dpu/task.c:0:3
	move r3, -1
.Ltmp14:
	//DEBUG_VALUE: mram_read:nb_of_bytes <- [DW_OP_constu 3, DW_OP_shl, DW_OP_stack_value] $r17
	//DEBUG_VALUE: mram_read:to <- $r15
	//DEBUG_VALUE: i <- $r2
	.file	4 "/opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib" "mram.h"
	.loc	4 39 24 is_stmt 1               // /opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/mram.h:39:24
	lsr_add r3, r3, r14, 3
	lsl_add r4, r15, r3, 24
	move r5, __sys_used_mram_end
.Ltmp15:
	//DEBUG_VALUE: mram_read:from <- $r5
	ldma r4, r5, 0
.Ltmp16:
	//DEBUG_VALUE: mram_read:nb_of_bytes <- [DW_OP_constu 3, DW_OP_shl, DW_OP_stack_value] $r17
	//DEBUG_VALUE: mram_read:to <- $r16
	//DEBUG_VALUE: mram_read:from <- $r1
	.loc	4 39 24 is_stmt 0               // /opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/mram.h:39:24
	lsl_add r3, r16, r3, 24
	ldma r3, r1, 0
	move r3, r15
	move r4, r0
	//DEBUG_VALUE: j <- 0
	move r5, r17, sz, .LBB0_3
.Ltmp17:
.LBB0_5:                                //   Parent Loop BB0_2 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	//DEBUG_VALUE: main:arr_size <- $r19
	//DEBUG_VALUE: main:ITER <- $r18
	//DEBUG_VALUE: main:buffer_size <- $r17
	//DEBUG_VALUE: main:cache_B <- $r16
	//DEBUG_VALUE: main:cache_A <- $r15
	//DEBUG_VALUE: i <- $r2
	//DEBUG_VALUE: main:mram_base_addr_B <- $r1
	//DEBUG_VALUE: main:b_offset <- $r1
	//DEBUG_VALUE: main:cache_C <- $r0
	//DEBUG_VALUE: j <- undef
	.loc	2 47 20 is_stmt 1               // ./dpu/task.c:47:20
	.loc	2 47 18 is_stmt 0               // ./dpu/task.c:47:18
	sd r4, 0, d6
.Ltmp18:
	//DEBUG_VALUE: j <- [DW_OP_plus_uconst 1, DW_OP_stack_value] undef
	.loc	2 37 28 is_stmt 1               // ./dpu/task.c:37:28
	add r4, r4, 8
	add r3, r3, 8
	add r5, r5, -1
.Ltmp19:
	.loc	2 37 5 is_stmt 0                // ./dpu/task.c:37:5
	jneq r5, 0, .LBB0_5
	jump .LBB0_3
.Ltmp20:
.LBB0_4:
	//DEBUG_VALUE: main:arr_size <- $r19
	//DEBUG_VALUE: main:ITER <- $r18
	//DEBUG_VALUE: main:buffer_size <- $r17
	//DEBUG_VALUE: main:cache_B <- $r16
	//DEBUG_VALUE: main:cache_A <- $r15
	//DEBUG_VALUE: main:cache_C <- $r0
	.loc	2 0 5                           // ./dpu/task.c:0:5
	move r0, 0
.Ltmp21:
	.loc	2 71 1 is_stmt 1                // ./dpu/task.c:71:1
	ld d18, r22, -32
.Ltmp22:
	ld d16, r22, -24
.Ltmp23:
	ld d14, r22, -16
.Ltmp24:
	ld d22, r22, -8
	jump r23
.Ltmp25:
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
	.section	.stack_sizes,"o",@progbits,.text.main
	.long	.Lfunc_begin0
	.byte	32
	.section	.text.main,"ax",@progbits
                                        // -- End function
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

	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
	.long	.Ltmp10-.Lfunc_begin0
	.long	.Ltmp20-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	0
	.long	0
.Ldebug_loc1:
	.long	.Ltmp10-.Lfunc_begin0
	.long	.Ltmp20-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	0
	.long	0
.Ldebug_loc2:
	.long	.Ltmp1-.Lfunc_begin0
	.long	.Ltmp23-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	97                              // DW_OP_reg17
	.long	0
	.long	0
.Ldebug_loc3:
	.long	.Ltmp2-.Lfunc_begin0
	.long	.Ltmp22-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	98                              // DW_OP_reg18
	.long	0
	.long	0
.Ldebug_loc4:
	.long	.Ltmp3-.Lfunc_begin0
	.long	.Ltmp22-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	99                              // DW_OP_reg19
	.long	0
	.long	0
.Ldebug_loc5:
	.long	.Ltmp5-.Lfunc_begin0
	.long	.Ltmp24-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	95                              // DW_OP_reg15
	.long	0
	.long	0
.Ldebug_loc6:
	.long	.Ltmp7-.Lfunc_begin0
	.long	.Ltmp23-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	96                              // DW_OP_reg16
	.long	0
	.long	0
.Ldebug_loc7:
	.long	.Ltmp8-.Lfunc_begin0
	.long	.Ltmp21-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	80                              // DW_OP_reg0
	.long	0
	.long	0
.Ldebug_loc8:
	.long	.Ltmp8-.Lfunc_begin0
	.long	.Ltmp11-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	48                              // DW_OP_lit0
	.byte	159                             // DW_OP_stack_value
	.long	.Ltmp11-.Lfunc_begin0
	.long	.Ltmp13-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	82                              // DW_OP_reg2
	.long	.Ltmp14-.Lfunc_begin0
	.long	.Ltmp20-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	82                              // DW_OP_reg2
	.long	0
	.long	0
.Ldebug_loc9:
	.long	.Ltmp15-.Lfunc_begin0
	.long	.Ltmp17-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	85                              // DW_OP_reg5
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
	.ascii	"\210\001"                      // DW_AT_alignment
	.byte	15                              // DW_FORM_udata
	.byte	2                               // DW_AT_location
	.byte	24                              // DW_FORM_exprloc
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	3                               // Abbreviation Code
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
	.byte	4                               // Abbreviation Code
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
	.byte	5                               // Abbreviation Code
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
	.byte	6                               // Abbreviation Code
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
	.byte	7                               // Abbreviation Code
	.byte	15                              // DW_TAG_pointer_type
	.byte	0                               // DW_CHILDREN_no
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	8                               // Abbreviation Code
	.byte	15                              // DW_TAG_pointer_type
	.byte	0                               // DW_CHILDREN_no
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	9                               // Abbreviation Code
	.byte	38                              // DW_TAG_const_type
	.byte	0                               // DW_CHILDREN_no
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	10                              // Abbreviation Code
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
	.byte	11                              // Abbreviation Code
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
	.byte	12                              // Abbreviation Code
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
	.byte	13                              // Abbreviation Code
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
	.byte	14                              // Abbreviation Code
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
	.byte	15                              // Abbreviation Code
	.byte	11                              // DW_TAG_lexical_block
	.byte	1                               // DW_CHILDREN_yes
	.byte	17                              // DW_AT_low_pc
	.byte	1                               // DW_FORM_addr
	.byte	18                              // DW_AT_high_pc
	.byte	6                               // DW_FORM_data4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	16                              // Abbreviation Code
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
	.byte	17                              // Abbreviation Code
	.byte	5                               // DW_TAG_formal_parameter
	.byte	0                               // DW_CHILDREN_no
	.byte	2                               // DW_AT_location
	.byte	23                              // DW_FORM_sec_offset
	.byte	49                              // DW_AT_abstract_origin
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	18                              // Abbreviation Code
	.byte	5                               // DW_TAG_formal_parameter
	.byte	0                               // DW_CHILDREN_no
	.byte	2                               // DW_AT_location
	.byte	24                              // DW_FORM_exprloc
	.byte	49                              // DW_AT_abstract_origin
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	19                              // Abbreviation Code
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
	.byte	1                               // Abbrev [1] 0xb:0x21f DW_TAG_compile_unit
	.long	.Linfo_string0                  // DW_AT_producer
	.short	12                              // DW_AT_language
	.long	.Linfo_string1                  // DW_AT_name
	.long	.Lline_table_start0             // DW_AT_stmt_list
	.long	.Linfo_string2                  // DW_AT_comp_dir
	.long	.Lfunc_begin0                   // DW_AT_low_pc
	.long	.Lfunc_end0-.Lfunc_begin0       // DW_AT_high_pc
	.byte	2                               // Abbrev [2] 0x26:0x12 DW_TAG_variable
	.long	.Linfo_string3                  // DW_AT_name
	.long	56                              // DW_AT_type
                                        // DW_AT_external
	.byte	2                               // DW_AT_decl_file
	.byte	12                              // DW_AT_decl_line
	.byte	8                               // DW_AT_alignment
	.byte	5                               // DW_AT_location
	.byte	3
	.long	nb_cycle
	.byte	3                               // Abbrev [3] 0x38:0xb DW_TAG_typedef
	.long	67                              // DW_AT_type
	.long	.Linfo_string5                  // DW_AT_name
	.byte	1                               // DW_AT_decl_file
	.byte	48                              // DW_AT_decl_line
	.byte	4                               // Abbrev [4] 0x43:0x7 DW_TAG_base_type
	.long	.Linfo_string4                  // DW_AT_name
	.byte	7                               // DW_AT_encoding
	.byte	4                               // DW_AT_byte_size
	.byte	2                               // Abbrev [2] 0x4a:0x12 DW_TAG_variable
	.long	.Linfo_string6                  // DW_AT_name
	.long	92                              // DW_AT_type
                                        // DW_AT_external
	.byte	2                               // DW_AT_decl_file
	.byte	14                              // DW_AT_decl_line
	.byte	8                               // DW_AT_alignment
	.byte	5                               // DW_AT_location
	.byte	3
	.long	DPU_INPUT_ARGUMENTS
	.byte	3                               // Abbrev [3] 0x5c:0xb DW_TAG_typedef
	.long	103                             // DW_AT_type
	.long	.Linfo_string12                 // DW_AT_name
	.byte	3                               // DW_AT_decl_file
	.byte	8                               // DW_AT_decl_line
	.byte	5                               // Abbrev [5] 0x67:0x41 DW_TAG_structure_type
	.byte	20                              // DW_AT_byte_size
	.byte	3                               // DW_AT_decl_file
	.byte	2                               // DW_AT_decl_line
	.byte	6                               // Abbrev [6] 0x6b:0xc DW_TAG_member
	.long	.Linfo_string7                  // DW_AT_name
	.long	56                              // DW_AT_type
	.byte	3                               // DW_AT_decl_file
	.byte	3                               // DW_AT_decl_line
	.byte	0                               // DW_AT_data_member_location
	.byte	6                               // Abbrev [6] 0x77:0xc DW_TAG_member
	.long	.Linfo_string8                  // DW_AT_name
	.long	56                              // DW_AT_type
	.byte	3                               // DW_AT_decl_file
	.byte	4                               // DW_AT_decl_line
	.byte	4                               // DW_AT_data_member_location
	.byte	6                               // Abbrev [6] 0x83:0xc DW_TAG_member
	.long	.Linfo_string9                  // DW_AT_name
	.long	56                              // DW_AT_type
	.byte	3                               // DW_AT_decl_file
	.byte	5                               // DW_AT_decl_line
	.byte	8                               // DW_AT_data_member_location
	.byte	6                               // Abbrev [6] 0x8f:0xc DW_TAG_member
	.long	.Linfo_string10                 // DW_AT_name
	.long	56                              // DW_AT_type
	.byte	3                               // DW_AT_decl_file
	.byte	6                               // DW_AT_decl_line
	.byte	12                              // DW_AT_data_member_location
	.byte	6                               // Abbrev [6] 0x9b:0xc DW_TAG_member
	.long	.Linfo_string11                 // DW_AT_name
	.long	56                              // DW_AT_type
	.byte	3                               // DW_AT_decl_file
	.byte	7                               // DW_AT_decl_line
	.byte	16                              // DW_AT_data_member_location
	.byte	0                               // End Of Children Mark
	.byte	7                               // Abbrev [7] 0xa8:0x1 DW_TAG_pointer_type
	.byte	8                               // Abbrev [8] 0xa9:0x5 DW_TAG_pointer_type
	.long	174                             // DW_AT_type
	.byte	4                               // Abbrev [4] 0xae:0x7 DW_TAG_base_type
	.long	.Linfo_string13                 // DW_AT_name
	.byte	4                               // DW_AT_encoding
	.byte	8                               // DW_AT_byte_size
	.byte	8                               // Abbrev [8] 0xb5:0x5 DW_TAG_pointer_type
	.long	186                             // DW_AT_type
	.byte	9                               // Abbrev [9] 0xba:0x1 DW_TAG_const_type
	.byte	10                              // Abbrev [10] 0xbb:0x2a DW_TAG_subprogram
	.long	.Linfo_string14                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	33                              // DW_AT_decl_line
                                        // DW_AT_prototyped
	.byte	1                               // DW_AT_inline
	.byte	11                              // Abbrev [11] 0xc3:0xb DW_TAG_formal_parameter
	.long	.Linfo_string15                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	33                              // DW_AT_decl_line
	.long	181                             // DW_AT_type
	.byte	11                              // Abbrev [11] 0xce:0xb DW_TAG_formal_parameter
	.long	.Linfo_string16                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	33                              // DW_AT_decl_line
	.long	168                             // DW_AT_type
	.byte	11                              // Abbrev [11] 0xd9:0xb DW_TAG_formal_parameter
	.long	.Linfo_string17                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	33                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	0                               // End Of Children Mark
	.byte	12                              // Abbrev [12] 0xe5:0x13d DW_TAG_subprogram
	.long	.Lfunc_begin0                   // DW_AT_low_pc
	.long	.Lfunc_end0-.Lfunc_begin0       // DW_AT_high_pc
	.byte	1                               // DW_AT_frame_base
	.byte	102
                                        // DW_AT_GNU_all_call_sites
	.long	.Linfo_string18                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	16                              // DW_AT_decl_line
	.long	546                             // DW_AT_type
                                        // DW_AT_external
	.byte	13                              // Abbrev [13] 0xfa:0xf DW_TAG_variable
	.long	.Ldebug_loc0                    // DW_AT_location
	.long	.Linfo_string20                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	30                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	13                              // Abbrev [13] 0x109:0xf DW_TAG_variable
	.long	.Ldebug_loc1                    // DW_AT_location
	.long	.Linfo_string21                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	22                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	13                              // Abbrev [13] 0x118:0xf DW_TAG_variable
	.long	.Ldebug_loc2                    // DW_AT_location
	.long	.Linfo_string8                  // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	20                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	13                              // Abbrev [13] 0x127:0xf DW_TAG_variable
	.long	.Ldebug_loc3                    // DW_AT_location
	.long	.Linfo_string7                  // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	17                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	13                              // Abbrev [13] 0x136:0xf DW_TAG_variable
	.long	.Ldebug_loc4                    // DW_AT_location
	.long	.Linfo_string11                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	19                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	13                              // Abbrev [13] 0x145:0xf DW_TAG_variable
	.long	.Ldebug_loc5                    // DW_AT_location
	.long	.Linfo_string22                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	25                              // DW_AT_decl_line
	.long	169                             // DW_AT_type
	.byte	13                              // Abbrev [13] 0x154:0xf DW_TAG_variable
	.long	.Ldebug_loc6                    // DW_AT_location
	.long	.Linfo_string23                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	26                              // DW_AT_decl_line
	.long	169                             // DW_AT_type
	.byte	13                              // Abbrev [13] 0x163:0xf DW_TAG_variable
	.long	.Ldebug_loc7                    // DW_AT_location
	.long	.Linfo_string24                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	27                              // DW_AT_decl_line
	.long	169                             // DW_AT_type
	.byte	14                              // Abbrev [14] 0x172:0xb DW_TAG_variable
	.long	.Linfo_string27                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	21                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	14                              // Abbrev [14] 0x17d:0xb DW_TAG_variable
	.long	.Linfo_string28                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	29                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	15                              // Abbrev [15] 0x188:0x84 DW_TAG_lexical_block
	.long	.Ltmp8                          // DW_AT_low_pc
	.long	.Ltmp20-.Ltmp8                  // DW_AT_high_pc
	.byte	13                              // Abbrev [13] 0x191:0xf DW_TAG_variable
	.long	.Ldebug_loc8                    // DW_AT_location
	.long	.Linfo_string25                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	32                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	16                              // Abbrev [16] 0x1a0:0x2c DW_TAG_inlined_subroutine
	.long	187                             // DW_AT_abstract_origin
	.long	.Ltmp14                         // DW_AT_low_pc
	.long	.Ltmp16-.Ltmp14                 // DW_AT_high_pc
	.byte	2                               // DW_AT_call_file
	.byte	33                              // DW_AT_call_line
	.byte	5                               // DW_AT_call_column
	.byte	17                              // Abbrev [17] 0x1b0:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc9                    // DW_AT_location
	.long	195                             // DW_AT_abstract_origin
	.byte	18                              // Abbrev [18] 0x1b9:0x7 DW_TAG_formal_parameter
	.byte	1                               // DW_AT_location
	.byte	95
	.long	206                             // DW_AT_abstract_origin
	.byte	18                              // Abbrev [18] 0x1c0:0xb DW_TAG_formal_parameter
	.byte	5                               // DW_AT_location
	.byte	129
	.byte	0
	.byte	51
	.byte	36
	.byte	159
	.long	217                             // DW_AT_abstract_origin
	.byte	0                               // End Of Children Mark
	.byte	16                              // Abbrev [16] 0x1cc:0x2a DW_TAG_inlined_subroutine
	.long	187                             // DW_AT_abstract_origin
	.long	.Ltmp16                         // DW_AT_low_pc
	.long	.Ltmp17-.Ltmp16                 // DW_AT_high_pc
	.byte	2                               // DW_AT_call_file
	.byte	35                              // DW_AT_call_line
	.byte	5                               // DW_AT_call_column
	.byte	18                              // Abbrev [18] 0x1dc:0x7 DW_TAG_formal_parameter
	.byte	1                               // DW_AT_location
	.byte	81
	.long	195                             // DW_AT_abstract_origin
	.byte	18                              // Abbrev [18] 0x1e3:0x7 DW_TAG_formal_parameter
	.byte	1                               // DW_AT_location
	.byte	96
	.long	206                             // DW_AT_abstract_origin
	.byte	18                              // Abbrev [18] 0x1ea:0xb DW_TAG_formal_parameter
	.byte	5                               // DW_AT_location
	.byte	129
	.byte	0
	.byte	51
	.byte	36
	.byte	159
	.long	217                             // DW_AT_abstract_origin
	.byte	0                               // End Of Children Mark
	.byte	15                              // Abbrev [15] 0x1f6:0x15 DW_TAG_lexical_block
	.long	.Ltmp17                         // DW_AT_low_pc
	.long	.Ltmp20-.Ltmp17                 // DW_AT_high_pc
	.byte	14                              // Abbrev [14] 0x1ff:0xb DW_TAG_variable
	.long	.Linfo_string26                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	37                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	0                               // End Of Children Mark
	.byte	0                               // End Of Children Mark
	.byte	19                              // Abbrev [19] 0x20c:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp4                          // DW_AT_low_pc
	.byte	19                              // Abbrev [19] 0x213:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp6                          // DW_AT_low_pc
	.byte	19                              // Abbrev [19] 0x21a:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp8                          // DW_AT_low_pc
	.byte	0                               // End Of Children Mark
	.byte	4                               // Abbrev [4] 0x222:0x7 DW_TAG_base_type
	.long	.Linfo_string19                 // DW_AT_name
	.byte	5                               // DW_AT_encoding
	.byte	4                               // DW_AT_byte_size
	.byte	0                               // End Of Children Mark
.Ldebug_info_end0:
	.section	.debug_str,"MS",@progbits,1
.Linfo_string0:
	.asciz	"clang version 12.0.0 (https://github.com/upmem/llvm-project.git 846fdda8285dcc9b20ee5d2fec9e54dfea6a8928)" // string offset=0
.Linfo_string1:
	.asciz	"dpu/task.c"                    // string offset=106
.Linfo_string2:
	.asciz	"/home/hamid.farzaneh/CostModelEvaluationRepo/LUT" // string offset=117
.Linfo_string3:
	.asciz	"nb_cycle"                      // string offset=166
.Linfo_string4:
	.asciz	"unsigned int"                  // string offset=175
.Linfo_string5:
	.asciz	"uint32_t"                      // string offset=188
.Linfo_string6:
	.asciz	"DPU_INPUT_ARGUMENTS"           // string offset=197
.Linfo_string7:
	.asciz	"ITER"                          // string offset=217
.Linfo_string8:
	.asciz	"buffer_size"                   // string offset=222
.Linfo_string9:
	.asciz	"thread_count"                  // string offset=234
.Linfo_string10:
	.asciz	"read"                          // string offset=247
.Linfo_string11:
	.asciz	"arr_size"                      // string offset=252
.Linfo_string12:
	.asciz	"dpu_arguments_t"               // string offset=261
.Linfo_string13:
	.asciz	"double"                        // string offset=277
.Linfo_string14:
	.asciz	"mram_read"                     // string offset=284
.Linfo_string15:
	.asciz	"from"                          // string offset=294
.Linfo_string16:
	.asciz	"to"                            // string offset=299
.Linfo_string17:
	.asciz	"nb_of_bytes"                   // string offset=302
.Linfo_string18:
	.asciz	"main"                          // string offset=314
.Linfo_string19:
	.asciz	"int"                           // string offset=319
.Linfo_string20:
	.asciz	"b_offset"                      // string offset=323
.Linfo_string21:
	.asciz	"mram_base_addr_B"              // string offset=332
.Linfo_string22:
	.asciz	"cache_A"                       // string offset=349
.Linfo_string23:
	.asciz	"cache_B"                       // string offset=357
.Linfo_string24:
	.asciz	"cache_C"                       // string offset=365
.Linfo_string25:
	.asciz	"i"                             // string offset=373
.Linfo_string26:
	.asciz	"j"                             // string offset=375
.Linfo_string27:
	.asciz	"mram_base_addr_A"              // string offset=377
.Linfo_string28:
	.asciz	"a_offset"                      // string offset=394
	.addrsig
	.addrsig_sym DPU_INPUT_ARGUMENTS
	.addrsig_sym __sys_used_mram_end
	.addrsig_sym nb_cycle
	.section	.debug_line,"",@progbits
.Lline_table_start0:
