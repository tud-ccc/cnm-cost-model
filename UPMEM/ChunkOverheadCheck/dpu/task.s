	.text
	.file	"task.c"
	.file	1 "/opt/upmem/upmem-2025.1.0-Linux-x86_64/bin/../share/upmem/include/stdlib" "stdint.h"
	.file	2 "/home/hamid.farzaneh/CostModelEvaluationRepo/NewEval/ChunkOverheadCheck" "dpu/task.c"
	.file	3 "/opt/upmem/upmem-2025.1.0-Linux-x86_64/bin/../share/upmem/include/syslib" "perfcounter.h"
	.section	.text.main,"ax",@progbits
	.globl	main                            // -- Begin function main
	.type	main,@function
main:                                   // @main
.Lfunc_begin0:
	.loc	2 19 0                          // dpu/task.c:19:0
	.cfi_sections .debug_frame
	.cfi_startproc
// %bb.0:
	.cfi_def_cfa_offset -56
	.cfi_offset 23, -8
	.cfi_offset 22, -4
	sd r22, 48, d22
	add r22, r22, 56
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
	.file	4 "/opt/upmem/upmem-2025.1.0-Linux-x86_64/bin/../share/upmem/include/syslib" "defs.h"
	.loc	4 35 12 prologue_end            // /opt/upmem/upmem-2025.1.0-Linux-x86_64/bin/../share/upmem/include/syslib/defs.h:35:12
	move r16, id, nz, .LBB0_2
.Ltmp1:
// %bb.1:
	.loc	4 0 12 is_stmt 0                // /opt/upmem/upmem-2025.1.0-Linux-x86_64/bin/../share/upmem/include/syslib/defs.h:0:12
	move r0, 1
.Ltmp2:
	.loc	2 21 5 is_stmt 1                // dpu/task.c:21:5
	move r1, r0
	call r23, perfcounter_config
.Ltmp3:
.LBB0_2:
	.loc	2 30 21                         // dpu/task.c:30:21
	lw r0, zero, .L__const.main.arr
	move r1, .L__const.main.arr
	sw r22, -32, r0
	lw r0, r1, 4
	sw r22, -28, r0
	lw r0, r1, 8
	sw r22, -24, r0
	lw r0, r1, 12
	sw r22, -20, r0
.Ltmp4:
	//DEBUG_VALUE: main:acc3 <- 3
	//DEBUG_VALUE: main:acc2 <- 2
	//DEBUG_VALUE: main:acc1 <- 1
	.loc	2 32 21                         // dpu/task.c:32:21
	sw r22, -16, 0
	move r18, 3
	move r17, 100000
	.loc	2 34 20                         // dpu/task.c:34:20
	call r23, perfcounter_get
.Ltmp5:
	movd d14, d0
	//DEBUG_VALUE: i <- 0
.Ltmp6:
.LBB0_3:                                // =>This Inner Loop Header: Depth=1
	//DEBUG_VALUE: main:acc1 <- 1
	//DEBUG_VALUE: main:acc2 <- 2
	//DEBUG_VALUE: main:acc3 <- $r18
	//DEBUG_VALUE: i <- undef
	//DEBUG_VALUE: t0 <- $r18
	//DEBUG_VALUE: t1 <- undef
	//DEBUG_VALUE: t2 <- [DW_OP_plus_uconst 2, DW_OP_stack_value] undef
	//DEBUG_VALUE: t3 <- undef
	.loc	2 44 19                         // dpu/task.c:44:19
	lw r0, r22, -24
.Ltmp7:
	//DEBUG_VALUE: v2 <- $r0
	.loc	2 45 19                         // dpu/task.c:45:19
	lw r1, r22, -20
.Ltmp8:
	//DEBUG_VALUE: v3 <- $r1
	.loc	2 46 19                         // dpu/task.c:46:19
	lw r2, r22, -32
.Ltmp9:
	//DEBUG_VALUE: v0 <- $r2
	.loc	2 47 19                         // dpu/task.c:47:19
	lw r3, r22, -28
.Ltmp10:
	//DEBUG_VALUE: v1 <- $r3
	.loc	2 41 22                         // dpu/task.c:41:22
	lsr_add r0, r0, r18, 1
.Ltmp11:
	.loc	2 48 15                         // dpu/task.c:48:15
	add r0, r0, r1
	.loc	2 48 20 is_stmt 0               // dpu/task.c:48:20
	add r0, r0, r2
	.loc	2 48 25                         // dpu/task.c:48:25
	add r0, r0, r3
	.loc	2 48 30                         // dpu/task.c:48:30
	add r0, r0, 3
	.loc	2 48 10                         // dpu/task.c:48:10
	sw r22, -16, r0
	.loc	2 49 12 is_stmt 1               // dpu/task.c:49:12
	lw r18, r22, -16
.Ltmp12:
	//DEBUG_VALUE: i <- [DW_OP_plus_uconst 1, DW_OP_stack_value] undef
	//DEBUG_VALUE: main:acc3 <- $r18
	.loc	2 35 26                         // dpu/task.c:35:26
	add r17, r17, -1, nz, .LBB0_3
.Ltmp13:
// %bb.4:
	//DEBUG_VALUE: main:acc3 <- $r18
	//DEBUG_VALUE: main:acc1 <- 1
	//DEBUG_VALUE: main:acc2 <- 2
	.loc	2 51 18                         // dpu/task.c:51:18
	call r23, perfcounter_get
.Ltmp14:
	//DEBUG_VALUE: main:end <- [DW_OP_LLVM_convert 64 7, DW_OP_LLVM_convert 32 7, DW_OP_stack_value] $d0
	.loc	2 53 7                          // dpu/task.c:53:7
	jneq r16, 0, .LBB0_6
.Ltmp15:
// %bb.5:
	//DEBUG_VALUE: main:acc3 <- $r18
	//DEBUG_VALUE: main:end <- [DW_OP_LLVM_convert 64 7, DW_OP_LLVM_convert 32 7, DW_OP_stack_value] $d0
	//DEBUG_VALUE: main:acc1 <- 1
	//DEBUG_VALUE: main:acc2 <- 2
	//DEBUG_VALUE: main:end <- $r1
	//DEBUG_VALUE: main:start <- $r15
	.loc	2 54 20                         // dpu/task.c:54:20
	sub r0, r1, r15
	.loc	2 54 14 is_stmt 0               // dpu/task.c:54:14
	sw zero, nb_cycle, r0
	.loc	2 55 16 is_stmt 1               // dpu/task.c:55:16
	lw r0, r22, -16
	.loc	2 55 14 is_stmt 0               // dpu/task.c:55:14
	sw zero, sink_out, r0
.Ltmp16:
.LBB0_6:
	//DEBUG_VALUE: main:acc3 <- $r18
	//DEBUG_VALUE: main:acc1 <- 1
	//DEBUG_VALUE: main:acc2 <- 2
	.loc	2 0 14                          // dpu/task.c:0:14
	move r0, 0
	.loc	2 57 3 is_stmt 1                // dpu/task.c:57:3
	ld d18, r22, -56
.Ltmp17:
	ld d16, r22, -48
	ld d14, r22, -40
	ld d22, r22, -8
	jump r23
.Ltmp18:
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
	.file	5 "/opt/upmem/upmem-2025.1.0-Linux-x86_64/bin/../share/upmem/include/syslib" "sysdef.h"
	.section	.stack_sizes,"o",@progbits,.text.main
	.long	.Lfunc_begin0
	.byte	56
	.section	.text.main,"ax",@progbits
                                        // -- End function
	.type	.L__const.main.arr,@object      // @__const.main.arr
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	2
.L__const.main.arr:
	.long	56                              // 0x38
	.long	52                              // 0x34
	.long	48                              // 0x30
	.long	44                              // 0x2c
	.size	.L__const.main.arr, 16

	.type	nb_cycle,@object                // @nb_cycle
	.section	.dpu_host,"aw",@progbits
	.globl	nb_cycle
	.p2align	3
nb_cycle:
	.long	0                               // 0x0
	.size	nb_cycle, 4

	.type	sink_out,@object                // @sink_out
	.globl	sink_out
	.p2align	3
sink_out:
	.long	0                               // 0x0
	.size	sink_out, 4

	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
	.long	.Ltmp4-.Lfunc_begin0
	.long	.Ltmp6-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	51                              // DW_OP_lit3
	.byte	159                             // DW_OP_stack_value
	.long	.Ltmp6-.Lfunc_begin0
	.long	.Ltmp17-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	98                              // DW_OP_reg18
	.long	0
	.long	0
.Ldebug_loc1:
	.long	.Ltmp4-.Lfunc_begin0
	.long	.Lfunc_end0-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	50                              // DW_OP_lit2
	.byte	159                             // DW_OP_stack_value
	.long	0
	.long	0
.Ldebug_loc2:
	.long	.Ltmp4-.Lfunc_begin0
	.long	.Lfunc_end0-.Lfunc_begin0
	.short	2                               // Loc expr size
	.byte	49                              // DW_OP_lit1
	.byte	159                             // DW_OP_stack_value
	.long	0
	.long	0
.Ldebug_loc3:
	.long	.Ltmp7-.Lfunc_begin0
	.long	.Ltmp11-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	80                              // DW_OP_reg0
	.long	0
	.long	0
.Ldebug_loc4:
	.long	.Ltmp8-.Lfunc_begin0
	.long	.Ltmp13-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	0
	.long	0
.Ldebug_loc5:
	.long	.Ltmp9-.Lfunc_begin0
	.long	.Ltmp13-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	82                              // DW_OP_reg2
	.long	0
	.long	0
.Ldebug_loc6:
	.long	.Ltmp10-.Lfunc_begin0
	.long	.Ltmp13-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	83                              // DW_OP_reg3
	.long	0
	.long	0
.Ldebug_loc7:
	.long	.Ltmp15-.Lfunc_begin0
	.long	.Ltmp16-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	81                              // DW_OP_reg1
	.long	0
	.long	0
.Ldebug_loc8:
	.long	.Ltmp15-.Lfunc_begin0
	.long	.Ltmp16-.Lfunc_begin0
	.short	1                               // Loc expr size
	.byte	95                              // DW_OP_reg15
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
	.byte	4                               // DW_TAG_enumeration_type
	.byte	1                               // DW_CHILDREN_yes
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	3                               // DW_AT_name
	.byte	14                              // DW_FORM_strp
	.byte	11                              // DW_AT_byte_size
	.byte	11                              // DW_FORM_data1
	.byte	58                              // DW_AT_decl_file
	.byte	11                              // DW_FORM_data1
	.byte	59                              // DW_AT_decl_line
	.byte	11                              // DW_FORM_data1
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	6                               // Abbreviation Code
	.byte	40                              // DW_TAG_enumerator
	.byte	0                               // DW_CHILDREN_no
	.byte	3                               // DW_AT_name
	.byte	14                              // DW_FORM_strp
	.byte	28                              // DW_AT_const_value
	.byte	15                              // DW_FORM_udata
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	7                               // Abbreviation Code
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
	.byte	8                               // Abbreviation Code
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
	.byte	9                               // Abbreviation Code
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
	.byte	10                              // Abbreviation Code
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
	.byte	11                              // Abbreviation Code
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
	.byte	12                              // Abbreviation Code
	.byte	11                              // DW_TAG_lexical_block
	.byte	1                               // DW_CHILDREN_yes
	.byte	17                              // DW_AT_low_pc
	.byte	1                               // DW_FORM_addr
	.byte	18                              // DW_AT_high_pc
	.byte	6                               // DW_FORM_data4
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
	.ascii	"\211\202\001"                  // DW_TAG_GNU_call_site
	.byte	0                               // DW_CHILDREN_no
	.ascii	"\223B"                         // DW_AT_GNU_call_site_target
	.byte	24                              // DW_FORM_exprloc
	.byte	17                              // DW_AT_low_pc
	.byte	1                               // DW_FORM_addr
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	15                              // Abbreviation Code
	.byte	1                               // DW_TAG_array_type
	.byte	1                               // DW_CHILDREN_yes
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	16                              // Abbreviation Code
	.byte	33                              // DW_TAG_subrange_type
	.byte	0                               // DW_CHILDREN_no
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	55                              // DW_AT_count
	.byte	11                              // DW_FORM_data1
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	17                              // Abbreviation Code
	.byte	53                              // DW_TAG_volatile_type
	.byte	0                               // DW_CHILDREN_no
	.byte	73                              // DW_AT_type
	.byte	19                              // DW_FORM_ref4
	.byte	0                               // EOM(1)
	.byte	0                               // EOM(2)
	.byte	18                              // Abbreviation Code
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
	.byte	0                               // EOM(3)
	.section	.debug_info,"",@progbits
.Lcu_begin0:
	.long	.Ldebug_info_end0-.Ldebug_info_start0 // Length of Unit
.Ldebug_info_start0:
	.short	4                               // DWARF version number
	.long	.debug_abbrev                   // Offset Into Abbrev. Section
	.byte	4                               // Address Size (in bytes)
	.byte	1                               // Abbrev [1] 0xb:0x1f2 DW_TAG_compile_unit
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
	.byte	8                               // DW_AT_decl_line
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
	.long	56                              // DW_AT_type
                                        // DW_AT_external
	.byte	2                               // DW_AT_decl_file
	.byte	9                               // DW_AT_decl_line
	.byte	8                               // DW_AT_alignment
	.byte	5                               // DW_AT_location
	.byte	3
	.long	sink_out
	.byte	5                               // Abbrev [5] 0x5c:0x31 DW_TAG_enumeration_type
	.long	67                              // DW_AT_type
	.long	.Linfo_string13                 // DW_AT_name
	.byte	4                               // DW_AT_byte_size
	.byte	3                               // DW_AT_decl_file
	.byte	32                              // DW_AT_decl_line
	.byte	6                               // Abbrev [6] 0x68:0x6 DW_TAG_enumerator
	.long	.Linfo_string7                  // DW_AT_name
	.byte	0                               // DW_AT_const_value
	.byte	6                               // Abbrev [6] 0x6e:0x6 DW_TAG_enumerator
	.long	.Linfo_string8                  // DW_AT_name
	.byte	1                               // DW_AT_const_value
	.byte	6                               // Abbrev [6] 0x74:0x6 DW_TAG_enumerator
	.long	.Linfo_string9                  // DW_AT_name
	.byte	2                               // DW_AT_const_value
	.byte	6                               // Abbrev [6] 0x7a:0x6 DW_TAG_enumerator
	.long	.Linfo_string10                 // DW_AT_name
	.byte	3                               // DW_AT_const_value
	.byte	6                               // Abbrev [6] 0x80:0x6 DW_TAG_enumerator
	.long	.Linfo_string11                 // DW_AT_name
	.byte	6                               // DW_AT_const_value
	.byte	6                               // Abbrev [6] 0x86:0x6 DW_TAG_enumerator
	.long	.Linfo_string12                 // DW_AT_name
	.byte	7                               // DW_AT_const_value
	.byte	0                               // End Of Children Mark
	.byte	7                               // Abbrev [7] 0x8d:0xc DW_TAG_subprogram
	.long	.Linfo_string14                 // DW_AT_name
	.byte	4                               // DW_AT_decl_file
	.byte	33                              // DW_AT_decl_line
	.long	153                             // DW_AT_type
	.byte	1                               // DW_AT_inline
	.byte	3                               // Abbrev [3] 0x99:0xb DW_TAG_typedef
	.long	67                              // DW_AT_type
	.long	.Linfo_string15                 // DW_AT_name
	.byte	5                               // DW_AT_decl_file
	.byte	27                              // DW_AT_decl_line
	.byte	8                               // Abbrev [8] 0xa4:0x139 DW_TAG_subprogram
	.long	.Lfunc_begin0                   // DW_AT_low_pc
	.long	.Lfunc_end0-.Lfunc_begin0       // DW_AT_high_pc
	.byte	1                               // DW_AT_frame_base
	.byte	102
                                        // DW_AT_GNU_all_call_sites
	.long	.Linfo_string16                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	19                              // DW_AT_decl_line
	.long	477                             // DW_AT_type
                                        // DW_AT_external
	.byte	9                               // Abbrev [9] 0xb9:0x1c DW_TAG_variable
	.byte	16                              // DW_AT_location
	.byte	145
	.byte	96
	.byte	147
	.byte	4
	.byte	145
	.byte	100
	.byte	147
	.byte	4
	.byte	145
	.byte	104
	.byte	147
	.byte	4
	.byte	145
	.byte	108
	.byte	147
	.byte	4
	.long	.Linfo_string18                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	30                              // DW_AT_decl_line
	.long	484                             // DW_AT_type
	.byte	9                               // Abbrev [9] 0xd5:0xe DW_TAG_variable
	.byte	2                               // DW_AT_location
	.byte	145
	.byte	112
	.long	.Linfo_string20                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	32                              // DW_AT_decl_line
	.long	496                             // DW_AT_type
	.byte	10                              // Abbrev [10] 0xe3:0xf DW_TAG_variable
	.long	.Ldebug_loc0                    // DW_AT_location
	.long	.Linfo_string21                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	31                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	10                              // Abbrev [10] 0xf2:0xf DW_TAG_variable
	.long	.Ldebug_loc1                    // DW_AT_location
	.long	.Linfo_string22                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	31                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	10                              // Abbrev [10] 0x101:0xf DW_TAG_variable
	.long	.Ldebug_loc2                    // DW_AT_location
	.long	.Linfo_string23                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	31                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	10                              // Abbrev [10] 0x110:0xf DW_TAG_variable
	.long	.Ldebug_loc7                    // DW_AT_location
	.long	.Linfo_string33                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	51                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	10                              // Abbrev [10] 0x11f:0xf DW_TAG_variable
	.long	.Ldebug_loc8                    // DW_AT_location
	.long	.Linfo_string34                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	34                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	11                              // Abbrev [11] 0x12e:0x10 DW_TAG_inlined_subroutine
	.long	141                             // DW_AT_abstract_origin
	.long	.Ltmp0                          // DW_AT_low_pc
	.long	.Ltmp1-.Ltmp0                   // DW_AT_high_pc
	.byte	2                               // DW_AT_call_file
	.byte	20                              // DW_AT_call_line
	.byte	7                               // DW_AT_call_column
	.byte	12                              // Abbrev [12] 0x13e:0x89 DW_TAG_lexical_block
	.long	.Ltmp6                          // DW_AT_low_pc
	.long	.Ltmp13-.Ltmp6                  // DW_AT_high_pc
	.byte	13                              // Abbrev [13] 0x147:0xb DW_TAG_variable
	.long	.Linfo_string24                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	35                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	12                              // Abbrev [12] 0x152:0x74 DW_TAG_lexical_block
	.long	.Ltmp6                          // DW_AT_low_pc
	.long	.Ltmp12-.Ltmp6                  // DW_AT_high_pc
	.byte	9                               // Abbrev [9] 0x15b:0xd DW_TAG_variable
	.byte	1                               // DW_AT_location
	.byte	98
	.long	.Linfo_string25                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	38                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	13                              // Abbrev [13] 0x168:0xb DW_TAG_variable
	.long	.Linfo_string26                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	39                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	13                              // Abbrev [13] 0x173:0xb DW_TAG_variable
	.long	.Linfo_string27                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	40                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	13                              // Abbrev [13] 0x17e:0xb DW_TAG_variable
	.long	.Linfo_string28                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	41                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	10                              // Abbrev [10] 0x189:0xf DW_TAG_variable
	.long	.Ldebug_loc3                    // DW_AT_location
	.long	.Linfo_string29                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	44                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	10                              // Abbrev [10] 0x198:0xf DW_TAG_variable
	.long	.Ldebug_loc4                    // DW_AT_location
	.long	.Linfo_string30                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	45                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	10                              // Abbrev [10] 0x1a7:0xf DW_TAG_variable
	.long	.Ldebug_loc5                    // DW_AT_location
	.long	.Linfo_string31                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	46                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	10                              // Abbrev [10] 0x1b6:0xf DW_TAG_variable
	.long	.Ldebug_loc6                    // DW_AT_location
	.long	.Linfo_string32                 // DW_AT_name
	.byte	2                               // DW_AT_decl_file
	.byte	47                              // DW_AT_decl_line
	.long	56                              // DW_AT_type
	.byte	0                               // End Of Children Mark
	.byte	0                               // End Of Children Mark
	.byte	14                              // Abbrev [14] 0x1c7:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp3                          // DW_AT_low_pc
	.byte	14                              // Abbrev [14] 0x1ce:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp5                          // DW_AT_low_pc
	.byte	14                              // Abbrev [14] 0x1d5:0x7 DW_TAG_GNU_call_site
	.byte	1                               // DW_AT_GNU_call_site_target
	.byte	103
	.long	.Ltmp14                         // DW_AT_low_pc
	.byte	0                               // End Of Children Mark
	.byte	4                               // Abbrev [4] 0x1dd:0x7 DW_TAG_base_type
	.long	.Linfo_string17                 // DW_AT_name
	.byte	5                               // DW_AT_encoding
	.byte	4                               // DW_AT_byte_size
	.byte	15                              // Abbrev [15] 0x1e4:0xc DW_TAG_array_type
	.long	496                             // DW_AT_type
	.byte	16                              // Abbrev [16] 0x1e9:0x6 DW_TAG_subrange_type
	.long	501                             // DW_AT_type
	.byte	4                               // DW_AT_count
	.byte	0                               // End Of Children Mark
	.byte	17                              // Abbrev [17] 0x1f0:0x5 DW_TAG_volatile_type
	.long	56                              // DW_AT_type
	.byte	18                              // Abbrev [18] 0x1f5:0x7 DW_TAG_base_type
	.long	.Linfo_string19                 // DW_AT_name
	.byte	8                               // DW_AT_byte_size
	.byte	7                               // DW_AT_encoding
	.byte	0                               // End Of Children Mark
.Ldebug_info_end0:
	.section	.debug_str,"MS",@progbits,1
.Linfo_string0:
	.asciz	"clang version 12.0.0 (https://github.com/upmem/llvm-project.git 02a1f3ffa0f15d3c21b60ffc12196131c3e3cffa)" // string offset=0
.Linfo_string1:
	.asciz	"dpu/task.c"                    // string offset=106
.Linfo_string2:
	.asciz	"/home/hamid.farzaneh/CostModelEvaluationRepo/NewEval/ChunkOverheadCheck" // string offset=117
.Linfo_string3:
	.asciz	"nb_cycle"                      // string offset=189
.Linfo_string4:
	.asciz	"unsigned int"                  // string offset=198
.Linfo_string5:
	.asciz	"uint32_t"                      // string offset=211
.Linfo_string6:
	.asciz	"sink_out"                      // string offset=220
.Linfo_string7:
	.asciz	"COUNT_SAME"                    // string offset=229
.Linfo_string8:
	.asciz	"COUNT_CYCLES"                  // string offset=240
.Linfo_string9:
	.asciz	"COUNT_INSTRUCTIONS"            // string offset=253
.Linfo_string10:
	.asciz	"COUNT_NOTHING"                 // string offset=272
.Linfo_string11:
	.asciz	"COUNT_DISABLE_BOTH"            // string offset=286
.Linfo_string12:
	.asciz	"COUNT_ENABLE_BOTH"             // string offset=305
.Linfo_string13:
	.asciz	"_perfcounter_config_t"         // string offset=323
.Linfo_string14:
	.asciz	"me"                            // string offset=345
.Linfo_string15:
	.asciz	"sysname_t"                     // string offset=348
.Linfo_string16:
	.asciz	"main"                          // string offset=358
.Linfo_string17:
	.asciz	"int"                           // string offset=363
.Linfo_string18:
	.asciz	"arr"                           // string offset=367
.Linfo_string19:
	.asciz	"__ARRAY_SIZE_TYPE__"           // string offset=371
.Linfo_string20:
	.asciz	"sink"                          // string offset=391
.Linfo_string21:
	.asciz	"acc3"                          // string offset=396
.Linfo_string22:
	.asciz	"acc2"                          // string offset=401
.Linfo_string23:
	.asciz	"acc1"                          // string offset=406
.Linfo_string24:
	.asciz	"i"                             // string offset=411
.Linfo_string25:
	.asciz	"t0"                            // string offset=413
.Linfo_string26:
	.asciz	"t1"                            // string offset=416
.Linfo_string27:
	.asciz	"t2"                            // string offset=419
.Linfo_string28:
	.asciz	"t3"                            // string offset=422
.Linfo_string29:
	.asciz	"v2"                            // string offset=425
.Linfo_string30:
	.asciz	"v3"                            // string offset=428
.Linfo_string31:
	.asciz	"v0"                            // string offset=431
.Linfo_string32:
	.asciz	"v1"                            // string offset=434
.Linfo_string33:
	.asciz	"end"                           // string offset=437
.Linfo_string34:
	.asciz	"start"                         // string offset=441
	.addrsig
	.addrsig_sym nb_cycle
	.addrsig_sym sink_out
	.section	.debug_line,"",@progbits
.Lline_table_start0:
