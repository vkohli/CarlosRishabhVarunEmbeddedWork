@ s_handler.S: SWI Handler
@
@ Author: Carlos Gil <cgil@andrew.cmu.edu>
@	  Varun Kohli <vkohli@andrew.cmu.edu>
@	  Rishabh Alaap Singh <rasingh@andrew.cmu.edu>
@ Date:   10/20/2012 3:10 pm
	
.extern C_IRQ_handler 
.global irq_handler 

irq_handler:

	STMFD  sp!, {r0-r12, lr} 	@ store users gp registers
	
	ldr ip, =R8_SAVED
	ldr r8, [ip]

	BL  C_IRQ_handler  		@ goto handler   

	LDMFD  sp!, {r0-r12, lr} 	@ unstack users registers  
	SUBS  pc, lr, #4  		@ return from handler
