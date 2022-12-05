#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<unistd.h>

pthread_mutex_t segments[5];
char seg[5][10] = {"AB","BC","CD","EB","FA"};
void arrivingAtSegment(int segment,int train){
    pthread_mutex_lock(&segments[segment]);
    printf("Train %d is at segment %s\n",train,seg[segment]);
}
void leavingSegment(int segment , int train){
    printf("Train %d is departing from segment %s\n", train , seg[segment]);
    pthread_mutex_unlock(&segments[segment]);
}
void *startingFromA(void *arg) {
    int train = *(int *) (arg);
    for(int segment = 0 ; segment < 3 ; segment ++){
        arrivingAtSegment(segment,train);
        usleep(1000000);
        leavingSegment(segment,train);
    }
}

void *startingFromF(void *arg) {
    int train = *(int *) (arg);
    arrivingAtSegment(4,train);
    usleep(1000000);
    leavingSegment(4,train);
    startingFromA(arg);
}

void *startingFromE(void *arg) {
    int train = *(int *) (arg);
    arrivingAtSegment(3,train);
    usleep(1000000);
    leavingSegment(3,train);
    for(int segment = 1 ; segment < 3 ;segment ++){
        arrivingAtSegment(segment,train);
        usleep(1000000);
        leavingSegment(segment,train);
    }
}

int main() {
    int trains_number;
    printf("Enter the number of trains: ");
    scanf("%d",&trains_number);
    srand(time(NULL));
    int r;
    pthread_t *train = calloc(trains_number, sizeof(unsigned long));
    for(int i=0;i<trains_number;i++){
        pthread_mutex_init(&segments[i], NULL);
    }
    for(int i=0;i<trains_number;i++){
        int * id = malloc(sizeof(int));
        *id = i+1;
        r = rand() % 100;
        if(r%3==0)
            pthread_create(&train[i], NULL, startingFromA, id);

        else if(r%3==1)
            pthread_create(&train[i], NULL, startingFromF, id);
        else
            pthread_create(&train[i], NULL, startingFromE, id);
    }
    for(int i=0;i<trains_number;i++){
        pthread_join(train[i],NULL);
    }
}