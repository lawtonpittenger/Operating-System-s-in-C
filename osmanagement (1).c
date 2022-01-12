/*
Program Author: Lawton Pittenger
Course: Operating System Concepts
Assignment: Assignment 5 OS Memory Management
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
                break;

            case THREAD:
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
