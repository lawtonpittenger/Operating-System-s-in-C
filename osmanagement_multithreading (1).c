
/*
Program Author: Lawton Pittenger
Course: Operating System Concepts
Assignment: Assignment 7 Multithreading with mutual exclusion
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>




// Declaration of constants
#define EXIT 0
#define INVALID -1
#define MEMORY 1
#define FILES 2
#define THREAD 3
#define FIRST 0
#define BEST 1
#define WORST 2
#define NEXT 3

// Function prototypes
int displayMenu();
void clearScreen();
void memoryManagement();
void displayProcess(int allocation[], int processes, int processSize[]);
void firstFit(int blockSize[], int blocks, int processSize[], int processes);
void worstFit(int blockSize[], int blocks, int processSize[], int processes);
void bestFit(int blockSize[], int blocks, int processSize[], int processes);
void nextFit(int blockSize[], int blocks, int processSize[], int processes);
void fileAttributes();
void printAttributes(char name[], struct stat statBuff);
void multiThreads(void);
void* threadFunction(void* vargp);


int main() {

    int choice = INVALID;

    while (choice != EXIT){

        choice = displayMenu();

        // Switch case to handle various user inputs.
        switch(choice)
        {
            case MEMORY:
                memoryManagement();
                break;

            case FILES:
                fileAttributes();
                break;

            case THREAD:
                multiThreads();
                break;

            case EXIT:
                exit(EXIT);

        }
    }
    return EXIT;
}

int displayMenu(){

    int choice = INVALID;

    while (choice == INVALID)
    {
        printf("\n\t******** Operating System Management Menu ********\n\n");
        printf("Select the OS program to run: Enter the number of your selection\n");
        printf("1. Memory Management\n");
        printf("2. File Management\n");
        printf("3. Multithreading\n");
        printf("0. Exit\n");

        scanf("%d", &choice);

        if (choice <= 0 && choice >= 3)
            choice = INVALID;
    }
    return choice;
}

void clearScreen(){

    char enter;

    printf("\n\t\t\t\t <HIT ENTER TO CONTINUE>\n\n");

    scanf("%c", &enter);

    //Windows Command
    system("cls");

    //Unix Command
    //system("clear");
}

void memoryManagement(){

    clearScreen();

    printf("\n\t******** Memory Management ******** \n\n");

    for (int algorithm = 0; algorithm <= NEXT; algorithm++){
        int blockSize[] = {15, 10, 20, 35, 80};
        int processSize[] = {10, 20, 5, 30, 65};
        int blocks = sizeof(blockSize) / sizeof(blockSize[0]);
        int processes = sizeof(processSize) / sizeof(processSize[0]);

        switch (algorithm){
            case FIRST:
                firstFit(blockSize, blocks, processSize, processes);
                break;
            case BEST:
                bestFit(blockSize, blocks, processSize, processes);
                break;
            case WORST:
                worstFit(blockSize, blocks, processSize, processes);
                break;
            case NEXT:
                nextFit(blockSize, blocks, processSize, processes);
                break;
        }
    }
}

// Function to allocate mem to blocks using next fit algo
void nextFit(int blockSize[], int blocks, int processSize[], int processes){

    printf("\n\t******** Next Fit ********\n");

    // Stores block ID of the block allocated to a process
    int allocation[processes];
    int id = 0;

    memset(allocation, INVALID, sizeof(allocation));

    for (int p = 0; p < processes; p++){

        // Don't start from beginning
        while (id < blocks){
            if (blockSize[id] >= processSize[p]){
                //Allocate block id to process
                allocation[p] = id;

                //Decrement available memory in this block
                blockSize[id] -= processSize[p];

                break;
            }

            id = (id + 1) % blocks;
        }
    }
    displayProcess(allocation, processes, processSize);
}


void firstFit(int blockSize[], int blocks, int processSize[], int processes)
{
    printf("\n\t******** First Fit ********\n");

    int allocation[processes];
    memset(allocation, INVALID, sizeof(allocation));

    for (int p = 0; p < processes; p++){
        for (int b = 0; b < blocks; b++){
            if (blockSize[b] >= processSize[b]){
                allocation[p] = b;
                blockSize[b] -= processSize[p];
                break;
            }
        }
    }
    displayProcess(allocation, processes, processSize);
}

void bestFit(int blockSize[], int blocks, int processSize[], int processes){
    printf("\n\t******** Best Fit ********\n\n");

    int allocation[processes];
    memset(allocation, INVALID, sizeof(allocation));

    for (int p = 0; p < processes; p++){
        int bestIDX = INVALID;
        for (int b = 0; b < blocks; b++){
            if (blockSize[b] >= processSize[p]){
                if (bestIDX == INVALID){
                    bestIDX = b;
                } else if (blockSize[bestIDX] > blockSize[b]){
                    bestIDX = b;
                }
            }
        }
        if (bestIDX != INVALID){
            allocation[p] = bestIDX;
            blockSize[bestIDX] -= processSize[p];
        }
    }
    displayProcess(allocation, processes, processSize);
}

void worstFit(int blockSize[], int blocks, int processSize[], int processes){
    printf("\n\t******** Worst Fit ********\n\n");

    int allocation[processes];
    memset(allocation, INVALID, sizeof(allocation));

    for (int p = 0; p < processes; p++){
        int worstIDX = INVALID;
        for (int b = 0; b < blocks; b++){
            if (blockSize[b] >= processSize[p]){
                if (worstIDX == INVALID){
                    worstIDX = b;
                } else if (blockSize[worstIDX] < blockSize[b]){
                    worstIDX = b;
                }
            }
        }
        if (worstIDX != INVALID){
            allocation[p] = worstIDX;
            blockSize[worstIDX] -= processSize[p];
        }
    }
    displayProcess(allocation, processes, processSize);

}

void displayProcess(int allocation[], int processes, int processSize[]){

    printf("\nProcess No. \tProcess Size\tBlock no.\n");

    for (int i = 0; i < processes; i++){
        printf(" %d \t\t %d \t\t", (i+1), processSize[i]);

        if (allocation[i] != INVALID)
            printf("%d\n", (allocation[i] + 1));
        else printf("Not Allocated\n");
    }

}


//Assignment 6 Code begins here

void fileAttributes(){

    //attribute structure
    struct stat statBuff;

    //error code
    int err;

    //pointer for directory entry
    struct dirent *de;

    // opendir function used to open CD
    DIR *dr = opendir(".");

    //opendir returns NULL if it couldn't open the directory
    if (dr == NULL){
        printf("Could not open current directory");
    }

    //read until readdir returns NULL showing there are no more files in directory
    //retrieve each file and store in directory entry structure
    while ((de = readdir(dr)) != NULL){
        //get the file attributes and store in the stat buffer, store the operation return value in err
        err = stat(de->d_name, &statBuff);

//if a -1 was returned, stat was unsuccessful
        if (err == -1){
            printf("\nError in stat\n");
            printf("\nPress 'Enter' to continue...");
            getchar();
        }
        printAttributes(de->d_name, statBuff);
    }
}

void printAttributes(char name[], struct stat statBuff){

    //time
    time_t t;
    //store the times of the file
    char timestr[100];

    //print file name
    printf("\n\n------------------------- FILE = %s -------------------------\n\n", name);

    //print device id
    printf("Device ID = %li\n", statBuff.st_dev);

    //print file s/n
    printf("File Serial Number = %li\n", statBuff.st_ino);

    //print file userID
    printf("File User ID = %i\n", statBuff.st_uid);

    //print file group ID
    printf("File Group ID = %i\n", statBuff.st_gid);

    //print file mode
    printf("File Mode = %i\n", statBuff.st_mode);

    //print owner permissions
    printf("Owner Permissions: ");
    if(statBuff.st_mode & S_IRUSR)
        printf("read ");
    if(statBuff.st_mode & S_IWUSR)
        printf("write ");
    if(statBuff.st_mode & S_IXUSR)
        printf("execute ");
    putchar('\n');

    //print group permissions
    printf("Group Permissions: ");
    if(statBuff.st_mode & S_IRGRP )
        printf("read ");
    if(statBuff.st_mode & S_IWGRP )
        printf("write ");
    if(statBuff.st_mode & S_IXGRP )
        printf("execute ");
    putchar('\n');

    //print other permissions
    printf("Other Permissions: ");
    if(statBuff.st_mode & S_IROTH)
        printf("read ");
    if(statBuff.st_mode & S_IWOTH)
        printf("write ");
    if(statBuff.st_mode & S_IXOTH)
        printf("execute ");
    putchar('\n');

    //print creation time
    t = statBuff.st_ctime;
    strcpy(timestr, ctime(&t));
    printf("Date Created = %s", timestr);

    //print last modification time
    t = statBuff.st_mtime;
    strcpy(timestr, ctime(&t));
    printf("Date Modified = %s", timestr);

    //print file size
    printf("File size = %li bytes", statBuff.st_size);

}

//Assignment 7 Code Begins here

//multiThreads function
void multiThreads(){

    const int SIZE = 7;
    int i;
    int error;
    pthread_t tid[7];

    for (i = 0; i < SIZE; i++){
      error = pthread_create(&tid[i], NULL, &threadFunction, (void*)&tid[i]);
        if(error != 0){
            printf("\nThread can't be created:[%s]" "Press 'Enter' to continue...\n", strerror(error));
        }
    }
    for (i = 0; i < SIZE; i++){
      pthread_join(tid[i], NULL);
    }
}

//threadFunction
void* threadFunction(void* vargp){
  const int LOOP = 10;
  int i = 0;
  int *myid = (int *)vargp;
  printf("---------------------\n");
  printf("Thread ID %d is printing\n", *myid);
  printf("---------------------\n");

  for (i = 0; i < LOOP; i++){
      printf("Thread ID %d is printing\n", *myid);
    }

  printf("\n---------------------\n");
  printf("Thread ID %d has finished\n", *myid);
  printf("---------------------\n\n\n");
  printf("---------------------\n\n\n");

  return NULL;
  }