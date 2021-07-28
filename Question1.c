/*
 -------------------------------------
 File:    a4.c
 Project: A04
 file description
 -------------------------------------
 Authors:  Muhammad Ali
 ID:      191651560
 Email:   alix1560@mylaurier.ca
 Version  2021-07-17
 -------------------------------------
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <time.h>
#include <semaphore.h>
#include <stdbool.h>

#define FILE_IN "sample4_in.txt"
#define MAX_INPUT_SIZE 256

// int allocated[4] = {}; // array to store allocated resources.
// int max[4] = {}; // array to store maximum required resources.
// int available[4] = {}; // array to store available resources.
// int needs[4] = {}; // array to store how many more resources are needed.

int customer;
int resource_type;
int safe;

int **allocated; // point to allocated resources
int **max; // point to max resources
int *available; // point to available resources
int **needs; // point to resources still needed

// typedef struct customer{
//     int customerID;
    
// } Customer;

// from a2 q3, repurposed
int **readFile(char* fileName)//use this method in a suitable way to read file
{
	FILE *in = fopen(fileName, "r");
	if(!in)
	{
		printf("Child A: Error in opening input file...exiting with error code -1\n");
		return -1;
	}

	struct stat st;
	fstat(fileno(in), &st);
	char* fileContent = (char*)malloc(((int)st.st_size+1)* sizeof(char));
	fileContent[0]='\0';	
	while(!feof(in))
	{
		char line[100];
		if(fgets(line,100,in)!=NULL)
		{
			strncat(fileContent,line,strlen(line));
		}
	}
	fclose(in);

	char* command = NULL;
	//int threadCount = 0;
	char* fileCopy = (char*)malloc((strlen(fileContent)+1)*sizeof(char));
	strcpy(fileCopy,fileContent);
	command = strtok(fileCopy,"\r\n");
	while(command!=NULL)
	{
		customer++;
		command = strtok(NULL,"\r\n");
	}
	//*threads = (Thread*) malloc(sizeof(Thread)*threadCount);
    strcpy(fileCopy, fileContent);
	char* lines[customer];
	//command = NULL;
	int i=0;
	command = strtok(fileCopy,"\r\n");
	while(command!=NULL)
	{
		lines[i] = malloc(sizeof(command)*sizeof(char));
		strcpy(lines[i],command);
		i++;
		command = strtok(NULL,"\r\n");
	}

    int **max = malloc(sizeof(int *) * customer);

    for (int k = 0; k < customer; k++){

        int *temp = malloc(sizeof(int) * resource);
        char *token = NULL;

        int j = 0;
        
        token = strtok(lines[k], ",");
        while (token!=NULL){
            temp[j] = atoi(token);
            j++;
            token = strtok(NULL, ",");
        }

        max[k] = temp;

    }

    return max;

    

// 	for(int k=0; k<threadCount; k++)
// 	{
// 		char* token = NULL;
// 		//int j = 0;
// 		token =  strtok(lines[k],";");
// 		while(token!=NULL)
// 		{
// 			strncpy((*threads + k) -> tid, token, 3); //copy thread id

// 			token = strtok(NULL, ";");

// 			if (token){
// 				(*threads + k) -> start_time = strtol(token, NULL, 10);
// 			}

// 			if (token){
// 				(*threads + k) -> lifetime = strtol(token, NULL, 10);
// 			}

// 			(*threads + k) -> executed = 0;

// //this loop tokenizes each line of input file
// //write your code here to populate instances of Thread to build a collection
// 		}
// 	}
// 	return threadCount;
}


int main(int argc, char *argv[]){

}

// safety function will be used to run the safety algorithm
int *safety(){

}

