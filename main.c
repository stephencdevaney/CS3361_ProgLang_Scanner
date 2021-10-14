#include "queue.h"
#include <string.h>


int main(int argc, char **argv){
    FILE *inFile;
    char curToken[20];
    inFile = fopen("tokenFile1.txt", "r");
    if(inFile == NULL)
    {
        printf("File does not exist!!!");
        exit(1);
    }
    queue_t *tokqueue;
    tokqueue = newQueue();
    int c;
    c = fgetc(inFile);
    while(c != EOF){
        printf("%c", (char)c);
        printf("/n");
        //ungetc(c, inFile);
        if(strcmp(curToken, "error")){
            printf("%s", curToken);
            exit(1);
        }
        c = fgetc(inFile);
    }
	exit(0);
}