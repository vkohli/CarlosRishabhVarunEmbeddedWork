/*
 * kernel.c: Kernel main (entry) function
 *
 * Author: Varun Kohli <vkohli@andrew.cmu.edu>
 *         Carlos Gil <cgil@andrew.cmu.edu>
 *         Rishabh Singh <rasingh@andrew.cmu.edu>
 *
 * Date:   10/17/12
 */

int main(int argc, char *argv[]) {
  
  unsigned *swivec = (unsigned *) 0xa20;
  unsigned *swiaddr = (unsigned *) 0xa24;
  
  *swiaddr = (unsigned) New_S_Handler;
  install_handler((unsigned) swiaddr, swivec);
  return 0;
}

//0x00000a20 is where we overwrite
unsigned install_handler(unsigned location, unsigned int *vector)
{
  unsigned offset;
  unsigned vec, oldvec;
  
  offset = ((unsigned) location - (unsigned) vector - 0x8);
  if (offset & 0xFFFFF000) 
    {
      puts("Installation of handler failed\n");
      exit(0);
    }

  vec = (offset | 0xe59FF000);
  oldvec = *vector;
  *vector = vec;
  return (oldvec);
}

void C_SWI_handler(unsigned swi_num, unsigned *regs)
{
  switch(swi_num) {
  //exit
  case 0x900001: 
    exit(regs[0]);
    break;

  //read
  case 0x900003:
    
    break;

  //write
  case 2:
    break;

  default:
    puts("Invalid swinumber: %d\n", swi_num);
    exit(0x0badc0de);
  }
}
// Todo
// Finish setup
// Implement SWI handler
// add clean up
