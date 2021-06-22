// Напишите две программы, осуществляющие взаимодействие через разделяемую память.
// Первая программа должна создавать сегмент разделяемой памяти и копировать туда
// собственный исходный текст, вторая программа должна брать оттуда этот текст,
// печатать его на экране и удалять сегмент разделяемой памяти из системы.

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
  char *buffer;
  int current = 0;
  int shmid;
  char pathname[] = "07-1a.c";
  key_t key;

  if ((key = ftok(pathname,0)) < 0) {
    printf("[error] Cannot generate key\n");
    exit(-1);
  }

  if ((shmid = shmget(key, 2000*sizeof(char), 0666|IPC_CREAT)) < 0) {
    printf("[error] Cannot find shared memory\n");
    exit(-1);
  }

  if ((buffer = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)) {
    printf("[error] Cannot attach shared memory\n");
  }

  while(buffer[current] != '\0') {
    printf("%c", buffer[current]);
    current++;
  }

  if (shmdt(buffer) < 0) {
    printf("[error] Cannot detach shared memory\n");
    exit(-1);
  }

  if (shmctl(shmid, 0, NULL) < 0) {
    printf("[error] Cannot free shared memory\n");
    exit(-1);
  }

  return 0;
}
