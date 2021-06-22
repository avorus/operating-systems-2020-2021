// Взяв за основу программу ex3.с с семинаров 5-6, реализуйте поочередную неоднократную
// передачу информации между ребёнком и родителем через один pipe в двух направлениях,
// используя ровно один IPC семафор. Считать, что сначала родитель передает данные ребёнку,
// а затем получает данные от него. Количество пар передача и приём данных в родителе и пар
// приём и передача в ребёнке равно N ≥ 2. Значение N вводится в родителе с клавиатуры до 
// порождения ребёнка. Число N потенциально может быть достаточно большим и превышать максимально
// возможное значение семафора, поэтому использовать значение N в операциях над семафором не разрешается.

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>

// value of S = 0
// цикл:
//   -- parent --
//   write()
//   A(S,2)
//   Z(S)
//   read()
//   -- child --
//   D(S,1)
//   read()
//   write()
//   D(S,1)

int main()
{
  int fd[2], result;
  size_t size;
  char  message[16];

  char pathname[] = "09-3.c";
  key_t key;
  struct sembuf mybuf;
  int semid;

  int N; // гарантируется, что N >= 2
  scanf("%d", &N);

  if (pipe(fd) < 0) {
    printf("[error] can\'t open pipe\n");
    exit(-1);
  }

  if ((key = ftok(pathname,0)) < 0) {
    printf("[error] can\'t generate key\n");
    exit(-1);
  }

  if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
    printf("[error] can\'t get semaphore set\n");
    exit(-1);
  }

  result = fork();
  if (result < 0) {
    printf("[error] can\'t fork child\n");
    exit(-1);
  }  

  for (int i = 0; i < N; ++i) {
    if (result > 0) {
      /* parent */
      // write
      size = write(fd[1], "Hi from parent!", 16);
      if (size != 16) {
        printf("[parent error] can\'t write all string to pipe\n");
        exit(-1);
      }

      // A(S,2)
      mybuf.sem_num = 0;
      mybuf.sem_op = 2;
      mybuf.sem_flg = 0;
      if (semop(semid, &mybuf, 1) < 0) {
        printf("[parent error] can\'t wait for condition\n");
        exit(-1);
      }

      // Z(S)
      mybuf.sem_num = 0;
      mybuf.sem_op = 0;
      mybuf.sem_flg = 0;
      if (semop(semid, &mybuf, 1) < 0) {
        printf("[parent error] can\'t wait for condition\n");
        exit(-1);
      }

      // read
      size = read(fd[0], message, 16);
      if (size != 16) {
        printf("[parent error] can\'t read string from pipe\n");
        exit(-1);
      }

      printf("[parent success %d] read from pipe: %s\n", i, message);
    } else {
      /* child */

      // D(S,1)
      mybuf.sem_num = 0;
      mybuf.sem_op = -1;
      mybuf.sem_flg = 0;
      if (semop(semid, &mybuf, 1) < 0) {
        printf("[parent error] can\'t wait for condition\n");
        exit(-1);
      }

      // read
      size = read(fd[0], message, 16);
      if (size != 16) {
        printf("[child error] can\'t read string from pipe\n");
        exit(-1);
      }

      printf("[child success %d] read from pipe: %s\n", i, message);

      // write
      size = write(fd[1], "Hi from chiild!", 16);
      if (size != 16) {
        printf("[child error] can\'t write all string to pipe\n");
        exit(-1);
      }

      // D(S,1)
      mybuf.sem_num = 0;
      mybuf.sem_op = -1;
      mybuf.sem_flg = 0;
      if (semop(semid, &mybuf, 1) < 0) {
        printf("[parent error] can\'t wait for condition\n");
        exit(-1);
      }
    }
  }

  if (close(fd[0]) < 0) {
    printf("[error] can\'t close reading side of pipe\n"); 
    exit(-1);
  }

  if (close(fd[1]) < 0) {
    printf("[error] can\'t close writing side of pipe\n"); 
    exit(-1);
  }

  return 0;
}
