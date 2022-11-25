#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>
#include<stdlib.h>

int main(int nrarg, char **argv){
    pid_t pid = fork();

    if(pid<0){
        printf("Fork failed: %d\n", errno);
        return errno;
    }
    else if(pid == 0){  
        // ramura copilului
        int n = atoi(argv[1]);
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
    }
    else{
        // ramura parintelui
        wait(NULL);
        printf("\nChild: %d finished, Parent: %d\n", pid, getpid());
    }
    return 0;
}