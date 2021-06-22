#include <unistd.h>

int main() {
  (void)fork();
  while(1);
  return 0;
}
