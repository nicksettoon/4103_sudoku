#include <stdio.h>
#include <pthread.h>

// enum WorkerType {ROW_WORKER, COL_WORKER, BOX_WORKER};

// struct verify_set {
//     int startposition;
//     enum WorkerType type;
// };

// void* verifySudokuWorker(void* set_in)
// {//worker thread of execution which verifies a line or a square of nine characters as a valid sudoku combo or not
//     //cast incoming struct from void* to verify_set
//     struct verify_set* workingset = (struct verify_set*) set_in;

//     switch (workingset->type)
//     {
//         case 0://ROW_WORKER
//             break;
//         case 1://COL_WORKER
//             break;
//         case 2://BOX_WORKER
//             break;
//     }
// }

void getPuzzle(int* puzzle)
{//opens file, gets puzzle state and stores it at the recieved location.
    FILE* infile = fopen("test.txt","r");
    char buffer[25]; //array for line by line read buffer

    if (infile == NULL){//open file
        printf("File not opened.");
        return 1;
    }

    int i = 0; //interator for filling puzzle array
    while (fgets(buffer, 25, infile) != NULL)
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
}

int main()
{//home's where the main is.
    //import sudoku puzzle from file into 2d array
    int puzzle[81]; //array for sudoku puzzle state
    getPuzzle(puzzle); //fill puzzle from txt file

    return 0;

    //generate thread starting positions
    //spawn threads
    // pthread_t tids[6];
    // pthread_attr_t attrs[6];
    // for (int i=0;i<6;i++){
    //     pthread_attr_init(&attrs[i]); //init thread attributes
    //     pthread_create(&tids[i], &attrs[i], verifySudokuWorker, &testset);
    // }

    //wait for threads to complete
    // pthread_join(tid, NULL);

    //output results

}

//pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);