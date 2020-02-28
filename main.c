#include <stdio.h>
#include <pthread.h>

struct verify_set {
    int numbs[9];
    int valid;
};

void* verifySudokuRow(void* set_in)
{//worker thread of execution which verifies a line or a square of nine characters as a valid sudoku combo or not
    //cast incoming struct from void* to verify_set
    struct verify_set* workingset = (struct verify_set*) set_in;

}

void* verifySudokuCol(void* set_in)
{//worker thread of execution which verifies a line or a square of nine characters as a valid sudoku combo or not
    //cast incoming struct from void* to verify_set
    struct verify_set* workingset = (struct verify_set*) set_in;

}

void* verifySudokuBox(void* set_in)
{//worker thread of execution which verifies a line or a square of nine characters as a valid sudoku combo or not
    //cast incoming struct from void* to verify_set
    struct verify_set* workingset = (struct verify_set*) set_in;

}

int main()
{//home's where the main is.

    struct verify_set testset;
    //import sudoku puzzle from file into 2d array
    //spawn threads
    pthread_t tids[6];
    pthread_attr_t attrs[6];

    //generate test sets

    for (int i=0;i<6;i++){
        pthread_attr_init(&attrs[i]); //init thread attributes
        pthread_create(&tids[i], &attrs[i], verifySudokuWorker, &testset);
    }

    //pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);

    //wait for threads to complete
    pthread_join(tid, NULL);

    //output results

}