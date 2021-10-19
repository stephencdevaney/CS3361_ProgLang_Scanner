#include "queue.h"
#include <string.h>

void scan(FILE *, char [7]);
int** createTranTable();
char** createTokTable();
char** createKeyTable();
void cleanuptables(int **, char **, char **);


int main(int argc, char **argv){
    FILE *inFile;
    char curToken[7];
    inFile = fopen("tokenFile01.txt", "r");
    if(inFile == NULL){
        scan(inFile, curToken);
        //printf("\n    %s\n", curToken);
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


void scan(FILE *inFile, char outputToken[7]){
    int **transitionTable;
    char **tokenTable;
    char **keywordTable;
    transitionTable = createTranTable();
    tokenTable = createTokTable();
    keywordTable = createKeyTable();
    cleanuptables(transitionTable, tokenTable, keywordTable);
}


int** createTranTable(){
    int TranTable[16][14] = {
    //   w   /n  /   *   (   )   +   -   :   =   .   dig let oth      \n indcates newline and w indicates white space
        { 1,  1,  2, 10,  6,  7,  8,  9, 11, -1, 13, 14, 16, -1},  // state 1
        { 0,  0,  3,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},  // state 2
        { 3,  1,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3},  // state 3
        { 4,  4,  4,  5,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4},  // state 4
        { 4,  4,  1,  5,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4},  // state 5
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},  // state 6
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},  // state 7
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},  // state 8
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},  // state 9
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},  // state 10
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // state 11
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},  // state 12
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},  // state 13
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},  // state 14
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},  // state 15
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}   // state 16
    };
    int ** newTranTab = malloc(16 * sizeof(int*));
    for(int i = 0; i < 16; i++){
        newTranTab[i] = malloc(14 * sizeof(int));
        for(int j = 0; j < 14; j++){
            newTranTab[i][j] = TranTable[i][j];
        }
    }
    return newTranTab;
}


char** createTokTable(){
    char TokTab[16][7] = {
        {"error"},  // state 1
        {"div"},  // state 2
        {"error"},  // state 3
        {"error"},  // state 4
        {"error"},  // state 5
        {"lparen"},  // state 6
        {"rparen"},  // state 7
        {"plus"},  // state 8
        {"minus"},  // state 9
        {"times"},  // state 10
        {"error"},  // state 11
        {"assign"},  // state 12
        {"error"},  // state 13
        {"number"},  // state 14
        {"number"},  // state 15
        {"id"}  // state 16
    };
    char ** newTokTab = malloc(16 * sizeof(char*));
    for(int i = 0; i < 16; i++){
        newTokTab[i] = malloc(7 * sizeof(char));
        strcpy(newTokTab[i], TokTab[i]);
    }
    return newTokTab;
}


char** createKeyTable(){
    char keyTab[2][7] = {{"read"}, {"write"}};
    char ** newKeyTab = malloc(2 * sizeof(char*));
    for(int i = 0; i < 2; i++){
        newKeyTab[i] = malloc(7 * sizeof(char));
        strcpy(newKeyTab[i], keyTab[i]);
    }
    return newKeyTab;
}


void cleanuptables(int **transitionTable, char **tokenTable, char **keywordTable){
    for(int i = 0; i < 16; i++){
        free(transitionTable[i]);
        free(tokenTable[i]);
        if( i == 0 || i == 1){
            free(keywordTable[i]);
        }
        free(transitionTable);
        free(tokenTable);
        free(keywordTable);
    }
}