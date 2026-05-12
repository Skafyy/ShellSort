#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <omp.h>
#include <pthread.h>

#define NUM_THREADS 4

typedef struct {
    int *array;
    int size;
    int gap;
    int thread_id;
    int num_threads;
} ThreadData;

int getArraySize();
void getArray(int *array, int size);
int getSpacing(int *spacing, int size);
void shellSort(int *array, int *spacing, int spacingSize, int size, int currentThreads);
void* shellSortThread(void* arg);

int main() {
    int threads[]={1,2,4,8};
    int numConfigs=sizeof(threads)/sizeof(threads[0]);
    int repeats=5;

    int size=getArraySize();

    int *array=(int*)malloc(size*sizeof(int));
    int *spacing=(int*)malloc(size*sizeof(int));

    if(array == NULL){
        printf("Nem sikerult memoriat foglalni a tombnek! (array)\n");
        return 1;
    }

    if(spacing == NULL){
        printf("Nem sikerult memoriat foglalni a tombnek! (spacing)\n");
        return 1;
    }

    int spacingSize=getSpacing(spacing, size);
    srand(time(NULL));

    printf("\n---------MERES INDITASA---------\n");
    printf("-10s | %-15s\n", "Szálak száma", "Átlagos idő (mp)");
    printf("--------------------------------\n");

    for(int i=0; i<numConfigs; i++){
        int currentThreads=threads[i];
        double totalTime=0.0;

        for(int j=0; j<repeats; j++){
            getArray(array, size);

            double start=omp_get_wtime();
            shellSort(array, spacing, spacingSize, size, currentThreads);
            double end=omp_get_wtime();

            totalTime+=(end-start);
        }
        printf("-%10d | %-15.10f\n", currentThreads, totalTime/repeats);
    }

    free(array);
    free(spacing);

    return 0;
}

int getArraySize(){
    char ch; bool ok; int x;
    do {
        ok=true;
        printf("Adja meg a tomb meretet: \n");
        if(scanf("%d", &x) != 1){
            printf("Hibas meretet adott meg, ez nem egy szam!\n");
            while((ch=getchar()!='\n'));
            ok=false;
        } else if(x<=0){
            printf("Hibas meretet adott meg, a meret nem lehet egyenlo a nullaval!\n");
            ok=false;
        }
        } while(!ok);
    return x;
}

void getArray(int *array, int size){
    for(int i = 0; i < size; i++){
        array[i] = rand();
    }
}

int getSpacing(int *spacing, int size){
    int h = 1; int count = 0;
    while(h<size/3){
        spacing[count]=h;
        h = 3*h+1;
        count++;
    }

    if(count==0){
        spacing[0]=1;
        count=1;
    }
    return count;
}

void* shellSortThread(void *threadData){
    ThreadData *data=(ThreadData*)threadData;
    int *arr=(*data).array;
    int size=(*data).size;
    int gap=(*data).gap;
    int thread_id=(*data).thread_id;
    int num_threads=(*data).num_threads;

    for(int i=thread_id; i<gap; i+=num_threads){
        for(int j=i+gap; j<size; j+=gap){
            int temp=arr[j];
            int k=j;

            while(k>=gap && arr[k-gap]>temp){
                arr[k]=arr[k-gap];
                k-=gap;
            }
            arr[k]=temp;
    }
    return NULL;
}
}

void shellSort(int *array, int *spacing, int spacingSize, int size, int currentThreads) {
    pthread_t threads[currentThreads];
    ThreadData data[currentThreads];

    for (int i = spacingSize - 1; i >= 0; i--) {
        int gap = spacing[i];

        for (int j = 0; j < currentThreads; j++) {
            data[j].array = array;
            data[j].size = size;
            data[j].gap = gap;
            data[j].thread_id = j;
            data[j].num_threads = currentThreads;

            pthread_create(&threads[j], NULL, shellSortThread, &data[j]);
        }

        for (int j = 0; j < currentThreads; j++) {
            pthread_join(threads[j], NULL);
        }
    }
}

