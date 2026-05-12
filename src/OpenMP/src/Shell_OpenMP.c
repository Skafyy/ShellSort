#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <omp.h>

int getArraySize(int *size);
void getArray(int *array, int size);
int getSpacingSize(int *spacingSize);
int getSpacing(int *spacing, int size);
void shellSort(int *array, int *spacing, int spacingSize, int size);

int main(){
    int threads[]={1,2,4,8};
    int numConfigs=sizeof(threads)/sizeof(threads[0]);
    int repeats=5;

    int size=getArraySize(&size);
    printf("A tomb merete: %d\n", size);

    int *spacing = (int *)malloc(size * sizeof(int));
    if(spacing==NULL){
        printf("Hiba tortent a memoria foglalasa kozben! (spacing)\n");
        return 1;
    }

    int *array = (int *)malloc(size * sizeof(int));
    if(array==NULL){
        printf("Hiba tortent a memoria foglalasa kozben! (array)\n");
        return 1;
    }


    int spacingSize = getSpacing(spacing, size);
    printf("A lepeskozok merete: %d\n", spacingSize);

    srand(time(NULL));
    
    printf("\n---- MERES INDITASA ----\n");
    printf("-%10s | %-15s\n", "Szalak", "Atlagos ido (mp)");
    printf("--------------------------------\n");

    for(int i = 0; i< numConfigs; i++){
        int currentThreads=threads[i];
        omp_set_num_threads(currentThreads);

        double totalTime=0.0;

        for(int j=0; j<repeats; j++){
            getArray(array, size);

            double startTime=omp_get_wtime();
            shellSort(array, spacing, spacingSize, size);
            double endTime=omp_get_wtime();

            totalTime+=(endTime-startTime);
        }

        double averageTime=totalTime/repeats;
        printf("-%10d | %-15.10f\n", currentThreads, averageTime);
    }
    
    printf("--------------------------------\n");
    printf("---- MERES VEGE ----\n");

    free(array);
    free(spacing);

    return 0;
}

int getArraySize(int *size){
    char ch; bool ok; int x;
    do {
        ok=true;
        printf("Kerem adja meg a tomb meretet: \n");
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

void shellSort(int *array, int *spacing, int spacingSize, int size){

    for(int lepes_index=spacingSize-1; lepes_index>=0; lepes_index--){
        int gap=spacing[lepes_index];

        #pragma omp parallel for
        for(int i=0; i<gap; i++){

            for(int aktualis_index=i+gap; aktualis_index<size; aktualis_index+=gap){
                int ideiglenes_elem=array[aktualis_index];
                int bal_index=aktualis_index-gap;

                while(bal_index>=i && array[bal_index]>ideiglenes_elem){
                    array[bal_index+gap]=array[bal_index];
                    bal_index-=gap;
                }
                array[bal_index+gap]=ideiglenes_elem;
            }
        }
    }
}
