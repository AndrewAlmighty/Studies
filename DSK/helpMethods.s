.section .data

registerR0_fault: .ascii "Register R0 fault\0"
registerR1_fault: .ascii "Register R1 fault\0"
registerR2_fault: .ascii "Register R2 fault\0"
registerR3_fault: .ascii "Register R3 fault\0"
registerR4_fault: .ascii "Register R4 fault\0"
registerR5_fault: .ascii "Register R5 fault\0"
registerR6_fault: .ascii "Register R6 fault\0"
registerR7_fault: .ascii "Register R7 fault\0"
registerR8_fault: .ascii "Register R8 fault\0"
registerR9_fault: .ascii "Register R9 fault\0"
registerR10_fault: .ascii "Register R10 fault\0"
registerR11_fault: .ascii "Register R11 fault\0"
registerR12_fault: .ascii "Register R12 fault\0"
registerLR_fault: .ascii "Register LR fault\0"
registerSP_fault: .ascii "Register R1 fault\0"
registerPC_fault: .ascii "Register R1 fault\0"
testZeroFlag_fail: .ascii "Zero flag fault \0"
testNegativeFlag_fail: .ascii "Neg flag fault \0"
testCarryFlag_fail: .ascii "Carry flag fault \0"
testOVFlag_fail: .ascii "OV Flag fault \0"
AddFail: .ascii "ADD fail! \0"
SUBFail: .ascii "SUB fail! \0"
MULFail: .ascii "MUL fail! \0"
ANDFail: .ascii "AND fail! \0"
ORRFail: .ascii "ORR fail! \0"
EORFail: .ascii "EOR fail! \0"
test_success: .ascii "All tests success\0"

.section .text

prepareErrRegOutput:
	push {lr}
	mov R7, #4
	mov R0, #1
	mov R2, #20
	pop {pc}

program_end:
	mov R7, #1
	SWI 0

test_end_success:
	bl prepareErrRegOutput
	LDR R1, =test_success
	SWI 0
	bl program_end

EOR_fail:
	bl prepareErrRegOutput
	LDR R1, =EORFail
	SWI 0
	bl program_end

ORR_fail:
	bl prepareErrRegOutput
	LDR R1, =ORRFail
	SWI 0
	bl program_end

AND_fail:
	bl prepareErrRegOutput
	LDR R1, =ANDFail
	SWI 0
	bl program_end

MUL_fail:
	bl prepareErrRegOutput
	LDR R1, =MULFail
	SWI 0
	bl program_end

SUB_fail:
	bl prepareErrRegOutput
	LDR R1, =SUBFail
	SWI 0
	bl program_end

ADD_fail:
	bl prepareErrRegOutput
	LDR R1, =AddFail
	SWI 0
	bl program_end

OVFlag_fail:
	bl prepareErrRegOutput
	LDR R1, =testOVFlag_fail
	SWI 0
	bl program_end

CarryFlag_fail:
	bl prepareErrRegOutput
	LDR R1, =testCarryFlag_fail
	SWI 0
	bl program_end

NegativeFlag_fail:
	bl prepareErrRegOutput
	LDR R1, =testNegativeFlag_fail
	SWI 0
	bl program_end

ZeroFlag_fail:
	bl prepareErrRegOutput
	LDR R1, =testZeroFlag_fail
	SWI 0
	bl program_end

R0_fault:
	bl prepareErrRegOutput
	LDR R1, =registerR0_fault
	SWI 0
	bl program_end

R1_fault:
	bl prepareErrRegOutput
	LDR R1, =registerR1_fault
	SWI 0
	bl program_end

R2_fault:
	bl prepareErrRegOutput
	LDR R1, =registerR2_fault
	SWI 0
	bl program_end

R3_fault:
	bl prepareErrRegOutput
	LDR R1, =registerR3_fault
	SWI 0
	bl program_end

R4_fault:
	bl prepareErrRegOutput
	LDR R1, =registerR4_fault
	SWI 0
	bl program_end

R5_fault:
	bl prepareErrRegOutput
	LDR R1, =registerR5_fault
	SWI 0
	bl program_end

R6_fault:
	bl prepareErrRegOutput
	LDR R1, =registerR6_fault
	SWI 0
	bl program_end

R7_fault:
	bl prepareErrRegOutput
	LDR R1, =registerR7_fault
	SWI 0
	bl program_end

R8_fault:
	bl prepareErrRegOutput
	LDR R1, =registerR8_fault
	SWI 0
	bl program_end

R9_fault:
	bl prepareErrRegOutput
	LDR R1, =registerR9_fault
	SWI 0
	bl program_end

R10_fault:
	bl prepareErrRegOutput
	LDR R1, =registerR10_fault
	SWI 0
	bl program_end

R11_fault:
	bl prepareErrRegOutput
	LDR R1, =registerR11_fault
	SWI 0
	bl program_end

R12_fault:
	bl prepareErrRegOutput
	LDR R1, =registerR12_fault
	SWI 0
	bl program_end

LR_fault:
	bl prepareErrRegOutput
	LDR R1, =registerLR_fault
	SWI 0
	bl program_end

SP_fault:
	bl prepareErrRegOutput
	LDR R1, =registerSP_fault
	SWI 0
	bl program_end

PC_fault:
	bl prepareErrRegOutput
	LDR R1, =registerPC_fault
	SWI 0
	bl program_end






