//
//  swi_handler_rishabh.S
//  
//
//  Created by Rishabh Alaap Singh (rasingh), Carlos Gil (cgil), Varun Kohli (vkohli) on 10/21/12.
//
//

SUB     sp, sp, #4          ; allocate a space for the SPSR
STMFD   sp!, {r0-r12, lr}   ; Store all the registers into the stack before bueginning SWI Call
MRS     r2, spsr            ; Move SPSR value into r2
STR     r2, [sp, #14*4]     ; Save SPSR into allocated space
MOV     r1, sp              ; Move the stack pointer data into the
LDR     r0, [lr, #-4]       ; Extract SWI Instruction from user mode and push into r0
BIC     r0, r0, #0xff000000 ; Extract SWI Number
BL      0xa24               ; 0xa24 holds the C_SWI_HANDLER
LDR     r2, [sp, #14*4]     ; get SPSR back into r2
MSR     spsr, r2            ; push resulting SPSR (in r2) into the SPSR register
LDMFD   sp!, {r1-r12, lr}   ; Pop all values from the stack (I don't think we should only load r1-r12), because then, for example, if we use read and push in the fd, we're never laoding it back.
ADD     sp, sp, #4          ; Bring stack pointer back up to FD convention (undo space alloc for SPSR)
MOVS    pc, lr              ;
