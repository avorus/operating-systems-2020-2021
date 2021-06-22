#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
  //
  // We will run the cat command with the command line argument 03-2.c
  // without changing the environment parameters,
  // i.e. actually run command "cat 03-2.c",
  // which should output the contents of the file on the screen.
  // For the execle function we give as program name its full name
  // with the path from the root directory -/bin/cat as the name of the program.
  // The first word on the command line must be the same as the name of the program we want to run.
  // The second word on the command line is the name of the file whose contents we want to print.
  //
  (void) execle("/bin/cat", "/bin/cat", "03-2.c", 0, envp);
  //
  // We get here only when an error occurs.
  //
  printf("Error on program start\n");
  exit(-1);
}
