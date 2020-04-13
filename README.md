Nicklaus Wayne Settoon
894473560
nsetto3@lsu.edu

DOCTORS EXCUSE:
Please see the .pdf file included with the submission. I will also be sending an email with it.
Sorry for the inconvenience. I take whatever judgment is deemed necessary.

COMPILATION:
$ gcc -pthread -g ./main.c -o ./main

RUNNING:
run using "valid.txt", "invalid.txt", or any other similarly formatted .txt file as an argument to the program like so:
$ ./main FILE_NAME.txt

OUTPUT:
Expected output should display the sudoku in a square, pretty much exactly as in the txt file. Following the puzzle print, should be 27 lines containing the information about the threads which executed their checks.
For valid.txt, all lines should say VALID.
For invalid.txt, Rows:2,5,9, Columns:3,5,7 and Boxes:3,5,7 should be INVALID. The rest should be valid.

DESCRIPTION:
First the puzzle is imported and stored as a single dimensional array of ints.

The program creates 27 threads with each having a structure dubbed a "desk", "workstation", or "station" containing all the info the thread needs to determine where to scan the puzzle array.

"Boxes" are the subgrids, laid out in order from left to right, top to bottom. So, first top-left, top-middle, top-right, then middle-left, middle-middle, etc...

A single thread function is split into three verification patterns via an enum/switch case.

Each worker case also has three further switch cases for printing its message to stdout. These printf statements are wrapped in a global mutex lock to ensure no race conditions when outputting.
