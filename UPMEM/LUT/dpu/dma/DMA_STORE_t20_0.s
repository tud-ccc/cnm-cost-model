	.text
	.file	"task.c"
	.file	1 "/opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/stdlib" "stdint.h"
	.file	2 "/home/hamid.farzaneh/CostModelEvaluationRepo/LUT/LUTFuncOperationRange" "./dpu/task.c"
	.file	3 "/home/hamid.farzaneh/CostModelEvaluationRepo/LUT/LUTFuncOperationRange" "./dpu/../support/common.h"
	.section	.text.main,"ax",@progbits
	.globl	main                            // -- Begin function main
	.type	main,@function
main:                                   // @main
.Lfunc_begin0:
	.loc	2 17 0                          // ./dpu/task.c:17:0
	.cfi_sections .debug_frame
	.cfi_startproc
// %bb.0:
	.cfi_def_cfa_offset -24
	.cfi_offset 23, -8
	.cfi_offset 22, -4
	sd r22, 16, d22
	add r22, r22, 24
	.cfi_offset 15, -16
	.cfi_offset 14, -12
.Ltmp0:
	sd r22, -16, d14
	.cfi_offset 17, -24
	.cfi_offset 16, -20
	sd r22, -24, d16
	.loc	2 18 42 prologue_end            // ./dpu/task.c:18:42
	move r0, DPU_INPUT_ARGUMENTS
.Ltmp1:
	//DEBUG_VALUE: main:arr_size <- undef
	.loc	2 20 48                         // ./dpu/task.c:20:48
	lw r16, r0, 4
.Ltmp2:
	//DEBUG_VALUE: main:mram_base_addr_B <- undef
	//DEBUG_VALUE: main:buffer_size <- $r16
	.loc	2 18 42                         // ./dpu/task.c:18:42
	lw r17, zero, DPU_INPUT_ARGUMENTS
.Ltmp3:
	//DEBUG_VALUE: main:ITER <- $r17
	.loc	2 25 43                         // ./dpu/task.c:25:43
	lsl r15, r16, 3
	.loc	2 25 21 is_stmt 0               // ./dpu/task.c:25:21
	move r0, r15
	call r23, mem_alloc
.Ltmp4:
	move r14, r0
.Ltmp5:
	//DEBUG_VALUE: main:cache_A <- $r14
	.loc	2 26 21 is_stmt 1               // ./dpu/task.c:26:21
	move r0, r15
	call r23, mem_alloc
.Ltmp6:
	//DEBUG_VALUE: main:cache_B <- undef
	.loc	2 27 21                         // ./dpu/task.c:27:21
	move r0, r15
	call r23, mem_alloc
.Ltmp7:
	//DEBUG_VALUE: x <- 0
	//DEBUG_VALUE: main:b_offset <- undef
	//DEBUG_VALUE: main:cache_C <- undef
	.loc	2 53 5                          // ./dpu/task.c:53:5
	jeq r17, 0, .LBB0_3
.Ltmp8:
// %bb.1:
	//DEBUG_VALUE: main:ITER <- $r17
	//DEBUG_VALUE: main:buffer_size <- $r16
	//DEBUG_VALUE: main:cache_A <- $r14
	//DEBUG_VALUE: x <- 0
	.loc	2 0 5 is_stmt 0                 // ./dpu/task.c:0:5
	move r1, __sys_used_mram_end
.Ltmp9:
.LBB0_2:                                // =>This Inner Loop Header: Depth=1
	//DEBUG_VALUE: main:buffer_size <- $r16
	//DEBUG_VALUE: main:cache_A <- $r14
	//DEBUG_VALUE: x <- undef
	move r0, -1
.Ltmp10:
	//DEBUG_VALUE: mram_write:nb_of_bytes <- $r16
	//DEBUG_VALUE: mram_write:from <- $r14
	.file	4 "/opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib" "mram.h"
	.loc	4 61 24 is_stmt 1               // /opt/upmem/upmem-2023.2.0-Linux-x86_64/bin/../share/upmem/include/syslib/mram.h:61:24
	lsr_add r0, r0, r16, 3
	lsl_add r0, r14, r0, 24
.Ltmp11:
	//DEBUG_VALUE: mram_write:to <- $r1
.Ltmp12:
	//DEBUG_VALUE: x <- [DW_OP_plus_uconst 1, DW_OP_stack_value] undef
	.loc	2 53 23                         // ./dpu/task.c:53:23
	add r17, r17, -1, nz, .LBB0_2
.Ltmp13:
.LBB0_3:
	//DEBUG_VALUE: main:buffer_size <- $r16
	//DEBUG_VALUE: main:cache_A <- $r14
	.loc	2 0 23 is_stmt 0                // ./dpu/task.c:0:23
	move r0, 0
	.loc	2 58 3 is_stmt 1                // ./dpu/task.c:58:3
	ld d16, r22, -24
.Ltmp14:
	ld d14, r22, -16
.Ltmp15:
	ld d22, r22, -8
	jump r23
.Ltmp16:
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
	.section	.stack_sizes,"o",@progbits,.text.main
	.long	.Lfunc_begin0
	.byte	24
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
	.long	.Ltmp2-.Lfunc_begin0
	.long	.Ltmp14-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	96                              // DW_OP_reg16
	.long	0
	.long	0
.Ldebug_loc1:
	.long	.Ltmp3-.Lfunc_begin0
	.long	.Ltmp9-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	97                              // DW_OP_reg17
	.long	0
	.long	0
.Ldebug_loc2:
	.long	.Ltmp5-.Lfunc_begin0
	.long	.Ltmp15-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	94                              // DW_OP_reg14
	.long	0
	.long	0
.Ldebug_loc3:
	.long	.Ltmp7-.Lfunc_begin0
	.long	.Ltmp9-.Lfunc_begin0
	.short	3                               // Loc expr size
	.byte	17                              // DW_OP_consts
	.byte	0                               // 0
	.byte	159                             // DW_OP_stack_value
	.long	0
	.long	0
.Ldebug_loc4:
	.long	.Ltmp11-.Lfunc_begin0
	.long	.Ltmp13-.Lfunc_begin0
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
	.byte	10                              // Abbreviation Code
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
	.byte	11                              // Abbreviation Code
	.byte	38                              // DW_TAG_const_type
	.byte	0                               // DW_CHILDREN_no
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
	.byte	24                              // DW_FORM_exprloc
	.byte	49                              // DW_AT_abstract_origin
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	18                              // Abbreviation Code
	.byte	5                               // DW_TAG_formal_parameter
	.byte	0                               // DW_CHILDREN_no
	.byte	2                               // DW_AT_location
	.byte	23                              // DW_FORM_sec_offset
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
	.byte	1                               // Abbrev [1] 0xb:0x1c1 DW_TAG_compile_unit
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
	.byte	13                              // DW_AT_decl_line
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
	.byte	15                              // DW_AT_decl_line
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
	.long	56                              // DW_AT_type
	.byte	9                               // Abbrev [9] 0xae:0x2a DW_TAG_subprogram
	.long	.Linfo_string13                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	55                              // DW_AT_decl_line
                                        // DW_AT_prototyped
	.byte	1                               // DW_AT_inline
	.byte	10                              // Abbrev [10] 0xb6:0xb DW_TAG_formal_parameter
	.long	.Linfo_string14                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	55                              // DW_AT_decl_line
	.long	216                             // DW_AT_type
	.byte	10                              // Abbrev [10] 0xc1:0xb DW_TAG_formal_parameter
	.long	.Linfo_string15                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	55                              // DW_AT_decl_line
	.long	168                             // DW_AT_type
	.byte	10                              // Abbrev [10] 0xcc:0xb DW_TAG_formal_parameter
	.long	.Linfo_string16                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	55                              // DW_AT_decl_line
	.long	67                              // DW_AT_type
	.byte	0                               // End Of Children Mark
	.byte	8                               // Abbrev [8] 0xd8:0x5 DW_TAG_pointer_type
	.long	221                             // DW_AT_type
	.byte	11                              // Abbrev [11] 0xdd:0x1 DW_TAG_const_type
	.byte	12                              // Abbrev [12] 0xde:0xe6 DW_TAG_subprogram
	.long	.Lfunc_begin0                   // DW_AT_low_pc
	.long	.Lfunc_end0-.Lfunc_begin0       // DW_AT_high_pc
	.byte	1                               // DW_AT_frame_base
	.byte	102
                                        // DW_AT_GNU_all_call_sites
	.long	.Linfo_string17                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	17                              // DW_AT_decl_line
	.long	452                             // DW_AT_type
                                        // DW_AT_external
	.byte	13                              // Abbrev [13] 0xf3:0xb DW_TAG_variable
	.long	.Linfo_string11                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	19                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	13                              // Abbrev [13] 0xfe:0xb DW_TAG_variable
	.long	.Linfo_string19                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	23                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	14                              // Abbrev [14] 0x109:0xf DW_TAG_variable
	.long	.Ldebug_loc0                    // DW_AT_location
	.long	.Linfo_string8                  // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	20                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	14                              // Abbrev [14] 0x118:0xf DW_TAG_variable
	.long	.Ldebug_loc1                    // DW_AT_location
	.long	.Linfo_string7                  // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	18                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	14                              // Abbrev [14] 0x127:0xf DW_TAG_variable
	.long	.Ldebug_loc2                    // DW_AT_location
	.long	.Linfo_string20                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	25                              // DW_AT_decl_line
	.long	169                             // DW_AT_type
	.byte	13                              // Abbrev [13] 0x136:0xb DW_TAG_variable
	.long	.Linfo_string21                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	26                              // DW_AT_decl_line
	.long	169                             // DW_AT_type
	.byte	13                              // Abbrev [13] 0x141:0xb DW_TAG_variable
	.long	.Linfo_string23                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	30                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	13                              // Abbrev [13] 0x14c:0xb DW_TAG_variable
	.long	.Linfo_string24                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	27                              // DW_AT_decl_line
	.long	169                             // DW_AT_type
	.byte	13                              // Abbrev [13] 0x157:0xb DW_TAG_variable
	.long	.Linfo_string25                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	22                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	13                              // Abbrev [13] 0x162:0xb DW_TAG_variable
	.long	.Linfo_string26                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	29                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	15                              // Abbrev [15] 0x16d:0x41 DW_TAG_lexical_block
	.long	.Ltmp7                          // DW_AT_low_pc
	.long	.Ltmp13-.Ltmp7                  // DW_AT_high_pc
	.byte	14                              // Abbrev [14] 0x176:0xf DW_TAG_variable
	.long	.Ldebug_loc3                    // DW_AT_location
	.long	.Linfo_string22                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	53                              // DW_AT_decl_line
	.long	452                             // DW_AT_type
	.byte	16                              // Abbrev [16] 0x185:0x28 DW_TAG_inlined_subroutine
	.long	174                             // DW_AT_abstract_origin
	.long	.Ltmp10                         // DW_AT_low_pc
	.long	.Ltmp12-.Ltmp10                 // DW_AT_high_pc
	.byte	2                               // DW_AT_call_file
	.byte	54                              // DW_AT_call_line
	.byte	9                               // DW_AT_call_column
	.byte	17                              // Abbrev [17] 0x195:0x7 DW_TAG_formal_parameter
	.byte	1                               // DW_AT_location
	.byte	94
	.long	182                             // DW_AT_abstract_origin
	.byte	18                              // Abbrev [18] 0x19c:0x9 DW_TAG_formal_parameter
	.long	.Ldebug_loc4                    // DW_AT_location
	.long	193                             // DW_AT_abstract_origin
	.byte	17                              // Abbrev [17] 0x1a5:0x7 DW_TAG_formal_parameter
	.byte	1                               // DW_AT_location
	.byte	96
	.long	204                             // DW_AT_abstract_origin
	.byte	0                               // End Of Children Mark
	.byte	0                               // End Of Children Mark
	.byte	19                              // Abbrev [19] 0x1ae:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp4                          // DW_AT_low_pc
	.byte	19                              // Abbrev [19] 0x1b5:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp6                          // DW_AT_low_pc
	.byte	19                              // Abbrev [19] 0x1bc:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp7                          // DW_AT_low_pc
	.byte	0                               // End Of Children Mark
	.byte	4                               // Abbrev [4] 0x1c4:0x7 DW_TAG_base_type
	.long	.Linfo_string18                 // DW_AT_name
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
	.asciz	"/home/hamid.farzaneh/CostModelEvaluationRepo/LUT/LUTFuncOperationRange" // string offset=117
.Linfo_string3:
	.asciz	"nb_cycle"                      // string offset=188
.Linfo_string4:
	.asciz	"unsigned int"                  // string offset=197
.Linfo_string5:
	.asciz	"uint32_t"                      // string offset=210
.Linfo_string6:
	.asciz	"DPU_INPUT_ARGUMENTS"           // string offset=219
.Linfo_string7:
	.asciz	"ITER"                          // string offset=239
.Linfo_string8:
	.asciz	"buffer_size"                   // string offset=244
.Linfo_string9:
	.asciz	"thread_count"                  // string offset=256
.Linfo_string10:
	.asciz	"read"                          // string offset=269
.Linfo_string11:
	.asciz	"arr_size"                      // string offset=274
.Linfo_string12:
	.asciz	"dpu_arguments_t"               // string offset=283
.Linfo_string13:
	.asciz	"mram_write"                    // string offset=299
.Linfo_string14:
	.asciz	"from"                          // string offset=310
.Linfo_string15:
	.asciz	"to"                            // string offset=315
.Linfo_string16:
	.asciz	"nb_of_bytes"                   // string offset=318
.Linfo_string17:
	.asciz	"main"                          // string offset=330
.Linfo_string18:
	.asciz	"int"                           // string offset=335
.Linfo_string19:
	.asciz	"mram_base_addr_B"              // string offset=339
.Linfo_string20:
	.asciz	"cache_A"                       // string offset=356
.Linfo_string21:
	.asciz	"cache_B"                       // string offset=364
.Linfo_string22:
	.asciz	"x"                             // string offset=372
.Linfo_string23:
	.asciz	"b_offset"                      // string offset=374
.Linfo_string24:
	.asciz	"cache_C"                       // string offset=383
.Linfo_string25:
	.asciz	"mram_base_addr_A"              // string offset=391
.Linfo_string26:
	.asciz	"a_offset"                      // string offset=408
	.addrsig
	.addrsig_sym DPU_INPUT_ARGUMENTS
	.addrsig_sym __sys_used_mram_end
	.addrsig_sym nb_cycle
	.section	.debug_line,"",@progbits
.Lline_table_start0:
