#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<stdio.h>
#include<errno.h>

void *thread_rev(void* arg){
    char *value = (char *)arg;
    printf("%s => ", value);

    int n = strlen(value);
    char* result = (char *) malloc (sizeof(char)*n + 1);
    for(int i=0; i<n; i++){
        result[n-i-1] = value[i];
    }
    printf("%s\n", result);
    free(result);
    return NULL;
}

int main(int argc, char *argv[]){
    pthread_t thread;

    char *retval = argv[1];

    if(pthread_create(&thread, NULL, thread_rev, retval)){
        perror(NULL);
        return errno;
    }
    if(pthread_join(thread, NULL)){
        perror(NULL);
        return errno;
    }

    return 0;
}