#include<stdlib.h>
#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<stdatomic.h>

int L1, C1, M1[100][100];
int L2, C2, M2[100][100];
int L3, C3, M3[100][100];

struct argument{
    int linie;
    int coloana;
};
typedef struct argument argument;

void * thread_product(void* arg){
    int linie = ((argument *)arg)->linie;
    int coloana = ((argument *)arg)->coloana;

    M3[linie][coloana] = 0;

    for(int i=0; i<C1; i++){
        M3[linie][coloana] += M1[linie][i] * M2[i][coloana];
    }

    return NULL;
}

int main(int argc, char *argv[]){
    int index_citire = 1;

    //citire
    L1 = atoi(argv[index_citire++]);
    C1 = atoi(argv[index_citire++]);


    for(int i=0; i<L1; i++){
        for(int j=0; j<C1; j++){
            M1[i][j] = atoi(argv[index_citire++]);
        }
    }

    L2 = atoi(argv[index_citire++]);
    C2 = atoi(argv[index_citire++]);

    for(int i=0; i<L2; i++){
        for(int j=0; j<C2; j++){
            M2[i][j] = atoi(argv[index_citire++]);
        }
    }

    L3 = L1, C3 = C2;

    pthread_t threads[10000];
    int index = 0;

    for(int a=0; a<L1; a++){
        for(int b=0; b<C2; b++){
            argument *ar = (argument *)malloc(sizeof(argument));
            ar->linie = a;
            ar->coloana = b;
            if(pthread_create(&threads[index++], NULL, thread_product, ar)){
                perror(NULL);
                return errno;
            }
        }
    }
    for(int i=0; i<(L1*C2); i++){
        if(pthread_join(threads[i], NULL)){
            perror(NULL);
            return errno;
        }
    }

    for(int i=0; i<L3; i++){
        for(int j=0; j<C3; j++){
            printf("%d ", M3[i][j]);
        }
        printf("\n");
    }

    return 0;
}