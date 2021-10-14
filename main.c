/* use in scanner function
c = fgetc(inFile);
ungetc(c, inFile);
 */

#include "queue.h"
#include <string.h>


int main(int argc, char **argv){
    FILE *inFile;
    char curToken[20];
    
    inFile = fopen("tokenFile01.txt", "r");
    if(inFile == NULL){
        printf("File does not exist!!!\n");
        return 0;
    }
    
    queue_t *tokqueue;
    tokqueue = newQueue();
    int c;
    while(!feof(inFile)){  //!feof(inFile)
        //curToken = scan(inFile);
        if(strcmp(curToken, "error") == 0){
            printf("%s\n", curToken);
            fclose(inFile);
            return 0;
        }
        enqueue(tokqueue, curToken);
    }
    fclose(inFile);
    /*while(!isempty(tokqueue)){
        printf("%s", dequeue(tokqueue));
    }*/
    printf("\n");
	return 0;
}