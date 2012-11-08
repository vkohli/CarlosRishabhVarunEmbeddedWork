@ s_handler.S: SWI Handler
@
@ Author: Carlos Gil <cgil@andrew.cmu.edu>
@		  Varun Kohli <vkohli@andrew.cmu.edu>
@		  Rishabh Singh <rasingh@cmu.edu>
@ Date:   10/20/2012 3:10 pm
	
.extern C_SWI_handler 
.global s_handler 

s_handler:
	mrs ip, cpsr
	bic ip, ip, #0x80
	msr cpsr, ip
	
//	SUB   sp, sp, #4 			@ leave room on stack for SPSR  

	STMFD  sp!, {r0-r12, lr} 	@ store users gp registers
	
//	MRS  r2, spsr 				@ get SPSR into gp registers 
//	STR  r2, [sp, #14*4]  		@ store SPSR above gp registers 

	ldr ip, =R8_SAVED
	ldr r8, [ip]

	MOV  r1, sp  				@ pointer to parameters on stack  
	LDR  r0, [lr, #-4]  		@ extract the SWI number  
	BIC  r0,r0,#0xff000000 		@ get SWI # by bit-masking
	BL  C_SWI_handler  			@ goto handler   

//	LDR  r2, [sp, #14*4]  		@ restore SPSR (NOT “sp!”) 
//	MSR  spsr, r2      			@ restore SPSR from r2  

	LDMFD  sp!, {r0-r12, lr} 	@ unstack users registers  

//	ADD  sp, sp, #4        		@ remove space used to store SPSR  

	MOVS  pc, lr  				@ return from handler
