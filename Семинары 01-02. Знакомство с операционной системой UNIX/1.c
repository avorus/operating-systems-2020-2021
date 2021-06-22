// Напишите, откомпилируйте и запустите программу, которая печатала бы
// идентификатор пользователя, запустившего программу, и идентификатор его группы.

#include <unistd.h>
#include <stdio.h>
int main(int agrc, char** argv) {
  printf("UID = %d\nGID=%d\n", getuid(), getgid());
  return 0;
}
