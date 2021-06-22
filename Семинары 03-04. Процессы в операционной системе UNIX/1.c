// Напишите программу, печатающую значения PID и PPID для текущего процесса.
// Запустите её несколько раз подряд. Посмотрите, как меняется идентификатор
// текущего процесса. Объясните наблюдаемые изменения.

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
int main(int agrc, char** argv) {
  printf("PID = %d\nPPID = %d\n", getpid(), getppid());
  return 0;
}

