/*
*
* @ project no. 1 - Working with text
* @ deadline: 11/15/2020
* @ name: Tadeáš Kachyňa
* @ login: xkachy00
*
*  About: This project does simple table operations.
*
*  Translation - "$ gcc -std=c99 -Wall -Wextra -Werror sheet.c -o sheet"
*
*  Exceution syntax - 
*  "./sheet [-d DELIM] [Table editing functions]" 
*  or
*  "./sheet [-d DELIM] [Rows selection] [Data processing functions]"
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH_SIZE 10242                       // = 10KiB (10240) + \n (next line) + \0 (null)

// ------------- TABLE EDITING FUNCTIONS - DEFINITIONS ------------- //

// Function IROW R - inserts a row R > 0
void irow(char *buffer, char *delim) {

    // the cycle iterates over the row until it finds the character \ n, so it replaces it with a zero, and then prints a new line
    // numOfDel = number of delimiters in a row
    for (int numOfDel= 0; buffer[numOfDel] != '\0'; ++numOfDel){
        if(delim[0] == buffer[numOfDel]) {
            printf("%c", delim[0]); 
        }
    }
    printf("\n");  // prints new line
}

// Function AROW - appends a row to the end of the table 
void arow(char *buffer, char *delim) {

    printf("\n");
    // the cycle iterates over the row until it finds the character \ n, so it replaces it with a zero, prints a new line 
    // numOfDel = number of delimiters in a row
    for (int numOfDel= 0; buffer[numOfDel] != '\0'; ++numOfDel){
        if(delim[0] == buffer[numOfDel]) {
            printf("%c", delim[0]);    
        }
    }
}

// Function ICOL C - inserts an empty column before the column given by the number C
void icol(char *buffer, int numOfCol, char *delim) {

    char bufferTemp1[MAX_LENGTH_SIZE] = "", bufferTemp2[MAX_LENGTH_SIZE] = "", bufferTemp3[MAX_LENGTH_SIZE] = "";                  
    unsigned int numOfChar = 0, numOfChar2 = 0, numOfChar3 = 0;  // number of characters in a row
    int numOfDelim = 0;  // number of delimiters in a row   
    
    char *p = strchr(buffer, '\n'); // looks for the first occureance of \n, then it replaces with null
    if (p) {
        *p = '\0';
    }

    strncat(buffer,delim, 1); // adds delimiter to the end of the buffer, in case of adding column at the end and avoid segmentation fault
    
    // Beginning of the string - for loop counts chars from the left side
    for (numOfChar = 0; numOfDelim < numOfCol - 1; numOfChar++){       
        if (buffer[numOfChar] == delim[0]) { // counts delimiters from the left side
            numOfDelim++; 
        }          
    }
    
    strncpy(bufferTemp1,buffer,numOfChar);              // copies first part of buffer
    while (buffer[numOfChar] != delim[0]) {             // Middle of the string                    
        bufferTemp2[numOfChar2] = buffer[numOfChar];
        numOfChar++;
        numOfChar2++;               
    }

    while (numOfChar != strlen(buffer)){                //  End of the string          
        bufferTemp3[numOfChar3] = buffer[numOfChar];
        numOfChar++; 
        numOfChar3++;         
    }
    
    // Final connection of strings
    strcpy(buffer, bufferTemp1), strcat(buffer,delim), strcat(buffer,bufferTemp2), strcat(buffer,bufferTemp3);  

    char *p2 = strrchr(buffer, delim[0]); // looks for the first occurance of delim from the end and replace it with null sign
    if (p2) {
        *p2 = '\0';
    }
    strcat(buffer,"\n"); // prints new line
}

// Function ACOL - appends a new column at the end of the table
void acol(char *buffer, char *delim) {

    char *p = strchr(buffer, '\n'); // looks for the first occureance of \n, then it replaces it with null
    if (p) {
        *p = '\0';
    }    
    strncat(buffer, delim, 1), 
    strcat(buffer, "\n"); // adds delim[0] and \n to buffer
}

// Function DCOL C - deletes column number C
void dcol(char *buffer, int numOfCol, char *delim) {
    
    char bufferTemp1[MAX_LENGTH_SIZE] = "", bufferTemp2[MAX_LENGTH_SIZE] = "", bufferTemp3[MAX_LENGTH_SIZE] = "";
    unsigned int numOfChar = 0, numOfChar2 = 0, numOfChar3 = 0;// number of characters
    int numOfDelim = 0; // number of delimiters         

    char *p = strchr(buffer, '\n'); // looks for the first occureance of \n, then it replaces with null
    if (p) {
        *p = '\0';
    }
    strncat(buffer,delim,1); // adds delimiter to the end of the buffer, in case of adding column at the end and avoid segmentation fault

    for (numOfChar = 0; numOfDelim < numOfCol - 1; numOfChar++){        
        if(buffer[numOfChar]== delim[0])     
            numOfDelim++;
        }
    
    strncpy(bufferTemp1,buffer,numOfChar);               // copies first part of buffer

    while (numOfChar2 < 1) {                             // Middle of the string        
        if (buffer[numOfChar] == delim[0]) {
            numOfChar2++;
        }
        numOfChar++;
    }
                          
    while (numOfChar!= strlen(buffer)){                    //  End of the string    
        bufferTemp2[numOfChar3]=buffer[numOfChar];
        numOfChar++; 
        numOfChar3++;        
    }
    
    // Final connection of strings
    strcpy(bufferTemp3,bufferTemp1), strcat(bufferTemp3,bufferTemp2),strcpy(buffer,bufferTemp3);

    char *p2 = strrchr(buffer, delim[0]); // looks for the first occureance of \n, then it replaces it with null
    if (p2) {
        *p2 = '\0';
    }
    strcat(buffer,"\n");
}

// Funkce DCOLS N M  = removes columns N to M (N <= M). In the case of N = M, the command behaves the same as dcol N.
void dcols(char *buffer, int numOfCol, int numOfCol2, char *delim) {
    char bufferTemp1[MAX_LENGTH_SIZE] = "", bufferTemp2[MAX_LENGTH_SIZE] = "", bufferTemp3[MAX_LENGTH_SIZE] = "";
    unsigned int numOfChar = 0, numOfChar2 = 0, numOfChar3 = 0; // number of characters
    int numOfDelim = 0;  // number of delimiters         

    char *p = strchr(buffer, '\n'); // looks for the first occureance of \n, then it replaces with null
    if (p) {
        *p = '\0';
    }
    strncat(buffer,delim,1); // adds delimiter to the end of the buffer, in case of adding column at the end and avoid segmentation fault
               
    for (numOfChar = 0; numOfDelim < numOfCol - 1; numOfChar++){        
        if(buffer[numOfChar]== delim[0])     
            numOfDelim++;
        }
    
    strncpy(bufferTemp1,buffer,numOfChar);              // copies first part of buffer
    int b = 0;
    while ( b < numOfCol2 - numOfCol + 1) {              // Middle of the string                          
        if (buffer[numOfChar] == delim[0]) {
            b++;
        }
        numOfChar++;
        numOfChar2++;  
    }
                         
    while (numOfChar!= strlen(buffer)){                 //  End of the string  
        bufferTemp2[numOfChar3]=buffer[numOfChar];
        numOfChar++; 
        numOfChar3++;        
    }
    
    // Final connection of strings
    strcpy(bufferTemp3,bufferTemp1), strcat(bufferTemp3,bufferTemp2), strcpy(buffer,bufferTemp3);

    char *p2 = strrchr(buffer, delim[0]); // looks for the first occureance of \n, then it replaces it with null
    if (p2) {
        *p2 = '\0';
    }
    strcat(buffer,"\n");
}

// ------------- DATA PROCESSING FUNCTIONS - DEFINITIONS ------------- // 

// Function CSET - the string STR will be set to the cell in column C
void cset(char *buffer, int numOfCol, char *argv, char *delim) {

    char bufferTemp1[MAX_LENGTH_SIZE] = "", bufferTemp2[MAX_LENGTH_SIZE] = "";
    unsigned int numOfChar = 0, numOfChar2 = 0, numOfChar3 = 0; // number of characters
    int numOfDelim = 0;  // number of delimiters           
   
    char *p = strchr(buffer, '\n'); // looks for the first occureance of \n, then it replaces with null
    if (p) {
        *p = '\0';
    }

    strncat(buffer,delim, 1); // adds delimiter to the end of the buffer, in case of editing column at the end and avoid segmentation fault
    
    for (numOfChar = 0; numOfDelim < numOfCol - 1; numOfChar++){        
        if(buffer[numOfChar]== delim[0])     
            numOfDelim++;
    }
                
    strncpy(bufferTemp1,buffer,numOfChar);               // copies first part of buffer
    
     while (numOfChar2 < 1) {                             // Middle of the string   
        if (buffer[numOfChar] == delim[0]) {                
            numOfChar2++;
        }  
        numOfChar++; 
    }
                                 
    while (numOfChar!= strlen(buffer)){                 //  End of the string      
        bufferTemp2[numOfChar3]=buffer[numOfChar];
        numOfChar++; 
        numOfChar3++;        
    }

    // Final connection of strings
    strcpy(buffer,bufferTemp1), strcat(buffer,argv), strncat(buffer,delim, 1),strcat(buffer,bufferTemp2);

    char *p2 = strrchr(buffer, delim[0]); // looks for the first occureance of \n, then it replaces it with null
    if (p2) {
        *p2 = '\0';
    }
    strcat(buffer,"\n");  
}

// Function TOLOWER C- the string in column C will be converted to lowercase
void f_tolower(char *buffer, int numOfCol, char *delim) {

    char bufferTemp1[MAX_LENGTH_SIZE] = "", bufferTemp2[MAX_LENGTH_SIZE] = "", bufferTemp3[MAX_LENGTH_SIZE] = "";
    unsigned int numOfChar = 0, numOfChar2 = 0, numOfChar3 = 0; // number of characters
    int numOfDelim = 0;  // number of delimiters  

    char *p = strchr(buffer, '\n'); // looks for the first occureance of \n, then it replaces with null
    if (p) {
        *p = '\0';
    }
    strncat(buffer,delim, 1); // adds delimiter to the end of the buffer, in case of editing column at the end and avoid segmentation fault

    // Beginning of the string - for cycle counts chars from the left side
    for (numOfChar = 0; numOfDelim < numOfCol - 1; numOfChar++){         
        if(buffer[numOfChar]==delim[0])  // counts number of delimiters from the left side
            numOfDelim++;
    }
    
    strncpy(bufferTemp1,buffer,numOfChar);   // copies first part of buffer
          
    while (buffer[numOfChar]!=delim[0]) {   // Middle of the string                       
    
        bufferTemp2[numOfChar2] = tolower(buffer[numOfChar]); // translates all letters to lowercase letters
        numOfChar++;
        numOfChar2++;   
            
    }
                       
    while (numOfChar != strlen(buffer)){     // End of the string              

        bufferTemp3[numOfChar3]=buffer[numOfChar];
        
        numOfChar++; 
        numOfChar3++;  
            
    }
    // Final connection of strings
    strcpy(buffer,bufferTemp1), strcat(buffer,bufferTemp2),strcat(buffer,bufferTemp3);

    char *p2 = strrchr(buffer, delim[0]); // looks for the first occureance of \n, then it replaces it with null
    if (p2) {
        *p2 = '\0';
    }
    strcat(buffer,"\n");  
}

// Function TOUPPER C - the string in column C will be converted to uppercase.
void f_toupper(char *buffer, int numOfCol, char *delim) {

    char bufferTemp1[MAX_LENGTH_SIZE] = "", bufferTemp2[MAX_LENGTH_SIZE] = "", bufferTemp3[MAX_LENGTH_SIZE] = "";
    unsigned int numOfChar = 0, numOfChar2 = 0, num0OfChar3 = 0; // number of characters
    int numOfDelim = 0;  // number of delimiters 

    char *p = strchr(buffer, '\n'); // looks for the first occureance of \n, then it replaces with null
    if (p) {
        *p = '\0';
    }

    strncat(buffer,delim, 1); // adds delimiter to the end of the buffer, in case of editing column at the end and avoid segmentation fault

    // Beginning of the string - for cycle counts chars from the left side
    for (numOfChar = 0; numOfDelim < numOfCol - 1; numOfChar++){         
        if(buffer[numOfChar] == delim[0])  // counts number of delimiters from the left side
            numOfDelim++;
    }
    
    strncpy(bufferTemp1,buffer,numOfChar);   // copies first part of buffer
           
    while (buffer[numOfChar] != delim[0]) {   // Middle of the string                       
        bufferTemp2[numOfChar2] = toupper(buffer[numOfChar]); // translates all letters to uppercase letters
        numOfChar++;
        numOfChar2++;        
    }
                         
    while (numOfChar != strlen(buffer)){     // End of the string              
        bufferTemp3[num0OfChar3] = buffer[numOfChar];
        numOfChar++; 
        num0OfChar3++;  
            
    }
    // Final connection of strings
    strcpy(buffer,bufferTemp1), strcat(buffer,bufferTemp2), strcat(buffer,bufferTemp3);

    char *p2 = strrchr(buffer, delim[0]); // looks for the first occureance of \n, then it replaces it with null
    if (p2) {
        *p2 = '\0';
    }
    strcat(buffer,"\n");  
}

// Function ROUND C - in column C rounds the number to an integer
void f_round(char *buffer, int numOfCol, char *delim) {

    char bufferTemp1[MAX_LENGTH_SIZE] = "", bufferTemp2[MAX_LENGTH_SIZE] = "", bufferTemp3[MAX_LENGTH_SIZE] = "";
    unsigned int numOfChar = 0, numOfChar2 = 0, numOfChar3 = 0; // number of characters
    int numOfDelim = 0;  // number of delimiters  

    char *p = strchr(buffer, '\n'); // looks for the first occureance of \n, then it replaces with null
    if (p) {
        *p = '\0';
    }

    strncat(buffer,delim, 1); // adds delimiter to the end of the buffer, in case of editing column at the end and avoid segmentation fault

    // Beginning of the string - for cycle counts chars from the left side
    for (numOfChar = 0; numOfDelim < numOfCol - 1; numOfChar++){         
        if(buffer[numOfChar]==delim[0])  // counts number of delimiters from the left side
            numOfDelim++;
    }
    
    strncpy(bufferTemp1,buffer,numOfChar);              // copies first part of buffer
          
    while (buffer[numOfChar]!=delim[0]) {               // Middle of the string                       
        bufferTemp2[numOfChar2] = buffer[numOfChar]; 
        numOfChar++;
        numOfChar2++;           
    } 
    
    float value;
    char *ptr;

    value = strtod(bufferTemp2, &ptr); // transfering string to double
    if ((*ptr == '\0') || (isspace(*ptr) != 0)){
        int a = (int)(value < 0 ? (value - 0.5) : (value + 0.5)); // if condition is true it does the first part, otherwise the second one
        sprintf(bufferTemp2, "%d", a); // transfering int to string
    } 
          
    while (numOfChar != strlen(buffer)){                   // End of the string              
            bufferTemp3[numOfChar3]=buffer[numOfChar];
            numOfChar++; 
            numOfChar3++;            
    }

    // Final connection of strings
    strcpy(buffer,bufferTemp1), strcat(buffer,bufferTemp2), strcat(buffer,bufferTemp3); 

    char *p2 = strrchr(buffer, delim[0]);  // looks for the first occureance of \n, then it replaces it with null
    if (p2) {
        *p2 = '\0';
    }
    strcat(buffer,"\n");
}

// Function INT C - removes the decimal part of the number in column C
void f_int(char *buffer, int numOfCol, char *delim) {
    char bufferTemp1[MAX_LENGTH_SIZE] = "", bufferTemp2[MAX_LENGTH_SIZE] = "", bufferTemp3[MAX_LENGTH_SIZE] = "";
    unsigned int numOfChar = 0, numOfchar2 = 0, numOfChar3 = 0; // number of characters
    int numOfDelim = 0;  // number of delimiters  

    char *p = strchr(buffer, '\n'); // looks for the first occureance of \n, then it replaces with null
    if (p) {
        *p = '\0';
    }
    strncat(buffer,delim, 1); // adds delimiter to the end of the buffer, in case of editing column at the end and avoid segmentation fault

    // Beginning of the string - for cycle counts chars from the left side
    for (numOfChar = 0; numOfDelim < numOfCol - 1; numOfChar++){         
        if(buffer[numOfChar]==delim[0])  // counts number of delimiters from the left side
            numOfDelim++;
    }
    
    strncpy(bufferTemp1,buffer,numOfChar);   // copies first part of buffer
          
    while (buffer[numOfChar]!=delim[0]) {   // Middle of the string                       
        bufferTemp2[numOfchar2] = buffer[numOfChar]; // translates all letters to lowercase letters
        numOfChar++;
        numOfchar2++;         
    } 
        
    double value; 
    char *ptr;
    value = strtod(bufferTemp2, &ptr); //transfering string to double
    if ((*ptr == '\0') || (isspace(*ptr) != 0)){
        int a = (value * 100) / 100;    // double to int conversion
        sprintf(bufferTemp2, "%d", a); // transfering string to int
    } 
     
    while (numOfChar != strlen(buffer)){     // End of the string              
        bufferTemp3[numOfChar3]=buffer[numOfChar];
        numOfChar++; 
        numOfChar3++;  
                
        }

    // Final connection of strings
    strcpy(buffer,bufferTemp1), strcat(buffer,bufferTemp2), strcat(buffer,bufferTemp3); 

    char *p2 = strrchr(buffer, delim[0]); // looks for the first occureance of \n, then it replaces it with null
    if (p2) {
        *p2 = '\0';
    }
    strcat(buffer,"\n"); 
}

// Function MOVE - its usage is going to be in the next three functions
void move(char *buffer, char *buffertemp, int numOfCol, char *delim) {
    char buffer1[1000] = "", buffer2[1000] = "";                  
    unsigned int numOfChar;
    int numOfDelim =0;

    char *p = strchr(buffer, '\n'); // looks for the first occureance of \n, then it replaces with null
    if (p) {
        *p = '\0';
    }
  
    strncat(buffer,delim, 1);   
    
    for (numOfChar = 0; numOfDelim < numOfCol - 1; numOfChar++)
    {       
        if(buffer[numOfChar]==delim[0])     
            numOfDelim++;
    }
    
    strncpy(buffer1,buffer,numOfChar);  
    int c=0;
   
    while (buffer[numOfChar]!=delim[0]) {                            
    
        buffer2[c]=buffer[numOfChar];
        numOfChar++;
        c++;   
            
        }

     char *p2 = strrchr(buffer, delim[0]); // looks for the first occureance of \n, then it replaces it with null
    if (p2) {
        *p2 = '\0';
    }
    strcat(buffer,"\n");  
    
    strncpy(buffertemp, buffer2, numOfChar);

}

// Function TABLEDITING - Editing the table causes the table to be enlarged or reduced, resp. rows and columns
void tableEditing(char* buffer, char *delim,int k, int cycle, char *argv[]) {

    // Function IROW R - inserts a row R > 0 -------------------- //
   
    if (strcmp(argv[k], "irow") == 0) {  
        if (atoi(argv[k+1]) == cycle) {
            for (int numOfDel= 0; buffer[numOfDel] != '\0'; ++numOfDel){
                if(delim[0] == buffer[numOfDel]) {
                    printf("%c", delim[0]);
                }
            }
            printf("\n");
        }
        
    // Function ICOL - inserts a new column before the R > 0-------------------- //

    } else if (strcmp(argv[k], "icol") == 0) {
        int numOfCol = atoi(argv[k+1]);
        icol(buffer,numOfCol,delim);

    // Function ACOL - appends a new column at the end of the table -------------------- //

    } else if (strcmp(argv[k], "acol") == 0) {
        acol(buffer, delim);

    // Function DCOL - deletes a column number R > 0 -------------------- //

    } else if (strcmp(argv[k], "dcol") == 0) {
        int numOfCol = atoi(argv[k+1]);
        dcol(buffer,numOfCol,delim);

    // Function DCOLS - removes columns N to M (N <= M). In the case of N = M, the command behaves the same as dcol N -------------------- //

    } else if (strcmp(argv[k], "dcols") == 0) {
        int numOfCol = atoi(argv[k+1]);
        int numOfCol2 = atoi(argv[k+2]);
        dcols(buffer,numOfCol,numOfCol2, delim);
    } 
}

// Function DATAPROCESSING - Data processing is the modification of the contents of individual table cells
int dataProcessing(char* buffer, char *delim,int k, char **argv, int length) {

    // int length = counts number of arguments for every function, will be useful ater

    // Function CSET - the string STR will be set in the cell in column C -------------------- //
    
    if (strcmp(argv[k], "cset") == 0){
        int numOfCol = atoi(argv[k+1]);
        length = 3; // means [k] + [k+1] + [k+2], same in other functions
        cset(buffer, numOfCol, argv[k+2], delim);

    // Function TOLOWER C -  the string in column C will be converted to lowercase -------------------- //

    } else if (strcmp(argv[k], "tolower") == 0){
        int numOfCol = atoi(argv[k+1]);
        length = 2;
        f_tolower(buffer,numOfCol,delim);

    // Function TOUPPER C - the string in column C will be converted to uppercase -------------------- //

    } else if (strcmp(argv[k], "toupper") == 0){
        int numOfCol = atoi(argv[k+1]);
        length = 2;
        f_toupper(buffer,numOfCol,delim);

    // Function ROUND C - in column C rounds the number to an integer -------------------- //
    
    } else if (strcmp(argv[k], "round") == 0){
        int numOfCol = atoi(argv[k+1]);
        length = 2;
        f_round(buffer,numOfCol,delim);

    // Function INT C - removes the decimal part of the number in column C -------------------- //

    } else if (strcmp(argv[k], "int") == 0) {
        int numOfCol = atoi(argv[k+1]);
        length = 2;
        f_int(buffer,numOfCol,delim);

    // In the next three functions we are going to use the function MOVE we have written before.

    // Function COPY N M - overwrites the contents of cells in column M with the values from column N -------------------- //

    } else if (strcmp(argv[k], "copy") == 0) {
        char buffertemp[MAX_LENGTH_SIZE];
        int numOfCol = atoi(argv[k+1]);
        int numOfCol2 = atoi(argv[k+2]);
        length = 3;
        move(buffer, buffertemp, numOfCol, delim);
        cset(buffer, numOfCol2, "", delim); 
        cset(buffer, numOfCol2,  buffertemp, delim);

    // Function SWAP N M -  swaps the values of cells in columns N and M -------------------- //

    } else if (strcmp(argv[k], "swap") == 0) {
        char buffertemp[MAX_LENGTH_SIZE];
        char buffertemp2[MAX_LENGTH_SIZE];
        int numOfCol = atoi(argv[k+1]);
        int numOfCol2 = atoi(argv[k+2]);
        length = 3;
        move(buffer, buffertemp, numOfCol, delim);
        move(buffer, buffertemp2, numOfCol2, delim);
        
        cset(buffer, numOfCol2,buffertemp, delim);
        cset(buffer, numOfCol, buffertemp2, delim);
        
    // Function MOVE N M - moves column N before column M -------------------- //

    } else if (strcmp(argv[k], "move") == 0) {
        int numOfCol = atoi(argv[k+1]);
        int numOfCol2 = atoi(argv[k+2]);
        char buffertemp[1000];
        length = 3;
        move(buffer, buffertemp, numOfCol, delim);
        icol(buffer,numOfCol2,delim);
        cset(buffer, numOfCol2, buffertemp, delim);
        dcol(buffer,numOfCol,delim);  
    } 

    return length;
}

// Function ROWSSELECTIONS
void rowsSelection (char* buffer, char *delim,int ii, int cycle, char **argv, int arg_length, int argc) {
    
    for (ii = 0; ii < argc; ii++) {
    
        if (strcmp(argv[ii], "beginswith") == 0 ){

            // function "move" withdraws cells of a column given by the argument
            int numOfCol = atoi(argv[ii+1]);
            char buffer_temp2[1000];
            move(buffer, buffer_temp2, numOfCol, delim);
            int len = strlen(argv[ii+2]);

            // two loops compare every char of argv[ii+2] and buffer_temp_2
            for (int e = 0; e < len; e++){
                for (int e2 = 0; e2 < len; e2++) {
                    if (buffer_temp2[e] == argv[ii+2][e2]) {
                        // it they coincide, dataProcessing will launch
                        dataProcessing(buffer, delim, ii + 3, argv, arg_length);
                    }
                }
            }
            ii =  ii + 3 + arg_length; // three arguments for "beginswith" function + arg_lenth arguments for each function

        } else if (strcmp(argv[ii], "rows") == 0) {
            char* ptr;
            char* ptr2;
            int j = strtol(argv[ii + 1], &ptr, 10); // string to int
            int j2 = strtol(argv[ii + 2], &ptr2, 10); // string to int
            if((strcmp(ptr,"-") == 0) && (strcmp(ptr2, "-") == 0)) { // if both strings are "-", this function applies only on last row
                char end_line[1] = "\n";
                int end = strlen(buffer) - 1;
                if (buffer[end] != end_line[0]) {
                    j = cycle;
                    j2 = cycle;
                   }                
            }
                
            if((strcmp(ptr,"-") != 0) && (strcmp(ptr2, "-") == 0)) { // if only second string is "-", this function  applies from row x till last row
                j2 = j;                   
            if (j < cycle) {
                    j2 = cycle;
                }  
            }

            if (j <= cycle && cycle <= j2) { // if neither of two statements above are true, function applies to rows from number x till number x
                dataProcessing(buffer, delim, ii + 3, argv, arg_length);  
            }
            ii = ii + 3 + arg_length; // three arguments for "rows" function + arg_lenth arguments for each function

        }  else if (strcmp(argv[ii], "contains") == 0) {

            // function "move" withdraws cells of a column given by the argument
            int NumOfCol = atoi(argv[ii+1]);
            char buffer_temp2[1000];
            move(buffer, buffer_temp2, NumOfCol, delim);

            // function "strstr" looks for a occurance
            if (strstr(buffer_temp2,argv[ii + 2])) {
                if (strstr(buffer, buffer_temp2)) {
                    dataProcessing(buffer, delim, ii + 3, argv, arg_length);
                }
            }
            ii = ii + 3  + arg_length; // three arguments for "contains" function + "arg_lenth" arguments for each function

        }  else {

            // this part runs without Rows Selection part
            dataProcessing(buffer, delim, ii, argv, arg_length);
            ii=  ii  + arg_length; // "arg_lenth" arguments for each function
        }

        
    }
}

// Function ARGCHECKING - checks entered arguments
int argChecking(int argc, char *argv[], int maxNum) {

    // this loop checks every entered argument, int maxNum represents number of delimiters
    // checking argument i + 1 is only needed in functions which are working with columns due to possible segmentation error
    for (int i = 1; i < argc; ) {
        
        if (strcmp(argv[i], "-d") == 0) {
            i = i + 2; 
        }else if(strcmp(argv[i], "irow") == 0) {
            i = i + 2; 
        } else if(strcmp(argv[i], "arow") == 0){
            i = i + 1;
        } else if(strcmp(argv[i], "drow") == 0){
            i = i + 2;
        } else if(strcmp(argv[i], "drows") == 0){
            i = i + 3;
        } else if(strcmp(argv[i], "icol") == 0){
            int num = strtol(argv[i+1], NULL, 10);
            if (num > maxNum + 1) {
                return 2;
            } 
            i = i + 2;
        } else if(strcmp(argv[i], "acol") == 0){
            i = i + 1;
        } else if(strcmp(argv[i], "dcol") == 0){
            int num = strtol(argv[i+1], NULL, 10);
             if (num > maxNum + 1) {
                return 2;
            }  
            i = i + 2;
        } else if(strcmp(argv[i], "dcols") == 0){
            int num2 = strtol(argv[i+2], NULL, 10);
            if (num2 > maxNum + 1){
                return 2;
            }
            i = i + 3;
        } else if(strcmp(argv[i], "cset") == 0){
            int num = strtol(argv[i+1], NULL, 10);
            if (num > maxNum + 1) {
                return 2;
            }
            i = i + 3;
        } else if(strcmp(argv[i], "tolower") == 0){
            int num = strtol(argv[i+1], NULL, 10);
            if (num > maxNum + 1) {
                return 2;
            }
            i = i + 2;
        } else if(strcmp(argv[i], "toupper") == 0){
            int num = strtol(argv[i+1], NULL, 10);
            if (num > maxNum + 1) {
                return 2;
            }
            i = i + 2;
        } else if(strcmp(argv[i], "int") == 0){
            int num = strtol(argv[i+1], NULL, 10);
             if (num > maxNum + 1) {
                return 2;
            } 
            i = i + 2;
        } else if(strcmp(argv[i], "round") == 0){
            int num = strtol(argv[i+1], NULL, 10);
            if (num  > maxNum + 1) {
                return 2;
            } 
            i = i + 2;
        } else if(strcmp(argv[i], "swap") == 0){
            int num = strtol(argv[i+1], NULL, 10);
            int num2 = strtol(argv[i+2], NULL, 10);
            if (num > num2 || num2> maxNum + 1) {
                return 2;
            }
            i = i + 3;
        } else if(strcmp(argv[i], "move") == 0){
            int num = strtol(argv[i+1], NULL, 10);
            int num2 = strtol(argv[i+2], NULL, 10);
             if (num > num2 || num2 > maxNum + 1) {
                return 2;
            }
            i = i + 3;
        } else if(strcmp(argv[i], "copy") == 0){
            int num = strtol(argv[i+1], NULL, 10);
            int num2 = strtol(argv[i+2], NULL, 10);
             if (num > num2 || num2 > maxNum + 1) {
                return 2;
            }
            i = i + 3;
        } else if(strcmp(argv[i], "beginswith") == 0){
            int num = strtol(argv[i+1], NULL, 10);
            if (num > maxNum + 1) {
                return 2;
            }
            i = i + 3;
        } else if(strcmp(argv[i], "contains") == 0){
            int num = strtol(argv[i+1], NULL, 10);
            if (num > maxNum + 1) {
                return 2;
            }
            i = i + 3;
        } else if(strcmp(argv[i], "rows") == 0){
            i = i + 3;
        } else {
            fprintf(stderr,"ERROR > Wrong argument - %s\n", argv[i]);
            return 1;  
        } 

    }

    // Arrays of all functions
    char *tableEditingFunctions [] = {"irow", "arow", "drow", "drows", "icol", "acol", "dcol", "dcols"}; // shortcut "TE"
    char *tableprocessingFunctions [] = {"cset", "toupper", "tolower", "int", "round", "copy", "swap", "move"}; // shortcut "TP"
    char *rowsSelection [] = { "rows", "beginswith", "contains"}; // shortcut "RS"

    int counterA = 0, counterB = 0, counterC = 0, counterD  = 0; // these counters count number of found arguments
    
    // Cycles which are looping through argument line and the array, if it finds the "funciton", it increments value of aaa +1;

    // LOOP ONE and TWO - walks trough TE and TP functions. 
    for (int a = 0; a < argc; a++) {
        for (int aa = 0; aa < 8; aa++){
            if(strcmp(argv[a],tableEditingFunctions[aa]) == 0) {
                counterA++;  
            }
        }
    }

    for (int b = 0; b < argc; b++) {
        for (int bb = 0; bb < 8; bb++){
            if(strcmp(argv[b],tableprocessingFunctions[bb]) == 0) {
                counterB++;
            }
        }
    }

    // if it finds at least one function from these two groups, the program is going to end
    if(counterA > 0 && counterB > 0) {
        return 3;
    } 
   
    // LOOP THREE - walks tough TP functions, if there are more than one of them, the program is going to end
    for (int c = 0; c < argc; c++) {
        for (int cc = 0; cc < 8; cc++){
            if(strcmp(argv[c],tableprocessingFunctions[cc]) == 0) {
                counterC++;
            }
        }
    }

    if (counterC > 1) {
        return 3;
    }

    // LOOP FOUR - walks trough RS functions, if there are mmore than one of them, the program is going to end
    for (int d = 0; d < argc; d++) {
        for (int dd = 0; dd < 3; dd++){
            if(strcmp(argv[d],rowsSelection[dd]) == 0) {
                counterD++;
            }
        }
    }

    if (counterD > 1) {
        return 3;
    }
    return 0;
}


// Function DELIMITER - separates the string obtained from fgets
void delimiter(char* delimArray, char* delimLookingFor, char delimReplace) {  

    int d, n;
    int delimLength = strlen(delimArray); // buffer
    int numberOfDelims = strlen(delimLookingFor); // delimiters
    for (d = 0; d < delimLength; d++) {                     // for cycles are looking for delims in whole row
        for (n = 0; n < numberOfDelims; n++) {
            if (delimArray[d] == delimLookingFor[n]) {      // if it finds delimiters we are looking for, it will replace it with the first one
                delimArray[d] = delimReplace;
            }
        } 
    }
}

// Function MAIN 
int main(int argc, char *argv[]) { 

    // checks if at least 2 arguments are entered
    if (argc < 2) {                                   
        fprintf(stderr, "ERROR > Input error. No arguments entered.\n");
        return 0;
    }

    // if argv[1] is equal to "-d", then argv[2] is set to delimeter
    char *delim = " ";
    if (argc > 2) {
        if (strcmp(argv[1], "-d") == 0) { 
            delim = argv[2];     
        } else {
            delim = " ";
        }
    }
   
    char buffer[MAX_LENGTH_SIZE]; // buffer that loads rows from a table using fgets
    int cycle = 1; // cycle counter
    int del = 0; // this variable counts number od delimiters in the first row 
    int i = 0; // a variable for a sequence of table editing functions
    int ii = 0; // a variable for rows selection
    int emptyFile = 0; // detecting empty file
 
    while(fgets(buffer, MAX_LENGTH_SIZE, stdin) != NULL) {

        delimiter(buffer, delim, delim[0]);

        // CHECKING OF DELIMITERS
        int del2 = 0; // this variable checks number of delimiters in every row
        // for loop counts delimiters in first row
        if(cycle == 1) {
            for (int numOfDel= 0; buffer[numOfDel] != '\0'; ++numOfDel){
                if(delim[0] == buffer[numOfDel]) {
                    del++;
                }
            }
        }
        // for loop counts delimiters in every row except the first one
        for (int numOfDel= 0; buffer[numOfDel] != '\0'; ++numOfDel){
            if(delim[0] == buffer[numOfDel]) {
                del2++;
            }
        }   
        // then it compares these two numbers
        if(!(del == del2)) {
            fprintf(stderr, "ERROR > Number of columns is not same in every row.\n");
            return 1;      
        } 

        int arg_length = 0;
        // this variable handles "Rows selection + Data processing" functions and "Data processing" functions to avoid error due to low number of arguments
        int print = 1; // this variable handles printing buffer, if it is "0" print will not happen

        // checks all arguments
        argChecking(argc,argv, del);

        // different errors, it depends on a nubmer which function returns
        if (argChecking(argc,argv, del) ==  1) {
            fprintf(stderr, "ERROR > Input error.\n");
            return 1;
        } else if (argChecking(argc,argv, del) == 2) {
            fprintf(stderr, "ERROR > There aren't so many columns. Maximum number is %d.\n", del + 1);
            return 1;
        } else if (argChecking(argc,argv, del) == 3){
            fprintf(stderr, "ERROR > Function collision.\n");
            return 1;
        }
       
        // -------------------- TABLE EDITING -------------------- //

        for (i = 0; i < argc; i++) { 
            tableEditing(buffer, delim, i, cycle, argv);

            // Function DROW - deletes a row number R > 0 -------------------- //

            if (strcmp(argv[i], "drow") == 0) {
                for (int j = 0; argv[i+1][j] != '\0'; j++) {
                    if (isdigit(argv[i+1][j])) {
                        if (atoi(argv[i+1]) == cycle) {
                            print = 0;  // means, the row wont't be printed
                        }   
                    } else {
                        fprintf(stderr, "ERROR > |%s| is not a number.\n", argv[i+1]);
                        return 0;
                    }
                }
            
            // Function DROWS N M - removes lines N to M (N <= M). In the case of N = M, the command behaves the same as drow N -------------------- //

            } else if (strcmp(argv[i], "drows") == 0) {
                if (atoi(argv[i+1]) <= cycle && cycle <= atoi(argv[i+2])) {
                    print = 0; // means, these rows wont't be printed
                }
            } 
        }

        // -------------------- ROWS SELECTION & DATA PROCESSING-------------------- // 
        
        rowsSelection(buffer,delim,ii,cycle,argv, arg_length, argc);
        
        // -------------------- BUFFER PRINT -------------------- //
        
        if (print == 1) { 
            printf("%s", buffer);
        } 

        cycle++; // cycles counter  
        emptyFile++; 
    }       
        
    if (emptyFile == 0) {
        fprintf(stderr, "ERROR > Empty file.\n");
        return 1;
    }
    // -------------------- TABLE EDITING - F. AROW -------------------- // 

    for (i = 0; i < argc; i++)   {

       // Function AROW - adds a new row row to the end of the table -------------------- //
        if (strcmp(argv[i], "arow") == 0) {
            arow(buffer, delim);
        }
    }  

    return 0;
}
