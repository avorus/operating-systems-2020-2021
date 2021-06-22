// Модифицируйте обработку SIGCHLD в программе ex6.c, так, чтобы процесс-родитель все-таки сообщал о статусе
// всех завершившихся детей. Изменять в программе можно только обработчик сигнала, используя только сведения
// текущего семинара, без всяких POSIX сигналов!

#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>

void my_handler(int nsig) {
  int status;
  pid_t pid;

  pid = waitpid(-1, &status, WNOHANG);
  while (pid != 0) {
    if (pid < 0) {
      if (errno == 10) {
        printf("There is no process to close\n");
      } else {
        printf("Some error on waitpid errno = %d\n", errno);
      }
      return;
    } else {
      if ((status & 0xff) == 0) {
        printf("Process %d was exited with status %d\n", pid, status >> 8);
      } else if ((status & 0xff00) == 0) {
        printf("Process %d killed by signal %d %s\n", pid, status &0x7f,
               (status & 0x80) ? "with core file" : "without core file");
      }
      pid = waitpid(-1, &status, WNOHANG);
    }
  }
}

int main(void) {
  pid_t pid;
  int i,j;

  (void) signal(SIGCHLD, my_handler);

  for (i=0; i<10; i++) {
    if ((pid = fork()) < 0) {
      printf("Can\'t fork child 1\n");
      exit(1);
    } else if (pid == 0) {
      /* Child  */
      for (j=1; j < 10000000; j++);
      exit(200 + i);
    }
  }

  while(1);
  return 0;
}
