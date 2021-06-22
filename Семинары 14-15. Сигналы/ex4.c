#include <signal.h>
#include <stdio.h>

int i = 0;

void (*p)(int); // Pointer that will contain the address of the previous signal handler

void my_handler(int nsig) {
  printf("Receive signal %d, CTRL-C pressed\n", nsig);
  i = i+1;
  //
  // After the 5th processing, we return the original response to the signal.
  //
  if (i == 5) (void)signal(SIGINT, p);
}

int main(void) {
  //
  // Set the process response to the SIGINT signal,
  // remembering the address of the previous handler.
  //
  p = signal(SIGINT, my_handler);
  //
  // From this point, the process will print a SIGINT message 5 times.
  //
  while(1);
  return 0;
}
