// Модифицируйте программу ex2.c, добавив к ней третью нить исполнения.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int a = 0; // глобальная переменная в качестве разделяемой памяти нитей

void *mythread(void *dummy)
{
  pthread_t thid;
  thid = pthread_self();

  a = a + 1;

  printf("[thread %u] calculation result: %d\n", thid, a);
  return NULL;
}

int main()
{
  pthread_t main_thid, thid1, thid2; // идентификаторы нитей
  int result; // код возможной ошибки

  // Создаем первую нить
  result = pthread_create (&thid1, (pthread_attr_t *)NULL, mythread, NULL);

  if (result != 0) {
    printf ("[error] on first thread create, fault code: %d\n", result);
    exit(-1);
  }

  printf("[thread %u] created\n", thid1);

  // Создаем вторую нить
  result = pthread_create (&thid2, (pthread_attr_t *)NULL, mythread, NULL);

  if (result != 0) {
    printf ("[error] on second thread create, fault code: %d\n", result);
    exit(-1);
  }

  printf("[thread %u] created\n", thid2);

  // Выполняем главную нить
  main_thid = pthread_self();

  a = a + 1;

  printf("[thread %u] calculation result: %d\n", main_thid, a);

  // Переводим главную нить в состояние ожидания завершения других нитей
  pthread_join(thid1, (void **)NULL);
  pthread_join(thid2, (void **)NULL);

  return 0;
}
