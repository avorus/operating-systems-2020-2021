// Измените предыдущие программы, так чтобы первая программа (ex1a.c) могла работать
// без блокировки после не менее 5 запусков второй программы (ex1b.c).

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[], char *envp[])
{
  int semid;
  char pathname[]="09-1a.c";
  key_t key;
  struct sembuf mybuf;

  // Генерируем значение ключа
  if ((key = ftok(pathname,0)) < 0) {
    printf("[error 09-1b.c] can\'t generate key\n");
    exit(-1);
  }

  // Создаем массив семафоров длиной 1, соответствующий ключу key
  if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
    printf("[error 09-1b.c] can\'t create semaphore set\n");
    exit(-1);
  }

  // Заполняем массив
  mybuf.sem_num = 0; // позиция в массиве
  mybuf.sem_op  = 1; // код операции A(S,1)
  mybuf.sem_flg = 0;

  // Выполняем операцию над семафором
  if (semop(semid, &mybuf, 1) < 0) {
    printf("[error 09-1b.c] can\'t add 1 to semaphore\n");
    exit(-1);
  }

  // Завершаем программу
  printf("[success 09-1b.c] the condition is present\n");
  return 0;
}
