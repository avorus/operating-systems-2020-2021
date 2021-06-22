#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
  int   semid;
  char pathname[]="09-1a.c";
  key_t key;
  struct sembuf mybuf;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t create semaphore set\n");
    exit(-1);
  }
  //
  // Perform operation A(semid1,1) on the semaphore array.
  // But first fill in the structure. The flag, as usual, is set to 0.
  // The semaphore array consists of one semaphore with number 0.
  // The opcode is 1.
  //
  mybuf.sem_num = 0;
  mybuf.sem_op  = 1;
  mybuf.sem_flg = 0;

  if (semop(semid, &mybuf, 1) < 0) {
    printf("Can\'t add 1 to semaphore\n");
    exit(-1);
  }

  printf("The condition is present\n");
  return 0;
}
