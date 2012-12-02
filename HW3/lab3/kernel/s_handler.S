@ s_handler.S: SWI Handler
@
@ Author: Carlos Gil <cgil@andrew.cmu.edu>
@	  Varun Kohli <vkohli@andrew.cmu.edu>
@	  Rishabh Alaap Singh <rasingh@andrew.cmu.edu>
@ Date:   10/20/2012 3:10 pm
	
.extern C_SWI_handler 
.global s_handler 

@s_handler
@sets up the values for the C_SWI_Handler. It gets the SWI number from the
@call and moves it into supervisor mode.

s_handler:	
	STMFD  sp!, {r0-r12, lr} 	@ store users gp registers

	mrs ip, cpsr
	bic ip, ip, #0x80
	msr cpsr, ip

	ldr ip, =R8_SAVED
	ldr r8, [ip]

	MOV  r1, sp  			@ pointer to parameters on stack  
	LDR  r0, [lr, #-4]  		@ extract the SWI number  
	BIC  r0,r0,#0xff000000 		@ get SWI # by bit-masking
	BL  C_SWI_handler  		@ goto handler   

	LDMFD  sp!, {r0-r12, lr} 	@ unstack users registers  

	MOVS  pc, lr  			@ return from handler
