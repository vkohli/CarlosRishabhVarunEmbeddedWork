/*
 * kernel.c: Kernel main (entry) function
 *
 * Author: Varun Kohli <vkohli@andrew.cmu.edu>
 *         Carlos Gil <cgil@andrew.cmu.edu>
 *         Rishabh Singh <rasingh@andrew.cmu.edu>
 *
 * Date:   10/17/12
 */
#import <exports.h>

void C_SWI_handler(unsigned swi_num, unsigned *regs);
unsigned install_handler(unsigned location, unsigned int *vector);
void setup(int argc, char *argv[]);
unsigned s_handler();

int main(int argc, char *argv[]) {
  
  unsigned *swivec = (unsigned *) 0xa20;
  unsigned *swiaddr = (unsigned *) 0xa24;
  
  *swiaddr = (unsigned) s_handler;
  install_handler((unsigned) swiaddr, swivec);

  //  setup(argc, argv);
  printf("end\n");
  return 0;
}

//0x00000a20 is where we overwrite
unsigned install_handler(unsigned location, unsigned int *vector)
{
  //unsigned offset;
  unsigned vec, oldvec;
  
  /* offset = ((unsigned) location - (unsigned) vector - 0x8);
  printf("offset: %d\n", offset);
   if (offset & 0xFFFFF000) 
    {
      puts("Installation of handler failed\n");
      return 0;
      //      exit(0);
      }*/

  vec = 0xe51FF004;
  oldvec = *vector;
  *vector = vec;
  return (oldvec);
}

void C_SWI_handler(unsigned swi_num, unsigned *regs)
{
  switch(swi_num) {
  //exit
  case 0x900001: 
    printf("exit!\n");
    break;

  //read
  case 0x900003:
    printf("read!\n");
    break;

  //write
  case 0x900004:
    printf("write\n");
    break;

  default:
    printf("Invalid swinumber: %d\n", swi_num);
    //exit(0x0badc0de);
  }
}
// Todo
// Finish setup
// Implement SWI handler
// add clean up
