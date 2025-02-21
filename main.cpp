#include <iostream>   //prints
#include <cstdlib>    //RNG
#include <unistd.h>   //forking
#include <sys/wait.h> //wait
using namespace std;

/*
    Assignment 3 for CSC343. Written by Andrew Kehoe.
*/

int main() {

    //Fill new array with random numbers
    int nums[20];
    for(int i = 0; i < 20; i++) {
        nums[i] = rand();
    }

    //Set up the pipe
    int fd[2];
    if (pipe(fd)==-1) {
        return 1;
    }

    //Fork the processes
    pid_t pid = fork();

    if (pid < 0) { //Error Check
        perror("Fork failed");
        return 1;
    } else if (pid == 0) { //Child process
        printf("Child Process: PID = %d\n",getpid());

        //Finds min of second half of array
        int minSecond = nums[10];

        for(int i = sizeof(nums)/2; i < sizeof(nums); i++) {
            if (nums[i] < minSecond) {
                minSecond = nums[i];
            }
        }
        
        //Writes the result to the pipe
        write(fd[1],&minSecond,sizeof(int));
        close(fd[1]);

        exit(1);
    } else { //Parent process
        printf("Parent process: PID = %d\n",getpid());
        
        //Finds min of first half of array
        int minFirst = nums[0];
        for(int i = 0; i < sizeof(nums)/2; i++) {
            if (nums[i] < minFirst) {
                minFirst = nums[i];
            } 
        }

        //Reads child process' minimum from the pipe (min of second half)
        int childMin;
        read(fd[0], &childMin,sizeof(int));
        close(fd[0]);
        
        printf("The minimum number in the child is %d\nThe minimum number in the parent is %d\n",childMin,minFirst);

        wait(NULL); //Wait for child to finish
        
        //Determine and show minimum of whole array
        int totalMin;
        if (childMin < minFirst) {
            totalMin = childMin;
        } else {
            totalMin = minFirst;
        }
        printf("Minimum number of array is %d\n",totalMin);
    }

    return 0;
}
