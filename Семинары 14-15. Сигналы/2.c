// Модифицируйте программу из предыдущего примера так, чтобы она печатала сообщение и об нажатии клавиш
// <ctrl> и <4>. На разные комбинации клавиш должны печататься разные сообщения. 
// Используйте одну и ту же функцию для обработки сигналов SIGINT и SIGQUIT. Откомпилируйте и запустите
// её, убедитесь в правильной работе. Снимать программу также придётся с другого терминала командой kill.

#include <signal.h>
#include <stdio.h>

void my_handler(int nsig) {
  if (nsig == 2)
    printf("Receive signal %d, CTRL-C pressed\n", nsig);
  else if (nsig == 3)
    printf("Receive signal %d, CTRL-4 pressed\n", nsig);
}

int main(void) {
  (void)signal(SIGINT, my_handler);
  (void)signal(SIGQUIT, my_handler);

  while(1);

  return 0;
}
