.section .text
.include "ARMTestLib.inc"
.include "helpMethods.s"

.global _start
_start:

	mov R0, #5
	bl register_R0_basic_test
	cmp R0, #5
	bne R0_fault

	mov R0, #5
	bl register_R1_basic_test
	cmp R0, #5
	bne R1_fault

	mov R0, #5
	bl register_R2_basic_test
	cmp R0, #5
	bne R2_fault
	
	mov R0, #5
	bl register_R3_basic_test
	cmp R0, #5
	bne R3_fault	

	mov R0, #5
	bl register_R4_basic_test
	cmp R0, #5
	bne R4_fault

	mov R0, #5
	bl register_R5_basic_test
	cmp R0, #5
	bne R5_fault

	mov R0, #5
	bl register_R6_basic_test
	cmp R0, #5
	bne R6_fault

	mov R0, #5
	bl register_R7_basic_test
	cmp R0, #5
	bne R7_fault

	mov R0, #5
	bl register_R8_basic_test
	cmp R0, #5
	bne R8_fault

	mov R0, #5
	bl register_R9_basic_test
	cmp R0, #5
	bne R9_fault

	mov R0, #5
	bl register_R10_basic_test
	cmp R0, #5
	bne R10_fault

	mov R0, #5
	bl register_R11_basic_test
	cmp R0, #5
	bne R11_fault

	mov R0, #5
	bl register_R12_basic_test
	cmp R0, #5
	bne R12_fault

	mov R0, #5
	bl register_LR_basic_test
	cmp R0, #5
	bne LR_fault

	bl register_PC_basic_test
	cmp R0, #1
	bne PC_fault

	mov R0, #10
	bl register_SP_basic_test
	cmp R0, #10
	bne SP_fault

	@-------------------------------------------------

	mov R0, #5
	mov R1, #5
	bl Simple_EQ_test
	cmp R0, #1
	bne ZeroFlag_fail 

	mov R0, #4
	mov R1, #5
	bl Simple_NE_test
	cmp R0, #1
	bne ZeroFlag_fail

	mov R0, #-4
	bl Simple_MI_test
	cmp R0, #0
	bne NegativeFlag_fail

	mov R0, #-4
	bl Simple_PL_test
	cmp R0, #1
	bne NegativeFlag_fail

	mov R0, #4
	mov R1, #2
	bl Simple_HI_test
	cmp R0, #1
	bne CarryFlag_fail

	mov r0, #2
	mov r1, #4
	bl Simple_LS_test
	cmp r0, #1
	bne CarryFlag_fail

	mov r0, #4
	mov r1, #-2
	bl Simple_GE_test
	cmp r0, #1
	bne OVFlag_fail

	mov r0, #-1
	mov r1, #2
	bl Simple_lt_test
	cmp r0, #1
	bne OVFlag_fail

	mov r0, #4
	mov r1, #2
	bl Simple_gt_test
	cmp r0, #1
	bne OVFlag_fail

	mov r0, #2
	mov r1, #4
	bl Simple_le_test
	cmp r0, #1
	bne OVFlag_fail

	mov r0, #10
	mov r1, #10
	bl ADD_simple_test
	cmp r0, #20
	bne ADD_fail

	mov r0, #20
	mov r1, #5
	bl SUB_simple_test
	cmp r0, #15
	bne SUB_fail

	mov r0, #5
	mov r1, #2
	bl MUL_simple_test
	cmp r0, #10
	bne MUL_fail

	mov R1, #3
	mov R2, #3
	bl AND_simple_test
	cmp R0, #3
	bne AND_fail

	mov R1, #5
	mov R2, #0
	bl ORR_simple_test
	cmp R0, #5
	bne ORR_fail

	mov R1, #3
	mov R2, #5
	bl EOR_simple_test
	cmp R0, #6
	bne EOR_fail

	bl test_end_success
