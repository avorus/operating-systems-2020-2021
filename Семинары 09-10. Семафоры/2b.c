// На прошлом занятии вы установили, что любые неатомарные операции, связанные с
// изменением содержимого разделяемой памяти, представляют собой критическую секцию
// процесса или нити исполнения. Модифицируйте программы ex3a.c и ex3b.c, которые
// иллюстрировали некорректное взаимодействие процессов через разделяемую память,
// обеспечив с помощью одного IPC семафора взаимоисключения для их правильной работы.
// Примечание:
// Считать, что инициализация разделяемой памяти и задание начального значения семафора
// происходят до взаимодействия процессов (т.е. при отдельном запуске одной из программ).

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/sem.h>

// Критическая секция - доступ к разделяемой памяти, запись значения в разделяемую память и печать сообщения с результатом работы программы на консоль.
// Если семафор не существует, то создаем семафор, так как изначально у него значение 0, пользуемся операцией A(S,1)
// Далее уменьшаем значение семафора D(S,1) до записи в разделяемую память,
// После записи увеличиваем значение семафора A(S,1)
int main()
{
  int *array;
  int shmid;
  int new = 1;
  long i;

  char pathname[] = "09-2a.c";
  key_t key;

  int semid;
  struct sembuf mybuf;

  if ((key = ftok(pathname,0)) < 0) {
    printf("[error 09-2b.c] can\'t generate key\n");
    exit(-1);
  }

  if ((shmid = shmget(key, 3*sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0) {
    if (errno != EEXIST) {
      printf("[error 09-2b.c] can\'t create shared memory\n");
      exit(-1);
    } else {
      if ((shmid = shmget(key, 3*sizeof(int), 0)) < 0) {
        printf("[error 09-2b.c] can\'t find shared memory\n");
        exit(-1);
      }
      new = 0;
    }
  }

  if ((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
    printf("[error 09-2b.c] can\'t attach shared memory\n");
    exit(-1);
  }

  if ((semid = semget(key, 1, 0666)) < 0) {
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
      printf("[error 09-2b.c] can\'t create semaphore set\n");
      exit(-1);
    }

    mybuf.sem_num = 0;
    mybuf.sem_op = 1;
    mybuf.sem_flg = 0;
    if (semop(semid, &mybuf, 1) < 0) {
      printf("[error 09-2b.c] can\'t wait for condition\n");
      exit(-1);
    }
  }

  if (new) {
    array[0] =  0;
    array[1] =  1;
    array[2] =  1;

    printf
        ("[success 09-2b.c]\nprogram 1 was spawn %d times, program 2 - %d times, total - %d times\n",
        array[0], array[1], array[2]);
  } else {
    mybuf.sem_num = 0;
    mybuf.sem_op = -1;
    mybuf.sem_flg = 0;

    if (semop(semid, &mybuf, 1) < 0) {
      printf("[error 09-2b.c] can\'t wait for condition\n");
      exit(-1);
    }

    array[1] += 1;
    for(i=0; i<2000000000L; i++);
    array[2] += 1;

    printf
        ("[success 09-2b.c]\nprogram 1 was spawn %d times, program 2 - %d times, total - %d times\n",
        array[0], array[1], array[2]);

    mybuf.sem_num = 0;
    mybuf.sem_op = 1;
    mybuf.sem_flg = 0;

    if (semop(semid, &mybuf, 1) < 0) {
      printf("[error 09-2b.c] can\'t wait for condition\n");
      exit(-1);
    }
  }

  if (shmdt(array) < 0) {
    printf("[error 09-2b.c] can\'t detach shared memory\n");
    exit(-1);
  }

  return 0;
}
