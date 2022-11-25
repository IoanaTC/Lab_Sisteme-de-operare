//pthread_mutex_init(pthread_mutex_t *mutex, NULL)
//pthread_mutex_(un)lock(&mutex)
//pthread_mutex_destroy(&mutex)

//sem_init(sem_t *sem, pshared = 0, unsigned_int sem_value)
//sem_wait(&sem) => scade <=> lock
//sem_post(&sem) => creste <=> unlock
//sem_destroy(&sem)

#include<pthread.h>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>

#define MAX_RESOURCES 5
#define NBTHREAD 5

int available_resources = MAX_RESOURCES;
pthread_t threads[NBTHREAD];
pthread_mutex_t mutex;

int decrease_count ( int count )
{
    if(available_resources < count) return -1;
    else{
        // critic section - start
        pthread_mutex_lock(&mutex);
        available_resources -= count; //race condition
        printf("Got %d resources, %d remaining\n", count, available_resources);
        pthread_mutex_unlock(&mutex);
        // critic section - end
    }
    return 0;
}
int increase_count ( int count )
{
    // critic section - start
    pthread_mutex_lock(&mutex);
    available_resources += count; //race condition
    printf("Released %d resources, %d remaining\n", count, available_resources);
    pthread_mutex_unlock(&mutex);
    // critic section - end

    return 0;
}

void * thread_resources (void * arg){
    int count = *(int*)arg;

    if(!decrease_count(count))
        increase_count(count);
    
    return NULL;
}

int main(){
    printf("MAX_RESOURCES = %d\n", MAX_RESOURCES);

    if(pthread_mutex_init(&mutex, NULL)){
        perror("Mutex create - error");
        return errno;
    }
    for(int index = 0; index<NBTHREAD; index++){
        int * count = (int*)malloc(sizeof(int));
        *count = rand() % (MAX_RESOURCES + 1); // nr random 0-MAX_RESOURCES

        if(pthread_create(&threads[index], NULL, &thread_resources, count)){
            perror("Thread create - error");
            return errno;
        }
        free(count);
    }
    for(int index = 0; index<NBTHREAD; index++){
        if(pthread_join(threads[index], NULL)){
            perror("Threads join - error");
            return errno;
        }
    }
    pthread_mutex_destroy(&mutex);

    return 0;
}