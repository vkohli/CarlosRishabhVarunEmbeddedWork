#include <stdio.h>
#include <fcntl.h>

main(int argc, char * argv[])
{
  char buf[32];
  int bytesRead;
  int fp;
  fp = open("test", O_RDWR, "rb");
  do{
    bytesRead = read(fp, buf, 1);
    printf("%d", bytesRead);
    }
  while(bytesRead != 0);
  close(fp);

  exit(0);
}
