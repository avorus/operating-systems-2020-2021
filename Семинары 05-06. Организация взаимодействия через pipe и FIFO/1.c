// Измените программу ex1.c так, чтобы она читала записанную ранее в файл информацию
// и печатала её на экране. Все лишние операторы желательно удалить.

#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
  int     fd;
  size_t  size;
  char    restring[14];
  
  (void)umask(0);
  
  if ((fd = open("myfile", O_RDONLY, 0666)) < 0) {
    printf("Can\'t open file\n");
    exit(-1);
  }
  
  size = read(fd, restring, 14);

  if (size != 14) {
    printf("Can\'t read all string\n");
    exit(-1);
  }

  printf("Value from file: %s\n", restring);

  if (close(fd) < 0) {
    printf("Can\'t close file\n");
  }

  return 0;
}
