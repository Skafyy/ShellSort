#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <omp.h>
#include <threads.h>

typedef struct {
    int *array;
    int size;
    int gap;
    int *thread_id;
    int num_threads;
} ThreadData;


int getArraySize();
void getArray(int *array, int size);
int getSpacing(int *spacing, int size);
void shellSort(int *array, int size, int *spacing, int spacingSize);
int shellSortThread(void *args);



int main(){
    return 0;
}