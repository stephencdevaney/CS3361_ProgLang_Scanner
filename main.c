#include "queue.h"
#include <string.h>
#include <ctype.h>

char* scan(FILE *);
int getCharIndex(char);
int** createTranTable();
char** createTokTable();
char** createKeyTable();
void cleanuptables(int **, char **, char **);


int main(int argc, char **argv){
    FILE *inFile;
    char *curToken;
    inFile = fopen("tokenFile1.txt", "r");
    if(inFile == NULL){
        printf("File does not exist!!!\n");
        return 0;
    }
    queue_t *tokqueue;
    tokqueue = newQueue();
    int cur_char;
    while((cur_char = fgetc(inFile)) != EOF){  // not at the end of the file
        ungetc(cur_char, inFile); // places character back into file stream
        curToken = scan(inFile);
        if(curToken == NULL) continue;
        if(strcmp(curToken, "error") == 0){
            printf("(%s)\n", curToken);
            fclose(inFile);
            return 0;
        }
        else enqueue(tokqueue, curToken);
    }
    fclose(inFile);
    if(!isempty(tokqueue)) printf("(%s", dequeue(tokqueue));
    while(!isempty(tokqueue)){
        printf(" %s", dequeue(tokqueue));
        if(!isempty(tokqueue)) printf(",");
        else printf(")");
    }
    printf("\n");
	return 0;
}


char* scan(FILE *inFile){
    // scanner setup
    int **transitionTable;
    char **tokenTable;
    char **keywordTable;
    transitionTable = createTranTable();
    tokenTable = createTokTable();
    keywordTable = createKeyTable();
    char *idImage;
    int idImageSize = 1;
    idImage = malloc(idImageSize * sizeof(char));
    int curState = 1;
    char cur_char;
    
    //scan for longest possible token
    while(((int)(cur_char = fgetc(inFile))) != EOF){
        int cur_char_index = getCharIndex(cur_char);
        if ((transitionTable[curState-1][cur_char_index]) == 0){ // recognize token
            // transtion table ingores comments and whites space
            ungetc(cur_char, inFile); // places character back into file stream
            break;
        }
        if (transitionTable[curState-1][cur_char_index] == -1){ // if automaton is stuck
            break;
        }
        else{
            curState = transitionTable[curState - 1][cur_char_index];
            if (curState == 16) {  // if the current state is id append the character on to the end of id check string
            
                idImageSize += 1;
                idImage = realloc(idImage, idImageSize);
                idImage[idImageSize - 2] = cur_char;
                idImage[idImageSize - 1] = '\0';
            }
        }
    }
    
    // output creation
    char *outputToken;
    char *tempOutputToken;
    tempOutputToken = NULL;
    if(curState == 16){ // if the current state is id check to see if the id is in the keyword table
        for(int i = 0; i < 2; i++){
            if(strcmp(keywordTable[i], idImage) == 0){
                tempOutputToken = keywordTable[i];
                break;
            }
            else tempOutputToken = tokenTable[curState-1];
        }
    }
    else{
        if(curState == 1 && (getCharIndex(cur_char) == 0 || getCharIndex(cur_char) == 1)){
            tempOutputToken = NULL;
        }
        else tempOutputToken = tokenTable[curState-1];
    }
    outputToken = malloc(7 * sizeof(char));
    if(outputToken == NULL) printf("out of memory for outputToken");
    if(tempOutputToken != NULL) strcpy(outputToken, tempOutputToken);
    else outputToken = NULL;
    
    // cleanuptables and return output
    free(idImage);
    cleanuptables(transitionTable, tokenTable, keywordTable);
    return outputToken;
}


int getCharIndex(char cur_char){
    int cur_char_index;
    switch (cur_char) {
        case ' ':
            cur_char_index = 0;
            break;
        case '\t':
            cur_char_index = 0;
            break;
        case '\n':
            cur_char_index = 1;
            break;
        case '/':
            cur_char_index = 2;
            break;
        case '*':
            cur_char_index = 3;
            break;
        case '(':
            cur_char_index = 4;
            break;
        case ')':
            cur_char_index = 5;
            break;
        case '+':
            cur_char_index = 6;
            break;
        case '-':
            cur_char_index = 7;
            break;
        case ':':
            cur_char_index = 8;
            break;
        case '=':
            cur_char_index = 9;
            break;
        case '.':
            cur_char_index = 10;
            break;
        default:
            if(isdigit(cur_char)) cur_char_index = 11;
            else if(isalpha(cur_char)) cur_char_index = 12;
            else cur_char_index = 13; 
            break;
    }
    return cur_char_index;
}


int** createTranTable(){
    int TranTable[16][14] = {
    //   w   /n  /   *   (   )   +   -   :   =   .   dig let oth      \n indcates newline and w indicates white space
        { 1,  1,  2, 10,  6,  7,  8,  9, 11, -1, 13, 14, 16, -1},  // state 1 (start state)
        { 0,  0,  3,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},  // state 2 (division)
        { 3,  1,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3},  // state 3 (comment)
        { 4,  4,  4,  5,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4},  // state 4 (comment)
        { 4,  4,  1,  5,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4},  // state 5 (comment)
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},  // state 6 (left parenthese)
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},  // state 7 (right parenthese)
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},  // state 8 (plus)
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},  // state 9 (minus)
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},  // state 10 (times)
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, 12, -1, -1, -1, -1},  // state 11 (assignment start)
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},  // state 12 (assignment end)
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 15, -1, -1},  // state 13 (decimal)
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 15, 14,  0,  0},  // state 14 (number without decimal)
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 15,  0,  0},  // state 15 (number with decimal)
        { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 16, 16,  0}   // state 16 identifer
    };
    
    // create a transition table pointer to be passed to calling function
    int ** newTranTab = malloc(16 * sizeof(int*));
    for(int i = 0; i < 16; i++){
        newTranTab[i] = malloc(14 * sizeof(int));
        for(int j = 0; j < 14; j++){
            newTranTab[i][j] = TranTable[i][j];
        }
    }
    
    return newTranTab; // return the transition table
}


char** createTokTable(){
    char TokTab[16][7] = {
        {"error"},  // state 1
        {"div"},  // state 2
        {""},  // state 3
        {""},  // state 4
        {""},  // state 5
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
    
    // create a token table pointer to be passed to calling function
    char ** newTokTab = malloc(16 * sizeof(char*));
    for(int i = 0; i < 16; i++){
        newTokTab[i] = malloc(7 * sizeof(char));
        strcpy(newTokTab[i], TokTab[i]);
    }
    
    return newTokTab; // return the token table
}


char** createKeyTable(){
    char keyTab[2][7] = {{"read\0"}, {"write\0"}};
    
    //create keyword table pointer to be passed to calling function
    char ** newKeyTab = malloc(2 * sizeof(char*));
    for(int i = 0; i < 2; i++){
        newKeyTab[i] = malloc(7 * sizeof(char));
        strcpy(newKeyTab[i], keyTab[i]);
    }
    
    return newKeyTab; // return the keyword table to calling function
}


void cleanuptables(int **transitionTable, char **tokenTable, char **keywordTable){
    //free all memory allocated for the scanner's tables
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