/*
* 
* @ project no. 2 - Working with Data Structures
* @ deadline: 12/6/2020
* @ name:  Tadeas Kachyna
* @ login: xkachy00
*
* About: This project is about working with a dynamically allocated table and its data.
*
* Translation - "gcc -std=c99 -Wall -Wextra -Werror sps.c -o sps"
*
* Execution syntax - "./sps [-d DELIM] CMD_SEQUENCE FILE"
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXARG 1000 // maximum size of an argument

// ---------- OTHER FUNCTIONS --------- //

// Function NO MEMORY - tells you, if there is no memory for dynamic allocation
void noMemory(){
    fprintf(stderr,"ERROR > No memory for dynamic array.\n");
    exit(1); 
}

// Function MEMORY ALLOCATION - dynamically alocating memory
char ***memoryAllocation(char ***table, int tabRowsDef, int tabColumnsDef, int tabChars){
    int m,n;

    table = calloc(tabRowsDef,sizeof(char **));
        if(table == NULL) {
            noMemory();
        }
    
    for (m = 0; m < tabRowsDef; ++m){
        table[m] = calloc(tabColumnsDef,sizeof(char *));
        if(table[m] == NULL) {
            noMemory();
        } 
    }

    for (m = 0; m < tabRowsDef; ++m){                           
        for (n = 0; n < tabColumnsDef; ++n){
            table[m][n] = calloc(tabChars + 1,sizeof(char)); 
            if(table[m][n] == NULL) {
                noMemory();    
            }   
        }
    }

    return table;
}

// Function FREE MEMORY - we should free up each dynamically allocated memory
void freeMemory(char ***table, int tabRowsDef, int tabColumnsDef){

    // first it deletes cells, then columns, then rows
    int i, j;
    for (i = 0; i < tabRowsDef; ++i){                              
        for (j = 0; j < tabColumnsDef; ++j){
            free(table[i][j]);        
        }
    }
    for (i = 0; i < tabRowsDef; ++i){        
        free(table[i]);
    }

    free(table); 
}

// Function FILLING UP THE TABLE
char ***fillingTable(char ***table, char *delim, FILE *file){
    int d; // delimiter, bool values, 0 = no, 1 = yes;
    char buffer;
    int charCounterCell = 0; // counts characters in cells
    int qmCounter = 0; // counts quoation marks
    int columnsCounter= 0; // counts columns
    int rows = 0, columns = 0, maxNoChar = 0;

    // loop goes through cycle and reads every character
    while ((buffer = fgetc(file)) != EOF) {   
         
        if (buffer == '\"'){    // counts quoation marks
            qmCounter++; 
        }

        d = 0;
        for (int r = 0; delim[r] != '\0'; r++) {   // delimeter indentification
            if (buffer == delim[r]){
                d = 1;
            }   
        }                    
           
        if (qmCounter % 2 == 0){                       
            if (d == 1){                                                        // jumps to the next cell
                if (charCounterCell > maxNoChar) maxNoChar = charCounterCell;   // max chars in the cell                          
                columnsCounter++,charCounterCell = 0;                           // counts columns in the row                                          
            }        
        }

        if(buffer == '\n'){                             // jumps to the next row
            rows++;                                     // counts rows in the table
            if (charCounterCell > maxNoChar) {          // max chars in the cell
                maxNoChar = charCounterCell;
            }   
            if (columnsCounter > columns ) {            // max columns in the row
                columns = columnsCounter; 
            }  
            charCounterCell = 0,columnsCounter = 0;  
           
        }
        if (d == 0 && buffer != '\n'){                                                  // counts chars in the cell
            if (buffer != '\"'){                                                        // removes "
                table[rows][columnsCounter][charCounterCell] = buffer;
                charCounterCell++;
            } 
        }    

        if (d == 1 && qmCounter % 2 != 0 ) {  
            table[rows][columnsCounter][charCounterCell] = buffer;
            charCounterCell++;
        }       
    }

    return table;
}

// Function ARGUMENT CHECKING
void argumentChecking(int argc) {

    // checks entered arguments
    if (argc < 2) {
        fprintf(stderr, "ERROR > Please enter more arguments.\n");
        exit(1);
    }
}

// Function DELIMITER - sets delimiter
char *delimiter(char **argv) { 
    char *delim;
    if (strcmp(argv[1],"-d") == 0){
        delim = argv[2];
    } else {
        delim = " ";
    }  
    return delim;
}

// Function ARGUMENT PARSING
int argumentParsing(char **commandsArgument, char **argv, int argc){
    int cmdNo = 0;
    char *p;
    p = strtok(argv[argc-2], ";");
    
    while (p != NULL) {                     // splitting of arguments
        commandsArgument[cmdNo++] = p;
        p = strtok(NULL, ";");
    }
    return cmdNo;
}

// Function ERROR - handles errors
void error(char **commandsArgument, char **argv, int argc, int cmdNo, FILE *file){
    int k;
    // checks the number of commands
    if (cmdNo > 1000) {
        fprintf(stderr, "ERROR > Too many arguments. Shutting down the program.\n");
        free(commandsArgument);
        exit(1);
    }

    // checks the length of each argument
    for (k = 0; k < cmdNo; k++) {
        int len = strlen(commandsArgument[k]);
        if (len > 1000) {
            fprintf(stderr, "ERROR > Too long argument. Maximum length is 1000.\n");
            free(commandsArgument);
            exit(1);
        }
    }

    // list of commands
    char *commands[] = {
    "irow", "arow", "drow", "icol", "acol", "dcol", 
    "set", "clear", "swap", "sum", "avg", "count", "len",
    "def", "use", "inc", "["};
    
    int counter = 0; // if counter is later equal to zero, it means cycle did not find any matches with user's input and possible commands
    // this cycle loops through every subargument and list of commands and compare them
    for(int k = 0; k < cmdNo; k++){
        counter = 0;
        for (int l = 0; l < 17; l++){
            if(strstr(commandsArgument[k], commands[l])){
                counter++;
            } 
        }
        if (counter == 0) {
            fprintf(stderr, "ERROR > Wrong entered argument %s.\n", commandsArgument[k]);
            free(commandsArgument);
            exit(1);
        }   
    }
    
    // checks correctness of the last argument
    char *lastArgument = strstr(argv[argc-1], ".txt");
    if(lastArgument == NULL){
        fprintf(stderr, "ERROR > Last argument supposed to a text file.\n");
        free(commandsArgument);
        exit(1);
    }

    // checks whether the file exists or not
    if (!file) {
        fprintf(stderr, "ERROR > File does not exist.\n");
        free(commandsArgument);
        exit(1);
    }

    // if so, then it checks, if it is not empty
    fseek(file, 0, SEEK_END);                                      
    if (ftell(file) == 0){
        fprintf(stderr,"ERROR > Empty file.\n");
        free(commandsArgument);
        exit(1);
    }
    
    // sets the file position to the beginning of the file 
    rewind(file);
}

// ---------- TYPEDEF STRUCTURES ---------- //

typedef struct tab {
    int rows;           // number of rows
    int columns;        // number of columns
    int maxNoChar;      // maximum number of characters
} tab;

typedef struct selection {
    // variables for all functions
    int numR1, numC1, numR2, numC2;     
    // variables for function "set"     
    int numR1set, numC1set, numR2set, numC2set;  
    // variables for function [MIN]
    int numR1min, numC1min, numR2min, numC2min;    
    // variables for function [MAX]
    int numR1max, numC1max, numR2max, numC2max; 
    // variables for function [find STR] 
    int numR1str, numC1str, numR2str, numC2str; 
    int tabCdef;
    int tabRdef;
    char ***table;
    FILE *file;      
} selection;

typedef struct function {
    double numSum;
    int numCount;
    int count;
    int len;
    int numA;
    int numB;
} function;

// ---------- FUNCTIONS ASSOCIATED WITH STRUCTURES---------- //

// Structure Function SCAN FILE - scans file and counts number of rows and colums
tab scanningFile(char *delim, FILE *file){
    tab t = {0,0,0};
    char buffer;
     // d = delimiter, charCounterCell = counts number of characters in a cell, qmCounter = counts quotation marks, columnsCOunter = counts columns
    int d, charCounterCell = 0, qmCounter = 0, columnsCounter = 0; 
    
    while ((buffer = fgetc(file)) != EOF) {  
        if (buffer == '\"'){ 
            qmCounter++; // counts quoation marks
        }  
          
        d = 0;
        for (int r = 0; delim[r] != '\0'; r++) {  // detects delimiters
            if (buffer == delim[r]){
                d = 1;
                
            }   
        }                    
           
        if (qmCounter % 2 == 0){ // detects the biggest cell in the table                
            if (d == 1){                                                       
                if (charCounterCell > t.maxNoChar){ 
                    t.maxNoChar = charCounterCell;           
                }                          
                columnsCounter++,charCounterCell = 0;                                                                 
            }        
        }

        if(buffer == '\n'){                           
            t.rows++;   // counts row in the table                                    
            if (charCounterCell > t.maxNoChar){  
                t.maxNoChar = charCounterCell; 
            }  
            if (columnsCounter > t.columns ) { // counts columns in the table
                t.columns = columnsCounter;  
            } 
            charCounterCell = 0,columnsCounter = 0;    
        }
        if (d == 0 && buffer != '\n') charCounterCell++;                    // counts chars in the next cell
        if (d == 1 && qmCounter % 2 != 0 ) charCounterCell++;               // adds delimiters inside the brackets and 2x"
        if (charCounterCell > t.maxNoChar) t.maxNoChar = charCounterCell;   // maximum characters in the cell      
    }
    if (columnsCounter > t.columns ) t.columns = columnsCounter;             // maximum columns in the last row
    return t;
}

// Structure Function SELECTION OF ROWS & COLUMS- highlights specific cells defined by a user via command line
selection selectionRowsColumns(char **commandsArgument, int tabRows, int tabColumns, int k, selection a){
   int commaCount = 0, i;

    for (i = k-1; i >= 0; i--) {   // search input values
        if (strstr(commandsArgument[i],"[set]"));
        else if(commandsArgument[i][0] == '[') {
            break;
        }
    }
    
    if (i < 0) {
        i = 0;
    }

    for (unsigned long a = 0; a < strlen(commandsArgument[i]); a++){     // counts commas
        if (commandsArgument[i][a] == ','){
                commaCount++; 
            }  
    }

    // [_] - selection of temporary
    if (strstr(commandsArgument[i],"[_]")){   
        a.numR1 = a.numR1set, a.numC1 = a.numC1set, a.numR2 = a.numR2set, a.numC2 = a.numC2set;
    
    // [min] - selection of min
    }else if (strstr(commandsArgument[i],"[min]")){   
        a.numR1 = a.numR1min, a.numC1 = a.numC1min, a.numR2 = a.numR2min, a.numC2 = a.numC2min;

    // [max] - selection of max
    }else if (strstr(commandsArgument[i],"[max]")){   
        a.numR1 = a.numR1max, a.numC1 = a.numC1max, a.numR2 = a.numR2max, a.numC2 = a.numC2max;

    // [str] - selection of find STR
    }else if (strstr(commandsArgument[i],"[find")){   
        a.numR1 = a.numR1str, a.numC1 = a.numC1str, a.numR2 = a.numR2str, a.numC2 = a.numC2str;

    // [_,_] - selection of whole table
    } else if (strstr(commandsArgument[i],"[_,_]")){   
        a.numR1 = 0, a.numC1 = 0, a.numR2 = tabRows - 1, a.numC2 = tabColumns - 1;
    
    // [R,C] or [R,_] or [_,C]
    } else if (commaCount == 1){
        char *endptr,*endptr1;                             
        a.numR1 = (strtol(commandsArgument[i]+1,&endptr,10))-1;
        if(endptr[0]=='_'){
            a.numR1 = 0;
            a.numR2 = tabRows - 1;
            endptr = endptr + 1;
        }else{
            a.numR2 = a.numR1;
        } 
        
        a.numC1 =(strtol(endptr+1,&endptr1,10))-1;
        if(endptr1[0]=='_'){
            a.numC1 = 0;
            a.numC2 = tabColumns - 1;
        }else{
            a.numC2 = a.numC1;
        }      

        if (a.numR1 && a.numR2 >= a.tabRdef){
            fprintf(stderr, "ERROR > Input Error. Unfortunately my table is not inflatable. Maximum number of rows is %d.\n", a.tabRdef);
            free(commandsArgument);
            freeMemory(a.table, a.tabRdef, a.tabCdef);
            fclose(a.file);
            exit(1);
        }
        if (a.numC1 && a.numC2 >= a.tabCdef){
            fprintf(stderr, "ERROR > Input Error. Unfortunately my table is not inflatable. Maximum number of columns is %d.\n", a.tabCdef);
            free(commandsArgument);
            freeMemory(a.table, a.tabRdef, a.tabCdef);
            fclose(a.file);
            exit(1);
        }
             

    // [R1,C1,R2,C2]
    } else if (commaCount == 3){
        char *endptr,*endptr1,*endptr2,*endptr3;                             
        a.numR1 = (strtol(commandsArgument[i]+1,&endptr,10))-1;
        if(endptr[0]=='-'){
            a.numR1 = 0;
            endptr = endptr+1;
        }
        
        a.numC1 = (strtol(endptr+1,&endptr1,10))-1;
        if(endptr1[0]=='-'){
            a.numC1 = 0;
            endptr1 = endptr1+1;
        }

        a.numR2 = (strtol(endptr1+1,&endptr2,10))-1;
        if(endptr2[0]=='-'){
            a.numR2 = tabRows - 1;
            endptr2 = endptr2+1;
        }  
        
        a.numC2 = (strtol(endptr2+1,&endptr3,10))-1;
        if(endptr3[0] == '-'){
            a.numC2 = tabColumns - 1;
        }   

        if (a.numR1 > a.numR2 || a.numC1 > a.numC2) {
            fprintf(stderr, "ERROR > Input Error. First number supposed to be bigger than the second one.\n");
            free(commandsArgument);
            freeMemory(a.table,  a.tabRdef, a.tabCdef);
            fclose(a.file);
            exit(1);
        }
        if (a.numR1 && a.numR2 >= a.tabRdef){
            fprintf(stderr, "ERROR > Input Error. Unfortunately my table is not inflatable. Maximum index of row is %d.\n", a.tabRdef);
            free(commandsArgument);
            freeMemory(a.table, a.tabRdef, a.tabCdef);
            fclose(a.file);
            exit(1);
        }
        if (a.numC1 && a.numC2 >= a.tabCdef){
            fprintf(stderr, "ERROR > Input Error. Unfortunately my table is not inflatable. Maximum index of column is %d.\n", a.tabCdef);
            free(commandsArgument);
            freeMemory(a.table, a.tabRdef, a.tabCdef);
            fclose(a.file);
            exit(1);
        }
    
    }


    // selection of [set]
    if (strstr(commandsArgument[k],"[set]")){   
        a.numR1set = a.numR1, a.numC1set = a.numC1, a.numR2set = a.numR2, a.numC2set = a.numC2;
    }
    return a;
}

// Structure Function CONTENT EDITING - provides basic structure for functions - SUM, AVG, COUNT, LEN
function contentEditing(char ***table, char **commandsArgument, int tabRows, int tabColumns, int k,selection a) {
    function s = {0,0,0,0,0,0};
    a = selectionRowsColumns(commandsArgument, tabRows, tabColumns, k, a);

    s.len = 0; // for LEN f.
    s.numSum = 0; // for SUM f.

    // getting values of each cell
    for (int u = a.numR1; u <= a.numR2; u++){                          
        for (int v = a.numC1; v <= a.numC2; v++){
            char *endptr;                             
            double num  = strtod(table[u][v],&endptr);
            if(strlen(endptr)!=0){
                num = 0;
            }
            if(strcmp(table[u][v], "") != 0) {
                s.count++;
            }  
            s.len = strlen(table[u][v]);
            s.numSum +=num;
            s.numCount++;  
        }   
    }
    
    // geting numbers of position in the table, where the result should be stored
    int counter = 0;
    char *ptr = commandsArgument[k];
    while (*ptr) {
        if (isdigit(*ptr)) { 
            if (counter == 0) {
                s.numA = strtol(ptr, &ptr, 10)-1; 
                counter++;
            } else if (counter == 1 ) {
                s.numB = strtol(ptr, &ptr, 10)-1;  
            }
        } else { 
            ptr++; 
        } 
    }

    return s;
}


// ---------- TABLE AND CELL EDITING FUNCTIONS ---------- //

// Function IROW - inserts a row above the selected row
void irow(char ***table, char **commandsArgument, int tabRows, int tabColumns, int k, selection a) {
    a = selectionRowsColumns(commandsArgument,tabRows, tabColumns, k, a); 
    // moves the table to the end   
    for (int u = tabRows - 1; u > a.numR1; u--){                           
        for (int v = 0; v < tabColumns; v++){
            strcpy(table[u][v],table[u-1][v]); 
        }   
    }
    // deletes the content of the insert row 
    for (int u = a.numR1; u == a.numR1; u--){                            
        for (int v = 0; v < tabColumns; v++){
        strcpy(table[u][v],""); 
        }   
    }
}

// Function AROW - inserts a row below the selected row
void arow(char ***table, char **commandsArgument, int tabRows, int tabColumns, int k, selection a) {
    a = selectionRowsColumns(commandsArgument,tabRows, tabColumns, k, a);  
    // moves the table one row below  
    for (int u = tabRows - 1; u > 0; u--){                    
        for (int v = 0; v < tabColumns; v++){
            strcpy(table[u][v],table[u-1][v]); 
        }   
    }
    // moves the beginning of the table up    
    for (int u = 0; u < a.numR1+1; u++){                            
        for (int v = 0; v < tabColumns; v++){
            strcpy(table[u][v],table[u+1][v]); 
        }   
    }
    // deletes the content of the inserted row   
    for (int u = a.numR1+1; u == a.numR1+1; u--){             
        for (int v = 0; v < tabColumns; v++){
        strcpy(table[u][v],""); 
        }   
    }
}

// Function DROW - deletes selected rows
int drow(char ***table, int tabRows, int tabColumns, int iRow, int iRow1) {
    // removes the content of the selected rows    
    for (int u = iRow; u <= iRow1; u++){                                             
        for (int v = 0; v < tabColumns; v++){
        strcpy(table[u][v],""); 
        }   
    }  
    // moves the end of table up       
    for (int u = iRow; u <= tabRows-1-(iRow1-iRow)-1; u++){                      
        for (int v = 0; v < tabColumns; v++){
            strcpy(table[u][v],table[u+1+(iRow1-iRow)][v]);
        }
    }
     // deletes the end of the table  
    for (int u = tabRows - (iRow1-iRow) - 1; u <= tabRows - 1; u++){                            
        for (int v = 0; v < tabColumns; v++){
        strcpy(table[u][v],""); 
        }   
    }
    // returns the final value of the current value of rows 
    tabRows = tabRows - (iRow1-iRow) - 1; 
    return tabRows;
}

// Function ICOL - inserts a column to the left
void icol(char ***table, char **commandsArgument, int tabRows, int tabColumns, int k, selection a){
    a = selectionRowsColumns(commandsArgument,tabRows, tabColumns, k, a); 
    // moves the table to the right
    for (int u = 0; u < tabRows; u++){                              
        for (int v = tabColumns - 1; v > a.numC1; v--){
            strcpy(table[u][v],table[u][v-1]); 
        }   
    }
    // sets up the values of the added column
    for (int u = 0; u < tabRows; u++){                                 
        for (int v = a.numC1; v == a.numC1; v++){
        strcpy(table[u][v],""); 
        }   
    }
}

// Function ACOL - inserts a columnt to the right
void acol(char ***table, char **commandsArgument, int tabRows, int tabColumns, int k, selection a){
     a = selectionRowsColumns(commandsArgument,tabRows, tabColumns, k, a); 
    // mvoves the whole table to the right
    for (int u = 0; u < tabRows; u++){                         
        for (int v = tabColumns - 1; 0 < v; v--){
            strcpy(table[u][v],table[u][v-1]); 
        }   
    }
    // moves the beginning of the table to the left
    for (int u = 0; u < tabRows - 1; u++){                             
        for (int v = 0; v < a.numC1+1; v++){
            strcpy(table[u][v],table[u][v+1]); 
        }   
    }
    // sets the value of the added column
    for (int u = 0; u < tabRows; u++){                                 
        for (int v = a.numC1+1; v == a.numC1+1; v++){
        strcpy(table[u][v],""); 
        }   
    }
}

// Function DCOL - deletes selected columns
int dcol(char ***table, int tabRows, int tabColumns, int iCol, int iCol1) {
    // deletes the content of the column    
    for (int u = 0; u < tabRows; u++){                                        
        for (int v = iCol; v <= iCol1; v++){
        strcpy(table[u][v],""); 
        }   
    } 
    // moves the beginning of the table to the left   
    for (int u = 0; u < tabRows; u++){                                       
        for (int v = iCol; v <= tabColumns - 1 - (iCol1 - iCol)-  1; v++){
            strcpy(table[u][v],table[u][v+1+(iCol1-iCol)]); 
        }   
    }
    // deletes the columns on the right side 
    for (int u = 0; u < tabRows; u++){                                                 
        for (int v = tabColumns - (iCol1-iCol)-1; v <= tabColumns - 1; v++){
        strcpy(table[u][v],""); 
        }   
    }
    // calculates final number od columns and returns it 
    tabColumns = tabColumns - (iCol1-iCol)-1;
    return tabColumns;
}

// Function SET - sets the value of the cell to a string STR
void set(char ***table, char **commandsArgument, int tabRows, int tabColumns, int tabCh, int k, selection a) {
    a = selectionRowsColumns(commandsArgument,tabRows, tabColumns, k, a); 

    // substratc first four characters "set "
    char *temp = commandsArgument[k] + 4; 
    int len = strlen(temp);

    // sizes up the current size of cells
    if (len > tabCh) {
        for (int u = a.numR1; u <= a.numR2; u++){                       
            for (int v = a.numC1; v <= a.numC2; v++){
                char *ptr = realloc(table[u][v], len+1 * sizeof(char));
                if(ptr == NULL) {
                    noMemory();
                } else {
                    table[u][v] = ptr;
                    strcpy(table[u][v],temp);
                }
                
            }   
        }
    // otherwise there is no change
    } else {
        for (int u = a.numR1; u <= a.numR2; u++){  // sets the string to the selected cells                        
            for (int v = a.numC1; v <= a.numC2; v++){
                strcpy(table[u][v],temp);
            }
        }
    }  
}

// Function CLEAR - the contents of the selected cells will be deleted
void clear(char ***table, char **commandsArgument, int tabRows, int tabColumns, int k, selection a){
    a = selectionRowsColumns(commandsArgument,tabRows, tabColumns, k, a);  
    // clears the content of the selected cells   
    for (int u = a.numR1; u <= a.numR2; u++){                     
        for (int v = a.numC1; v <= a.numC2; v++){
            strcpy(table[u][v],"");
        }   
    }
}

// Function SWAP - replaces the contents of the selected cell with the cell in row R and column C 
void swap(char ***table, char **commandsArgument, int tabRows, int tabColumns, int k, selection a){
    int numA,numB,counter = 0;
    char *ptr = commandsArgument[k];
    while (*ptr) {
        if (isdigit(*ptr)) { 
            if (counter == 0) {
                numA = strtol(ptr, &ptr, 10)-1; 
                counter++;
            } else if (counter == 1 ) {
                numB = strtol(ptr, &ptr, 10)-1;  
            }
        } else { 
            ptr++; 
        } 
    }

    char temp[MAXARG];
    char temp2[MAXARG];

    a = selectionRowsColumns(commandsArgument,tabRows, tabColumns, k, a); 

    // swaps selected cells 
    for (int u = a.numR1; u <= a.numR2; u++){                        
        for (int v = a.numC1; v <= a.numC2; v++){
            strcpy(temp,table[u][v]);
            strcpy(temp2,table[numA][numB]);
            strcpy(table[numA][numB],temp);
            strcpy(table[u][v],temp2);
        }   
    }
}

// Function SUM - stores the sum of the values of the selected cells in the cell on row R and column C
void sum(char ***table, char **commandsArgument, int tabRows, int tabColumns, int k, selection a){
    function s;
    s = contentEditing(table, commandsArgument, tabRows, tabColumns, k, a); 
    // calculates the sum and saves it to the selected cell
    char str[MAXARG];
    sprintf(str,"%g",s.numSum);
    strcpy(table[s.numA][s.numB],str);
}

// Function AVG - same as sum, but the arithmetic mean of the selected cells is stored
void avg(char ***table, char **commandsArgument, int tabRows, int tabColumns, int k, selection a){
    function s;
    s = contentEditing(table, commandsArgument, tabRows, tabColumns, k, a);  
    char str[MAXARG];
    // calculates average and saves it to the selected cell
    double avg = s.numSum/s.numCount;
    sprintf(str,"%g",avg);
    strcpy(table[s.numA][s.numB],str);
}

// Function COUNT - same as sum, but stores the number of non-empty cells from the selected cells
void count(char ***table, char **commandsArgument, int tabRows, int tabColumns, int k, selection a){
    function s;
    s = contentEditing(table, commandsArgument, tabRows, tabColumns, k, a); 
    char str[MAXARG];
    sprintf(str,"%d",s.count);
    strcpy(table[s.numA][s.numB],str);
}

// Function LEN - stores the string length of the currently selected cell in the cell on row R and column C
void len(char ***table, char **commandsArgument, int tabRows, int tabColumns, int k, selection a){
    function s;
    s = contentEditing(table, commandsArgument, tabRows, tabColumns, k, a);  
    char str[MAXARG];
    sprintf(str,"%d",s.len);
    strcpy(table[s.numA][s.numB],str);
}

// Selection [max] - looks for a cell with maximal value
selection max(char ***table, char **commandsArgument, int tabRows, int tabColumns, int k, selection a) {
    a = selectionRowsColumns(commandsArgument,tabRows, tabColumns, k, a); 
    double current = 0, max, min = 0;

    // first, it gets the minimal value
    for (int u = a.numR1; u <= a.numR2; u++){                          
        for (int v = a.numC1; v <= a.numC2; v++){
            current = 0;
            char *endptr; 
            current = strtod(table[u][v],&endptr);
            if((current < min) && (strlen(endptr)==0)){
                min = current;   
            } 
        }           
    }

    max = min; // then it saves it to  max 
    // now it is comparing every value with the lowest one
    for (int u = a.numR1; u <= a.numR2; u++){                          
        for (int v = a.numC1; v <= a.numC2; v++){
            current = 0;
            char *endptr; 
            current = strtod(table[u][v],&endptr);
            if((current > max) && (strlen(endptr)==0)){
                max = current;   
            } 
        }           
    }
    
    // now it saves its first coordinates
    for (int u = a.numR1; u <= a.numR2; u++){                          
        for (int v = a.numC1; v <= a.numC2; v++){
            current = 0; 
            char *endptr; 
            current = strtod(table[u][v],&endptr);
            if((current == max) && (strlen(endptr)==0)) {
                a.numR1max = u, a.numC1max = v, a.numR2max = u, a.numC2max = v;
                // using goto to easily jump out of nested loops
                goto maxSearchEnd;   
            } 
        }           
    }
    maxSearchEnd: max = 0;
    return a;
}

// Selection [min] - looks for a cell with minimal value
selection min(char ***table, char **commandsArgument, int tabRows, int tabColumns, int k, selection a){
    a = selectionRowsColumns(commandsArgument,tabRows, tabColumns, k, a); 
    double current = 0, min,max = 0;

    // first, it gets the maximal value
    for (int u = a.numR1; u <= a.numR2; u++){             
        for (int v = a.numC1; v <= a.numC2; v++){
            current = 0;
            char *endptr; 
            current = strtod(table[u][v],&endptr);
            if((current > max) && (strlen(endptr)==0)){
                max = current;   
            } 
        }           
    }
    min = max; // then it saves it to min
    // now it is comparing every value with the biggest one
    for (int u = a.numR1; u <= a.numR2; u++){                          
        for (int v = a.numC1; v <= a.numC2; v++){
            current = 0;
            char *endptr; 
            current = strtod(table[u][v],&endptr);
            if (strlen(table[u][v]) == 0);
            else if((current < min) && (strlen(endptr)==0)){
                min = current;  
            } 
        }           
    }

     // now it saves its first coordinates
    for (int u = a.numR1; u <= a.numR2; u++){                          
        for (int v = a.numC1; v <= a.numC2; v++){ 
            current = 0;
            char *endptr; 
            current = strtod(table[u][v],&endptr);
            if (strlen(table[u][v]) == 0);
            else if((current == min) && (strlen(endptr)==0)) {
                a.numR1min = u, a.numC1min = v, a.numR2min = u, a.numC2min = v;
                // using goto to easily jump out of nested loops
                goto minSearchEnd;   
            } 
        }           
    }
    minSearchEnd: min = 0;
    return a;
}

// Selection [find STR] - looks for specific string in the table
selection findString(char ***table, char **commandsArgument, int tabRows, int tabColumns, int k, selection a){
    a = selectionRowsColumns(commandsArgument,tabRows, tabColumns, k, a); 
    // goes through whole table and looks for the string
    for (int u = a.numR1; u <= a.numR2; u++){                          
        for (int v = a.numC1; v <= a.numC2; v++){
            char *temp = strrchr(commandsArgument[k]+6, ']');
            if(temp) {
                *temp = '\0';
            }
            if (strstr(table[u][v],commandsArgument[k]+6)){
                a.numR1str = u, a.numC1str = v, a.numR2str = u, a.numC2str = v;
                // using goto to easily jump out of nested loops
                goto End;
            } 
            
        }           
    }

    End: return a;
}

// Function DEF INC - subfunction for function DEF and INC
int DefInc(char **commandsArgument, int k){

    // first it gets the number of a temporary variable 
    int varNum;
    char *ptr = commandsArgument[k];
    while (*ptr) {
        if (isdigit(*ptr)) { 
            varNum = strtol(ptr, &ptr , 10); 
        } else { 
            ptr++; 
        } 
    }
    if (varNum > 9 || varNum < 0) {
        fprintf(stderr, "WARNING > You can choose only variables with numbers from 0 to 9. Your selected cells will be automatically saved to variable with number 0.\n");
        varNum = 0;
    }

    return varNum;
}

// Function MAIN
int main(int argc, char *argv[]) {

    selection a = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // sets all values in structure to zero
    argumentChecking(argc);                                          // checks arguments
    char *delim = delimiter(argv);                                   // sets delimiter
    int length = strlen(argv[argc-2]);                               // gets length of the CMD_SEQUENCE argument   
    char **commandsArgument = malloc(length * sizeof(char *));       // allocates memory for CMD_SEQUENCE argument
    int cmdNo = argumentParsing(commandsArgument, argv, argc);       // tokenizes CMD_SEQUENCE argument
    FILE *file = fopen(argv[argc-1],"r");                            // opens .txt file which is the last argument         
    error(commandsArgument, argv, argc, cmdNo, file);                // error handling
    tab t = scanningFile(delim, file);                               // scans .txt file, gets info about number of columns, row etc.
    rewind(file);                                                    // jumps back to the start of a .txt file
    
    // this loop checks CMD_SEQUENCE, wheter there is a occurance of one of these functions. If so, it adds +1 row or column to memory allocation.
    int tabRowAdd = 1, tabColumnAdd = 1;
    for (int i = 0; i < cmdNo; i++){             
        if ((strcmp(commandsArgument[i], "irow") == 0) || (strcmp(commandsArgument[i], "arow") == 0)){
            tabRowAdd++;
        }
        if ((strcmp(commandsArgument[i], "icol") == 0) || (strcmp(commandsArgument[i], "acol") == 0)){
            tabColumnAdd++;
        }
    }   
    
    int tabRows = t.rows + 1; // + 1, C counts from zero, me from one
    int tabColumns = t.columns + 1; // same case
    int tabRowsDef = t.rows + tabRowAdd; // definitive number of rows
    int tabColumnsDef = t.columns + tabColumnAdd; // definitive number of columns
    int tabChars = t.maxNoChar;
    
    char ***table;
    table = NULL;
    table = memoryAllocation(table, tabRowsDef, tabColumnsDef, tabChars);
    table = fillingTable(table,  delim, file);
    a.table = table;
    a.file = file;
    a.tabCdef = tabColumnsDef;
    a.tabRdef = tabRowsDef;

    char def[10][MAXARG]; // table of temporary variables 
    int varNum; // variable number

    // this loops goes through CMD_SEQUENCE and looks for commands
    for (int k = 0; k < cmdNo; k++) {
        
        // Function IROW - adds a row 
        if (strcmp(commandsArgument[k], "irow") == 0) {
            tabRows++;
            irow(table, commandsArgument, tabRows, tabColumns, k, a);
            
        // Function AROW - adds a row below
        } else if (strcmp(commandsArgument[k], "arow") == 0) {
            tabRows++;
            arow(table, commandsArgument, tabRows, tabColumns, k, a);

        // Function DROW - deletes rows
        } else if (strcmp(commandsArgument[k], "drow") == 0) {   
            a = selectionRowsColumns(commandsArgument,tabRows, tabColumns, k, a); 
            int iRow, iRow1;
            iRow = a.numR1, iRow1 = a.numR2;
            tabRows = drow(table, tabRows, tabColumns, iRow, iRow1);

        // Function ICOL - adds column
        } else if (strcmp(commandsArgument[k], "icol") == 0) {
            tabColumns++;
            icol(table, commandsArgument, tabRows, tabColumns, k, a);

        // Function ACOL - adds column below
        } else if (strcmp(commandsArgument[k], "acol") == 0) {
            tabColumns++;
            acol(table, commandsArgument, tabRows, tabColumns, k, a);

        // Function DCOL - deletes columns
        } else if (strcmp(commandsArgument[k], "dcol") == 0) {
            a = selectionRowsColumns(commandsArgument,tabRows, tabColumns, k, a); 
            int iCol, iCol1;
            iCol = a.numC1, iCol1 = a.numC2;
            tabColumns = dcol(table, tabRows, tabColumns, iCol, iCol1);

        // Function [set] - saves the current cell selection
        } else if (strcmp(commandsArgument[k], "[set]") == 0) {
            a = selectionRowsColumns(commandsArgument,tabRows, tabColumns, k, a); 
       
        // Funciton SET - sets string to a cell
        } else if (strstr(commandsArgument[k],"set")) {
            set(table, commandsArgument, tabRows, tabColumns, tabChars, k, a);
            
        // Function SWAP - swaps cells
        } else if (strstr(commandsArgument[k], "swap")) {
            swap(table, commandsArgument, tabRows, tabColumns, k, a); 

        // Function CLEAR - clears cells
        } else if (strcmp(commandsArgument[k], "clear") == 0) {
            clear(table, commandsArgument, tabRows, tabColumns, k, a);

        // Function SUM - counts total sum of selected cells
        } else if (strstr(commandsArgument[k],"sum")) {
            sum(table, commandsArgument, tabRows, tabColumns, k, a);
        
        // Function AVG - counts average of selected cells
        } else if (strstr(commandsArgument[k],"avg")) {
            avg(table, commandsArgument, tabRows, tabColumns, k, a);

        // Function COUNT - counts number of non-empty cells
        } else if (strstr(commandsArgument[k],"count")) {
            count(table, commandsArgument, tabRows, tabColumns, k, a);

        // Function lEN - counts length of a cell
        } else if (strstr(commandsArgument[k],"len")) {
            len(table, commandsArgument, tabRows, tabColumns, k, a);
        
        // Function DEF - defines up to ten temporary variables
        } else if(strstr(commandsArgument[k], "def")){
            varNum = DefInc(commandsArgument, k);
            a = selectionRowsColumns(commandsArgument,tabRows, tabColumns, k, a); 
            strcpy(def[varNum],table[a.numR1][a.numC1]);

        // Function INC - iterates a temporary variable by one with each call
        } else if (strstr(commandsArgument[k], "inc")) {
            varNum = DefInc(commandsArgument, k);
            if(strcmp(def[varNum], "") == 0) {
                strcpy(def[varNum], "0");
            }
            int num = strtol(def[varNum], NULL, 10);
            num++;
            sprintf(def[varNum], "%d", num);

        // Function USE - calls a applies temporary variables
        }  else if (strstr(commandsArgument[k], "use")) {
            a = selectionRowsColumns(commandsArgument,tabRows, tabColumns, k, a);  
            varNum = DefInc(commandsArgument, k);
            for (int u = a.numR1; u <= a.numR2; u++){                          
                for (int v = a.numC1; v <= a.numC2; v++){
                    strcpy(table[u][v],def[varNum]);
                }   
            }
        
        // Funciton [max] - looks for a cell with the highest numerical value
        } else if (strcmp(commandsArgument[k], "[max]") == 0) {
            a = max(table, commandsArgument,tabRows, tabColumns, k, a); 
        
        // Function [min] - looks for a cell with lowest numerical value
        } else if (strcmp(commandsArgument[k], "[min]") == 0) {
            a = min(table, commandsArgument,tabRows, tabColumns, k, a); 
        
        // Function [find STR] - looks for a specific string
        }  else if (strstr(commandsArgument[k], "[find"))  {  
            a = findString(table, commandsArgument,tabRows, tabColumns, k, a); 
        }
    }

    int lastValue; // counts number of empty right columns
    for (lastValue = tabColumnsDef-1; 0 < lastValue; lastValue --){                     
         for (int u = 0; u < tabRows; u++){
            if(strcmp(table[u][lastValue ],"") != 0) {
                // jumps out of nested loops
                goto lastCol;
            } 
        }
    }

    lastCol: tabColumns = lastValue+1;
    // printing table
    fclose(file);
    file = freopen(argv[argc-1], "w", stdout);
    
    for ( int u = 0; u < tabRows; u++){                      
        for (int v = 0; v < tabColumns; v++){
            fprintf(file,"%s", table[u][v]);
            if(v < tabColumns - 1) {
                fprintf(file,"%c", delim[0]);
            } 
        }
        if (u < tabRows -1){
            fprintf(file, "\n");
        }
        
    }

    // free memory
    free(commandsArgument);
    fclose(file);
    freeMemory(table, tabRowsDef, tabColumnsDef);
    
    return 0;
}
