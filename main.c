#include <stdio.h>
#include <pthread.h>

struct verify_set {
    int num1;
    int num2;
    int num3;
};

void* verifySudokuWorker(void* set_in)
{//worker thread of execution which verifies a line or a square of nine characters as a valid sudoku combo or not
    //cast incoming struct from void* to verify_set
    struct verify_set* workingset = (struct verify_set*) set_in;

    printf("num1: %i", workingset->num1);
    printf("num2: %i", workingset->num2);
    printf("num3: %i", workingset->num3);
}

int main()
{//home's where the main is.

    struct verify_set testset;
    testset.num1 = 1;
    testset.num2 = 5;
    testset.num3 = 9;
    //import sudoku puzzle from file into 2d array
    //spawn threads
    pthread_t tid;

    pthread_attr_t attr;
    pthread_attr_init(&attr); //init thread attributes

    pthread_create(&tid, &attr, verifySudokuWorker, &testset);

    //pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);

    //wait for threads to complete
    pthread_join(tid, NULL);

    //output results

}