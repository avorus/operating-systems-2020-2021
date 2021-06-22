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
  //
  // Query the status of the completed process and get its identifier at the same time.
  //
  if ((pid = waitpid(-1, &status, 0))  < 0) {
    printf("Some error on waitpid errno = %d\n", errno);
  } else {
    if ((status & 0xff) == 0) {
      //
      // The process ended with an explicit or implicit exit() call.
      //
      printf("Process %d was exited with status %d\n", pid, status >> 8);
    } else if ((status & 0xff00) == 0) {
      //
      // The process was terminated with a signal.
      //
      printf("Process %d killed by signal %d %s\n", pid, status &0x7f,
             (status & 0x80) ? "with core file" : "without core file");
    }
  }
}

int main(void) {
  pid_t pid;

  (void) signal(SIGCHLD, my_handler);

  if ((pid = fork()) < 0) {
    printf("Can\'t fork child 1\n");
    exit(1);
  } else if (pid == 0) {
    /* Child 1 */
    exit(200);
  }

  if ((pid = fork()) < 0) {
    printf("Can\'t fork child 2\n");
    exit(1);
  } else if (pid == 0){
    /* Child 2 */
    while(1);
  }

  while(1);
  return 0;
}
