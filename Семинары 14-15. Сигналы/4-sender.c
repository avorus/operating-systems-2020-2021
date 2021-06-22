// Необходимо написать две взаимодействующие программы – передатчик и приёмник, которые будут запускаться
// с разных терминалов. Каждая программа вначале печатает свой PID и вводит с клавиатуры PID собеседника.
// Вводится, естественно, пользователем, порядок ввода должен не иметь значения. Программа-передатчик
// запрашивает с клавиатуры ввод целого числа (число может быть и отрицательным!). Введённое число побитово
// передаётся программе-приемнику с использованием пользовательских сигналов SIGUSR1 и SIGUSR2. 
// Программа-приёмник после завершения приёма печатает принятое число.

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int received = 1;

void waiting() {
  received = 1;
}

int main(void) {
  (void)signal(SIGUSR1, waiting);

  printf("[15-sender.c] my PID: %d\n", getpid());

  int recpid; 
  printf("[15-sender.c] enter receiver\'s PID: ");
  scanf("%d", &recpid);

  int number;
  printf("[15-sender.c] enter number: ");
  scanf("%d", &number);

  for (int i = 0; i < 32; ++i) {
    if ((number & (1 << i)) == 0) {
      kill(recpid, SIGUSR1);
    } else {
      kill(recpid, SIGUSR2);
    }
    received = 0;
    while (i != 31 && received == 0) {}
  }

  printf("[15-sender.c] send number: %d\n", number);
  return 0;
}
