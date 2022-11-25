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
#include<semaphore.h>

#define NBTHREAD 20
pthread_t threads[NBTHREAD];

pthread_mutex_t mutex;
sem_t semaphore;
sem_t semaphore2;

int count;
unsigned int t_allowed, N, N2;

void init(unsigned int N){
    if(pthread_mutex_init(&mutex, NULL)){
        perror("Mutex create - error");
    }
    sem_init(&semaphore, 0, N);
}

void barrier_point(){

    pthread_mutex_lock(&mutex);
    count++;
    if(count == NBTHREAD) {
        sem_post(&semaphore); //trigger pentru 1

        
        // for(int i = 0; i<=NBTHREAD; i++)
        //    sem_post(&semaphore);
         count = 0;
    }
    pthread_mutex_unlock(&mutex);

    // se blocheaza aici primele threaduri
    sem_wait(&semaphore);
    sem_post(&semaphore);
}

void * tfun(void * v)
{
    int * tid = (int *) v ;
    printf("%d reached the barrier---------------- \n" , * tid);

    barrier_point();

    printf("%d passed the barrier \n" , * tid);
    free( tid);
    return NULL ;
}

int main(int argc, char *argv[]){
    printf("NBTHREADS = %d\n", NBTHREAD);
    init(N);

    for(int index=0; index<NBTHREAD; index++){
        int * tid1 = (int*)malloc(sizeof(int));
        *tid1 = index;

        if(pthread_create(&threads[index], NULL, tfun, tid1)){
            perror("Thread create - error");
            return errno;
        }
    }
    for(int index = 0; index<NBTHREAD; index++){
        if(pthread_join(threads[index], NULL)){
            perror("Threads join - error");
            return errno;
        }
    }
    pthread_mutex_destroy(&mutex);
    sem_destroy(&semaphore);
    return 0;
}