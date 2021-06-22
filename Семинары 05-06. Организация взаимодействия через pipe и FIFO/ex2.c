#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int     fd[2];
  ssize_t size;
  char     string[] = "Hello, world!";
  char  resstring[14];
  //
  // Try to create a pipe
  //
  if (pipe(fd) < 0) {
    printf("Can\'t open pipe\n");
    exit(-1);
  }

  size = write(fd[1], string, 14);

  if (size != 14) {
    printf("Can\'t write all string to pipe\n");
    exit(-1);
  }

  size = read(fd[0], resstring, 14);

  if (size < 0) {
    printf("Can\'t read string from pipe\n");
    exit(-1);
  }

  printf("%s\n", resstring);

  if (close(fd[0]) < 0) {
    printf("Can'\t close reading side of pipe\n"); exit(-1);
  }
  if (close(fd[1]) < 0) {
    printf("Can'\t close writing side of  pipe\n"); exit(-1);
  }

  return 0;
}
