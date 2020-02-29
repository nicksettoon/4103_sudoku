#include <stdio.h>
#include <pthread.h>

enum WorkerType {ROW_WORKER = 0, COL_WORKER, BOX_WORKER};
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct WorkStation {//struct for a worker's stuff
    int checktoperf;
    enum WorkerType type;
};

void* verifySudokuWorker(void* station_in)
{//worker thread of execution which verifies a line or a square of nine characters as a valid sudoku combo or not
    //cast incoming struct from void* to WorkStation
    struct WorkStation* station = (struct WorkStation*) station_in;

    switch (station->type)
    {
        case 0://ROW_WORKER
            pthread_mutex_lock(&mutex);
            printf("I am a row worker, checking row %d.\n", station->checktoperf);
            pthread_mutex_unlock(&mutex);
            break;
        case 1://COL_WORKER
            pthread_mutex_lock(&mutex);
            printf("I am a col worker, checking col %d.\n", station->checktoperf);
            pthread_mutex_unlock(&mutex);
            break;
        case 2://BOX_WORKER
            pthread_mutex_lock(&mutex);
            printf("I am a box worker, checking box %d.\n", station->checktoperf);
            pthread_mutex_unlock(&mutex);
            break;
    }
}

int main()
{//home's where the main is.
    const int CPU_THREAD_COUNT = 12;
    //import sudoku puzzle from file into 2d array
    FILE* infile = fopen("test.txt","r");
    int puzzle[81]; //array for sudoku puzzle state
    char buffer[21]; //array for line by line read buffer

    if (infile == NULL){//open file
        printf("File not opened.");
        return 0;
    }
    else
        printf("File opened.\n");

    int i = 0; //interator for filling puzzle array
    while (fgets(buffer, 21, infile) != NULL)
    {//grab each line of the sudoku puzzle in the text file
        printf("%s", buffer);
        for (int c = 0; c < sizeof(buffer) ; c++)
        {//for each letter in that line, add it to puzzle, skipping whitespace and newline
            if (buffer[c] != 32 && buffer[c] != 10 && buffer[c] != 85 && !(buffer[c] <= 0))
            {
                puzzle[i] = buffer[c] - '0';//convert to int counterpart
                i++;
            }
        }
    }
    fclose(infile);
    printf("\n");

    for (int i = 0; i < 81; i++){
        printf("%d",puzzle[i]);
    }
    printf("\n");

    int checks[3] = {0,0,0};//array for keeping track of which sudoku checks have been scheduled and which ones havent. There are 3 types of check rows, cols, and boxes and each type has 9 checks to be made. checks start positions will always be scheduled in following order:
    //rows: 0, 9, 18, etc...
    //cols: 0, 1, 2, 3, etc...
    //boxes, 0, 3, 6, 27, 30, 33, etc...

    pthread_t tids[CPU_THREAD_COUNT];
    pthread_attr_t attrs[CPU_THREAD_COUNT];
    struct WorkStation desks[CPU_THREAD_COUNT];

    for (int i = 0; i < CPU_THREAD_COUNT;i++)
    {
    //generate thread starting positions
    //spawn all box threads first if possible
        int checktype = i % 3;
        desks[i].type = checktype; //tell worker what kind of worker it is
        desks[i].checktoperf = checks[checktype];//tell the worker which check it is performing
        pthread_attr_init(&attrs[i]); //init thread attributes
        pthread_create(&tids[i], &attrs[i], verifySudokuWorker, &desks[i]);
        checks[checktype]++;
    }

    // }
    //wait for threads to complete

    for (int i = 0; i < CPU_THREAD_COUNT; i++)
    {
        pthread_join(tids[i], NULL);
    }

    //output results

    return 1;
}

//pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);