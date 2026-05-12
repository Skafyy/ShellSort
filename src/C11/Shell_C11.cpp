#include <iostream>
#include <vector>
#include <thread>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <cstdio>

using namespace std;

struct ThreadData {
    int *array;
    int size;
    int gap;
    int thread_id;
    int num_threads;
};

int getArraySize();
void getArray(int *array, int size);
void getSpacing(vector<int>& spacing, int size);
void shellSortThread(int *arr, int size, int gap, int thread_id, int num_threads);
void shellSort(int *array, const vector<int>& spacing, int size, int currentThreads);

int main(){
    srand(static_cast<unsigned>(time(NULL)));

    int threadConfigs[]={1,2,4,8};
    int numConfigs=sizeof(threadConfigs)/sizeof(threadConfigs[0]);
    int repeats=5;

    int size=getArraySize();

    int *array=(int*)malloc(size * sizeof(int));
    vector<int> spacing;

    getSpacing(spacing, size);

    if(array == NULL){
        printf("Hiba a memoriafoglalaskor!\n");
        return 1;
    }

    printf("\n-----------------MERES INDITASA----------------------\n");
    printf("%-15s | %-15s\n", "Szalak szama", "Atlagos ido(mp)");
    printf("---------------------------------------------------");

    for(int i=0; i<numConfigs; i++){
        int currentThread=threadConfigs[i];
        double totalTime=0.0;

        for(int j=0; j<repeats; j++){
            getArray(array, size);

            double start=omp_get_wtime();
            shellSort(array, spacing, size, currentThread);
            double end=omp_get_wtime();

            totalTime+=(end-start);
        }
        printf("%-15d | %-15d.10f\n", currentThread, totalTime/repeats);
    }

    free(array);
    return 0;
}

int getArraySize(){
    char ch; bool ok; int x;
    do {
        ok=true;
        printf("Adja meg a tomb meretet: \n");
        if(scanf("%d", &x) != 1){
            cout<<"Hibas bemenet, ez nem egy szam!";
            while(std::cin.get(ch) && ch != '\n');
            ok=false;
        } else if(x<=0){
            printf("Hibas meretet adott meg, a meret nem lehet egyenlo nullaval!\n");
            ok=false;
        }
    } while(!ok);

    return x;
}

void getArray(int *array, int size){
    for(int i=0; i<size; i++){
        array[i]=rand();
    }
}

void getSpacing(int *spacing, int size){
    int h=1, count=0;
    while(h<size/3){
        spacing[count++]=h;
        h=3*h+1;
    }
    if(count==0){
        spacing[count++]=1;
    }
}

void shellSortThread(int *arr, int size, int gap, int thread_id, int num_threads){
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
    }
}

void shellSort(int *array, const vector<int>& spacing, int size, int currentThreads){
    for(int i=(int)spacing.size(); i>=0; i--){
        int gap=spacing[i];
        vector<thread> threads;

        for(int j=0; j<currentThreads; j++){
            ThreadData td={array, size, gap, j, currentThreads};
            threads.emplace_back(shellSortThread, td);
        }

        for(auto &t : threads){
            if(t.joinable()){
                t.join();
            }
        }
    }
}

