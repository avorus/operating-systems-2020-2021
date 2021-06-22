// Отредактировать пример 1 (ex1.c) так, чтобы сделать поведение двух процессов различным.

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
int main(int argc, char **argv) {
	pid_t process_id = fork();
	if (process_id  < 0) {
		printf("Error has caused by creating new process.\n");
	} 
	else if (process_id  == 0) {
		printf("Process-child:\nPID = %d\nPPID = %d\n", getpid(), getppid());
	}
	else printf("Process-parent:\nPID = %d\nPPID = %d\nChildPID = %d\n", getpid(), getppid(), process_id);
	return 0;
}
