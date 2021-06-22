// Напишите на базе предыдущего примера две программы, одна из которых
// пишет информацию в FIFO, а вторая – читает из него, так чтобы между
// ними не было ярко выраженных родственных связей (т.е. чтобы ни одна
// из них не была потомком другой).

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int     fd;
  size_t  size;
  char    name[] = "aaa.fifo";

  (void)umask(0);

  if (mknod(name, S_IFIFO | 0666, 0) < 0) {
    printf("[error] Can\'t create FIFO\n");
    exit(-1);
  }

  if ((fd = open(name, O_WRONLY)) < 0) {
    printf("[error] Can\'t open FIFO for writting\n");
    exit(-1);
  }

  size = write(fd, "Hello, world!", 14);

  if (size != 14) {
    printf("[error] Can\'t write all string to FIFO\n"); 
    exit(-1);
  }

  if (close(fd) < 0) {
    printf("[error] Can\'t close FIFO\n"); 
    exit(-1);
  }

  printf("Writing program exit\n");

  return 0;
}
