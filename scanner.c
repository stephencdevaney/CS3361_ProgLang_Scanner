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
    //Name : main
    //Input : File Name, given from the command line on the console
    //Output : A queue that holds all tokens identified by the scanner and a token to check for an error
    //Purpose : Reads a file into the program and establishes a queue for the scanner
    //          Once the queue is complete, each token of the queue is printed to the screen
    FILE *inFile;
    char *curToken;
    if(argc == 2 )inFile = fopen(argv[argc-1], "r");  //Read in the text file to scan
    else{
        printf("Incorrect number of arguements!!!\n");
        return 0;
    }
    if(inFile == NULL){  //Failsafe if the input file cannot be found
        printf("File does not exist!!!\n"); 
        return 0;
    }
    queue_t *tokqueue;
    tokqueue = newQueue();
    int cur_char;
    while((cur_char = fgetc(inFile)) != EOF){  // not at the end of the file
        ungetc(cur_char, inFile); // places character back into file stream
        curToken = scan(inFile); // call scan function to get the next token
        if(curToken == NULL) continue; // if the function returns a NULL string the function is at the end of the file and found only white space of comments that need to be ignored
        if(strcmp(curToken, "error") == 0){  //Checks if the current token points to an error
            printf("(%s)\n", curToken);
            fclose(inFile);
            return 0;
        }
        else enqueue(tokqueue, curToken);  //Initializes a token to be added to the queue if no errors are found
    }
    fclose(inFile);
    //Setup to run through the completed queue and output the scanner's results to the screen
    if(!isempty(tokqueue)) printf("(%s", dequeue(tokqueue)); // decorative print
    while(!isempty(tokqueue)){
        printf(" %s", dequeue(tokqueue));  //Prints out each token by order of our queue
        if(!isempty(tokqueue)) printf(","); // decorative print
        else printf(")"); // decorative print
    }
    printf("\n");
	return 0;
}


char* scan(FILE *inFile){  
    //Name : scan
    //Input : Pointer File read in with the main function
    //Output : The token to be output to the main program
    //Purpose : scans for the next longest possible token in a text file based upon a defined automaton
    
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
    while(((int)(cur_char = fgetc(inFile))) != EOF){ // while not at the end of the file
        int cur_char_index = getCharIndex(cur_char); // convert the character to an index
        if ((transitionTable[curState-1][cur_char_index]) == 0){ // recognize token
            // transtion table ingores comments and whites space
            ungetc(cur_char, inFile); // places character back into file stream
            break;
        }
        if (transitionTable[curState-1][cur_char_index] == -1){ // if automaton is stuck
            break;
        }
        else{
            curState = transitionTable[curState - 1][cur_char_index];  // else move to next state
            if (curState == 16) {  // if the current state is id append the character on to the end of id image
            
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
                tempOutputToken = keywordTable[i];  //If in the keyword table, set the temp token to one of "read" or "write" as stored by idImage
                break;
            }
            else tempOutputToken = tokenTable[curState-1]; // if not in keyword table set token to id
        }
    }
    else{
        if(curState == 1 && (getCharIndex(cur_char) == 0 || getCharIndex(cur_char) == 1)){ // check to see if white space is at the end of the file
            tempOutputToken = NULL;
        }
        else tempOutputToken = tokenTable[curState-1]; // ready token for output
        if(strcmp(tempOutputToken, "\0") == 0) tempOutputToken = NULL; // if comment set at the end of the file ignore the comment
    }
    outputToken = malloc(7 * sizeof(char)); // allocate memory for outputToken
    if(outputToken == NULL) printf("out of memory for outputToken");
    if(tempOutputToken != NULL) strcpy(outputToken, tempOutputToken); // copy token to output token
    else outputToken = tempOutputToken; // if at the end of file and the output token is comment or white space output NULL to ignore it
    
    // cleanuptables and return output
    free(idImage);
    cleanuptables(transitionTable, tokenTable, keywordTable);
    return outputToken;
}



int getCharIndex(char cur_char){  
    //Name : getCharIndex
    //Input : a single character read in from the input file
    //Output : An integer value 0-13, given the char value input
    //Purpose : Returns an input value correlating to the character scanned in the input file; Based off the automata based scanner in the book/slides.

    int cur_char_index;
    switch (cur_char) { //Controlled using a switch case
        case ' ':  
            cur_char_index = 0;  //whitespace or tab
            break;
        case '\t':
            cur_char_index = 0;
            break;
        case '\n':
            cur_char_index = 1;  //newline
            break;
        case '/':
            cur_char_index = 2;  //division
            break;
        case '*':
            cur_char_index = 3;  //times
            break;
        case '(':
            cur_char_index = 4;  //Left Parentheses
            break;
        case ')':
            cur_char_index = 5;  //Right parentheses
            break;
        case '+':
            cur_char_index = 6;  //plus
            break;
        case '-':
            cur_char_index = 7;  //minus
            break;
        case ':':
            cur_char_index = 8;  //colon
            break;
        case '=':
            cur_char_index = 9;  //equals
            break;
        case '.':
            cur_char_index = 10; //decimal
            break;
        default:  //Default case holds the 11th and 12th case as a means to easier check for a letter or digit using isAlpha and isDigit
            if(isdigit(cur_char)) cur_char_index = 11;  //Digit
            else if(isalpha(cur_char)) cur_char_index = 12; //Letter
            else cur_char_index = 13;  //Default
            break;
    }
    return cur_char_index;
}


int** createTranTable(){
    //Name : createTranTable
    //Input : None
    //Output : A new transition table pointer
    //Purpose : Utilizes and holds the transition table simular to the one in the book (missing states 17 and 18 in order to have correct automaton); Returns a transition table pointer for the calling function
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
    //Name : createTokTable
    //Input : None
    //Output : A new token table pointer
    //Purpose : Holds an array of tokens for the transition table and scanner to reference
    char TokTab[16][7] = {
        {"error"},  // state 1
        {"div"},  // state 2
        {"\0"},  // state 3
        {"\0"},  // state 4
        {"\0"},  // state 5
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
    //Name : createKeyTable
    //Input : None
    //Output : A new Keyword table pointer
    //Purpose : Holds an small array for the scanner to reference in the case of keywords "read" and "write" being encountered
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
    //Name : cleanuptables
    //Input : The transition table, keyword table and token table
    //Output : None (Void function)
    //Purpose : Frees the memory allocated for the scanner's tables to finish the scan function.

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
