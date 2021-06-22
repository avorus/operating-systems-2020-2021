// Определите размер pipe для вашей операционной системы.

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
  int fd[2];
  int full = 0; //0 - false, there is a space in the pipe, 1 - true
  int result = 0;
  if (pipe2(fd, O_NONBLOCK) < 0) {
    printf("[error] Can\'t open pipe\n");
    exit(-1);
  }

  while (!full) {
    if (write(fd[1], "p", 1) > 0) result++;
    else full = 1;
  }

  printf("[answer] %d\n", result);

  if (close(fd[0]) < 0) {
    printf("[error] Can'\t close reading side of pipe\n"); exit(-1);
  }
  if (close(fd[1]) < 0) {
    printf("[error] Can'\t close writing side of  pipe\n"); exit(-1);
  }

  return 0;
}
