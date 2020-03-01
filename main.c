#include <stdio.h>
#include <pthread.h>

enum WorkerType {ROW_WORKER = 0, COL_WORKER, BOX_WORKER};
enum validBit {NONE = -1, INVALID = 0, VALID = 1};
static pthread_mutex_t printmutex = PTHREAD_MUTEX_INITIALIZER;

struct WorkStation {//struct for a worker's stuff
    int id;
    int checktoperf;//which row, col, box to validate
    enum WorkerType type;//type of worker check pattern
    enum validBit valid;//enum for storing check status
    int* puzzle;//puzzle to search through
};

struct bitfield{
    unsigned int flag : 1;
};

enum validBit checkBitfield(struct bitfield* numbitstr){
    for (int i=1; i <= 9; i++){// every possible number, check if it was in the scope of this worker's puzzle space.
        if (numbitstr[i].flag == 0)
            return INVALID;
    }
    return VALID;
}

void* verifySudokuWorker(void* station_in)
{//worker thread of execution which verifies a line or a square of nine characters as a valid sudoku combo or not
    //cast incoming struct from void* to WorkStation*
    struct WorkStation* station = (struct WorkStation*) station_in;
    struct bitfield numbitstr[10];
    int startindex;

    switch (station->type)
    {
        case 0://ROW_WORKER
            startindex = 9*(station->checktoperf);//start index is the row # * some multiple of 9.
            for (int i = 0; i < 9; i++)
            {//for each puzzle entry set the bitfield value for that number to 1
                numbitstr[station->puzzle[startindex + i]].flag = 1;
            }
            station->valid = checkBitfield(numbitstr);
            switch (station->valid)
            {
            case VALID:
                pthread_mutex_lock(&printmutex);
                printf("THREAD %d, ROW %d, VALID.\n", station->id, (station->checktoperf + 1));
                pthread_mutex_unlock(&printmutex);
                break;
                
            case INVALID:
                pthread_mutex_lock(&printmutex);
                printf("THREAD %d, ROW %d, INVALID.\n", station->id, (station->checktoperf + 1));
                pthread_mutex_unlock(&printmutex);
                break;
            
            default:
                pthread_mutex_lock(&printmutex);
                printf("THREAD %d, ROW %d, SOMETHING WENT WRONG.\n", station->id, (station->checktoperf + 1));
                pthread_mutex_unlock(&printmutex);
                break;
            }
            break;

        case 1://COL_WORKER
            startindex = station->checktoperf;
            for (int i = 0; i < 9; i++){
                numbitstr[station->puzzle[startindex + i*9]].flag = 1;
            }
            station->valid = checkBitfield(numbitstr);
            switch (station->valid)
            {
            case VALID:
                pthread_mutex_lock(&printmutex);
                printf("THREAD %d, COL %d, VALID.\n", station->id, (station->checktoperf + 1));
                pthread_mutex_unlock(&printmutex);
                break;
                
            case INVALID:
                pthread_mutex_lock(&printmutex);
                printf("THREAD %d, COL %d, INVALID.\n", station->id, (station->checktoperf + 1));
                pthread_mutex_unlock(&printmutex);
                break;
            
            default:
                pthread_mutex_lock(&printmutex);
                printf("THREAD %d, COL %d, SOMETHING WENT WRONG.\n", station->id, (station->checktoperf + 1));
                pthread_mutex_unlock(&printmutex);
                break;
            }
            break;
        case 2://BOX_WORKER
            if (station->checktoperf == 0 || station->checktoperf == 3 || station->checktoperf == 6){
                startindex = (station->checktoperf)*9;
            }
            else if (station->checktoperf == 1 || station->checktoperf == 4 || station->checktoperf == 7){
                startindex = (station->checktoperf - 1)*9 + 3;
            }
            else
                startindex = (station->checktoperf - 2)*9 + 6;
            
            int rowstart;
            int value;
            for (int k = 0; k < 3; k++){
                rowstart = startindex + k*9;
                for (int i = 0; i < 3; i++){
                    numbitstr[station->puzzle[rowstart + i]].flag = 1;
                }
            }
            station->valid = checkBitfield(numbitstr);
            switch (station->valid)
            {
            case VALID:
                pthread_mutex_lock(&printmutex);
                printf("THREAD %d, BOX %d, VALID.\n", station->id, (station->checktoperf + 1));
                pthread_mutex_unlock(&printmutex);
                break;
                
            case INVALID:
                pthread_mutex_lock(&printmutex);
                printf("THREAD %d, BOX %d, INVALID.\n", station->id, (station->checktoperf + 1));
                pthread_mutex_unlock(&printmutex);
                break;
            
            default:
                pthread_mutex_lock(&printmutex);
                printf("THREAD %d, BOX %d, SOMETHING WENT WRONG.\n", station->id, (station->checktoperf + 1));
                pthread_mutex_unlock(&printmutex);
                break;
            }
            break;
    }
}

int main(int argc, char* argv[])
{//home's where the main is.
    const int THREAD_COUNT = 27;
    //import sudoku puzzle from file into 2d array
    FILE* infile = fopen(argv[1],"r"); //get file from shell
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

    int checks[3] = {0,0,0};//array for keeping track of which sudoku checks have been scheduled and which ones havent. There are 3 types of check: rows, cols, and boxes and each type has 9 checks to be made. checks start positions will always be scheduled in following order:

    pthread_t tids[THREAD_COUNT];
    pthread_attr_t attrs[THREAD_COUNT];
    struct WorkStation desks[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT;i++)
    {
        int checktype = i % 3;//pick type of worker to spawn
        desks[i].id = i;
        desks[i].type = checktype; //tell worker what kind of worker it is
        desks[i].checktoperf = checks[checktype];//tell the worker which check it is performing
        desks[i].valid = -1;//check hasn't been performed.
        desks[i].puzzle = puzzle;//give worker pointer to puzzle

        pthread_attr_init(&attrs[i]); //init thread attributes
        //spawn it
        pthread_create(&tids[i], &attrs[i], verifySudokuWorker, &desks[i]);
        checks[checktype]++;//keep track of how many checks are being made
    }

    for (int i = 0; i < THREAD_COUNT; i++)
    {//wait for threads to complete
        pthread_join(tids[i], NULL);
    }

    return 1;
}