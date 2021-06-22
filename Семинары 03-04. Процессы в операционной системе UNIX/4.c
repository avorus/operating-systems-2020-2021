// Модифицируйте программу, созданную при выполнении задачи 2 (2.c) так,
// чтобы порождённый процесс запускал на исполнение новую (любую) программу.

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[], char *envp[]) {
	pid_t process_id = fork();
	if (process_id < 0) {
		printf("Error has caused by creating process\n");
		exit(-1);
	}
	else if (process_id == 0) {
		printf("Hi, I'm child! I want to call cat for task4-1.c\n");
		(void) execle("/bin/cat","/bin/cat","task4-1.c",0, envp);
		printf("Error has caused by calling program\n");
		exit(-1);
	}
	else printf("Hi,I'm parent!\n");
	return 0;
}
