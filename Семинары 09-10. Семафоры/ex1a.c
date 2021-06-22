#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
  int    semid;        // IPC descriptor for an array of IPC semaphores
  char   pathname[]="09-1a.c"; // The file name used to generate the key.
                               // A file with this name must exist in the current directory
  key_t  key;          // IPC key
  struct sembuf mybuf; // Structure for specifying operations on a semaphore.

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }
  //
  // Try to access by key the array of semaphores, if it exists,
  // or create it from a single semaphore if it does not exist yet,
  // with read & write access for all users.
  //
  if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t create semaphore set\n");
    exit(-1);
  }
  //
  // Perform operation D(semid1,1) on the semaphore array.
  // But first fill in the structure. The flag, as usual, is set to 0.
  // The semaphore array consists of one semaphore with number 0.
  // The opcode is -1.
  //
  mybuf.sem_num = 0;
  mybuf.sem_op  = -1;
  mybuf.sem_flg = 0;

  if (semop(semid, &mybuf, 1) < 0) {
    printf("Can\'t wait for condition\n");
    exit(-1);
  }

  printf("The condition is present\n");
  return 0;
}
