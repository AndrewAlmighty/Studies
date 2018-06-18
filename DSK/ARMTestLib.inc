.section .text

@General registers basic tests. We jsut add simple value to register, where we keep 0 and then return it.

register_R0_basic_test:
	push {lr}
	mov R1, R0
	mov R0, #0
	add R0, R1
	pop {pc}

register_R1_basic_test:
	push {lr}
	mov R1, #0
	add R1, R0
	mov R0, R1
	pop {pc}

register_R2_basic_test:
	push {lr}
	mov R2, #0
	add R2, R0
	mov R0, R2
	pop {pc}

register_R3_basic_test:
	push {lr}
	mov R3, #0
	add R3, R0
	mov R0, R3
	pop {pc} 

register_R4_basic_test:
	push {lr}
	mov R4, #0
	add R4, R0
	mov R0, R4
	pop {pc}

register_R5_basic_test:
	push {lr}
	mov R5, #0
	add R5, R0
	mov R0, R5
	pop {pc}

register_R6_basic_test:
	push {lr}
	mov R6, #0
	add R6, R0
	mov R0, R6
	pop {pc}

register_R7_basic_test:
	push {lr}
	mov R7, #0
	add R7, R0
	mov R0, R7
	pop {pc}

register_R8_basic_test:
	push {lr}
	mov R8, #0
	add R8, R0
	mov R0, R8
	pop {pc}

register_R9_basic_test:
	push {lr}
	mov R9, #0
	add R9, R0
	mov R0, R9
	pop {pc}

register_R10_basic_test:
	push {lr}
	mov R10, #0
	add R10, R0
	mov R0, R10
	pop {pc}

register_R11_basic_test:
	push {lr}
	mov R11, #0
	add R11, R0
	mov R0, R11
	pop {pc}

register_R12_basic_test:
	push {lr}
	mov R12, #0
	add R12, R0
	mov R0, R12
	pop {pc}

@LR register is tested for whole time as link register, now we will just test it like a general register.
register_LR_basic_test:
	push {lr}
	mov lr, #0
	add lr, R0
	mov R0, lr
	pop {pc}

@register R13 test - the stack pointer - SP. We push argument to stack, then we push 1 n times (n is second argument). Then we pop n times stack, then pop our test argument and return.

register_SP_basic_test:
	push {lr}
	push {R0}
	mov R5, #0
	mov R2, R1
	mov R3, #1
.SP_basic_test_push:
	push {R3}
	sub R1, #1
	cmp R1, R5
	bgt .SP_basic_test_push
.SP_basic_test_pop:
	pop {R3}
	sub R2, #1
	cmp R2, R5
	bgt .SP_basic_test_pop
	
	pop {R0}
	pop {pc}

@PC register test - we will just check if PC value grows well
register_PC_basic_test:

	push {lr}
	mov R1, PC
	mov R2, PC
	mov R3, PC
	mov R4, PC
	mov R5, PC
	
	sub R5, R4
	sub R4, R3
	cmp R5, R4
	bne PC_error
	sub R3, R2
	cmp R4, R3
	bne PC_error
	sub R2, R1
	cmp R3, R2
	bne PC_error
	mov R0, #1
	pop {pc}

PC_error:
	mov R0, #0
	pop {pc}
	
@End of registers test
@Begin of CPSR register tests

Simple_EQ_test:
	push {lr}
	cmp R0, R1
	bne .QE_err
	mov R0, #1
	pop {pc}
.QE_err:
	mov R0, #0
	pop {pc}

Simple_NE_test:
	push {lr}
	cmp R0, R1
	bne .NE_ok
	mov R0, #0
	pop {pc}
.NE_ok:
	mov R0, #1
	pop {pc}

Simple_MI_test:
	push {lr}
	cmp R0, R0
	bpl .MI_err
	mov R0, #1
	pop {pc}

.MI_err:
	mov R0, #0
	pop {pc}

Simple_PL_test:
	push {lr}
	cmp R0, R0
	bmi .PL_err
	mov R0, #1
	pop {pc}
.PL_err:
	mov R0, #0
	pop {pc}

Simple_HI_test:
	push {lr}
	cmp R0, R1
	bhi .HI_ok
	mov R0, #0
	pop {pc}
.HI_ok:
	mov R0, #1
	pop {pc}

Simple_LS_test:
	push {lr}
	cmp R0, R1
	bls .ls_ok
	mov R0, #0
	pop {pc}
.ls_ok:
	mov R0, #1
	pop {pc}

Simple_GE_test:
	push {lr}
	cmp R0, R1
	bge .ge_ok
	mov R0, #0
	pop {pc}
.ge_ok:
	mov R0, #1
	pop {pc}

Simple_lt_test:
	push {lr}
	cmp R0, R1
	blt .lt_ok
	mov R0, #0
	pop {pc}
.lt_ok:
	mov R0, #1
	pop {pc}

Simple_gt_test:
	push {lr}
	cmp R0, R1
	bgt .gt_ok
	mov R0, #0
	pop {pc}
.gt_ok:
	mov R0, #1
	pop {pc}

Simple_le_test:
	push {lr}
	cmp R0, R1
	ble .le_ok
	mov R0, #0
	pop {pc}
.le_ok:
	mov R0, #1
	pop {pc}

@--------------------------------------------------------
@Basic instructions test

ADD_simple_test:
	push {lr}
	ADD R0, R1
	pop {pc}

SUB_simple_test:
	push {lr}
	SUB R0, R1
	pop {pc}

MUL_simple_test:
	push {lr}
	MUL R0, R1
	pop {pc}

AND_simple_test:
	push {lr}
	AND R0, R1, R2
	pop {pc}

ORR_simple_test:
	push {lr}
	ORR R0, R1, R2
	pop {pc}

EOR_simple_test:
	push {lr}
	EOR R0, R1, R2
	pop {pc}
