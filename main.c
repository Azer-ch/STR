#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<unistd.h>

pthread_mutex_t vAB, vBC, vCD, vEB, vFA;
void *departedFromA(void *arg) {
    int i = *(int *) (arg);
    pthread_mutex_lock(&vAB);
    printf("segment AB occupied by train %d\n", i);
    usleep(1000000);
    printf("Train %d is leaving segment AB\n", i);
    pthread_mutex_unlock(&vAB);
    pthread_mutex_lock(&vBC);
    printf("segment BC occupied by train %d\n", i);
    usleep(1000000);
    printf("Train %d is leaving segment BC\n", i);
    pthread_mutex_unlock(&vBC);
    pthread_mutex_lock(&vCD);
    printf("segment CD occupied by train %d\n", i);
    usleep(1000000);
    printf("Train %d is leaving segment CD\n", i);
    pthread_mutex_unlock(&vCD);
}

void *departedFromF(void *arg) {
    int i = *(int *) (arg);
    pthread_mutex_lock(&vFA);
    printf("segment FA occupied by train %d\n", i);
    usleep(1000000);
    printf("Train %d is leaving segment FA\n", i);
    pthread_mutex_unlock(&vFA);
    departedFromA(arg);
}

void *departedFromE(void *arg) {
    int i = *(int *) (arg);
    pthread_mutex_lock(&vEB);
    printf("segment EB occupied by train %d\n", i);
    usleep(1000000);
    printf("Train %d is leaving segment EB\n", i);
    pthread_mutex_unlock(&vEB);
    pthread_mutex_lock(&vBC);
    printf("segment BC occupied by train %d\n", i);
    usleep(1000000);
    printf("Train %d is leaving segment BC\n", i);
    pthread_mutex_unlock(&vBC);
    pthread_mutex_lock(&vCD);
    printf("segment CD occupied by train %d\n", i);
    usleep(1000000);
    printf("Train %d is leaving segment CD\n", i);
    pthread_mutex_unlock(&vCD);
}

int main() {
    pthread_t train[5];
    pthread_mutex_init(&vAB, NULL);
    pthread_mutex_init(&vBC, NULL);
    pthread_mutex_init(&vCD, NULL);
    pthread_mutex_init(&vEB, NULL);
    pthread_mutex_init(&vFA, NULL);
    for(int i=0;i<5;i++){
        int * id = malloc(sizeof(int));
        *id = i+1;
        if(i%3==0)
            pthread_create(&train[i], NULL, departedFromA, id);

        else if(i%3==1)
            pthread_create(&train[i], NULL, departedFromF,id);
        else
            pthread_create(&train[i], NULL, departedFromE, id);
    }
    for(int i=0;i<5;i++){
        pthread_join(train[i],NULL);
    }
}