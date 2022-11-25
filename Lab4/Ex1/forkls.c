/* creez un proces nou care sa execute ls in fisierul curent*/
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>

int main(){
	pid_t pid = fork();

	if(pid<0){
        // forkul a esuat
		printf("Fork failed: %d\n", errno);
		return errno;
	}
	else if(pid == 0){
        // ramura copilului
		printf("My PID = %d, Child PID = %d\n", getppid(), getpid());

		char *argv[] = {"ls", NULL};
		execve("/usr/bin/ls", argv, NULL);
		perror(NULL);
	}
	else{
        // ramura parintelui
		wait(NULL);
		// parintele primeste dupa fork pidul copilului
		printf("Child %d finished\n", pid);
	}
	return 0;
}