// Модифицируйте программу ex3.с для организации такой двусторонней связи.

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int     fd1[2], fd2[2], result;

  size_t size1, size2;
  char  resstring1[27], resstring2[26];

  if (pipe(fd1) < 0) {
    printf("Can\'t open first pipe\n");
    exit(-1);
  }
  
  if (pipe(fd2) < 0) {
    printf("Can\'t open second pipe\n");
    exit(-1);
  }
  
  result = fork();

  if (result < 0) {
    printf("Can\'t fork child\n");
    exit(-1);
  } else if (result > 0) {

   /* Parent process */

    if (close(fd1[0]) < 0) {
      printf("[parent] Can\'t close reading side of first pipe\n"); exit(-1);
    }

    size1 = write(fd1[1], "Hello, world, from parent!", 27);

    if (size1 != 27) {
      printf("Can\'t write all string to first pipe\n");
      exit(-1);
    }

    if (close(fd1[1]) < 0) {
      printf("[parent] Can\'t close writing side of first pipe\n"); exit(-1);
    }

    if (close(fd2[1]) < 0) {
      printf("[parent] Can\'t close writing side of second pipe\n"); exit(-1);
    }

    size2 = read(fd2[0], resstring2, 26);

    if (size2 < 0) {
      printf("Can\'t read string from second pipe\n");
      exit(-1);
    }

    printf("Parent got:  %s\n", resstring2);

    if (close(fd2[0]) < 0) {
      printf("[parent] Can\'t close reading side of first pipe\n"); exit(-1);
    }

  } else {

    /* Child process */

    if (close(fd1[1]) < 0) {
      printf("[child] Can\'t close writing side of first pipe\n"); exit(-1);
    }

    size1 = read(fd1[0], resstring1, 27);

    if (size1 < 0) {
      printf("Can\'t read string from first pipe\n");
      exit(-1);
    }

    printf("Child got:  %s\n", resstring1);

    if (close(fd1[0]) < 0) {
      printf("[child] Can\'t close reading side of first pipe\n"); exit(-1);
    }

    if (close(fd2[0]) < 0) {
      printf("[child] Can\'t close reading side of second pipe\n"); exit(-1);
    }

    size2 = write(fd2[1], "Hello, world, from child!", 26);

    if (size2 != 26) {
      printf("Can\'t write all string to second pipe\n");
      exit(-1);
    }

    if (close(fd2[1]) < 0) {
      printf("[child] Can\'t close writing side of second pipe\n"); exit(-1);
    }
  }

  return 0;
}
