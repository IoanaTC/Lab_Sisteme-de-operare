#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>
#include<stdlib.h>

int main(int nrarg, char *argv[]){
    printf("Starting parent %d\n", getpid());
	for(int i=0; i<nrarg; i++){
        // cream un nou proces corespunzator parametrului dat
		pid_t pid = fork();

		if(pid<0){
	        printf("Fork failed: %d\n", errno);
            return errno;
        }
      	else if(pid == 0){
            // ramura copilului
            int n = atoi(argv[i+1]);
            printf("%d: ", n);

            while(n > 1){
                if(n%2 == 0){
                    n = n/2;
                    printf("%d ", n);
                }
                else{
                    n = 3*n+1;
                    printf("%d ", n);
                }
            }
            printf(".\n");
            // terminam procesul copil
            return 0;
        }
        else{
            // ramura parinte
            // parintele curent asteapta ca oricare dintre procesele copil sa se termine
            pid_t child_pid = wait(NULL);
            printf("Done Parent %d Me %d\n", getpid(), child_pid);
        }
	}
    return 0;
}