#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "utils.h"
#include "macro.h"
#include "globals.h"

boolean preprocessorFileCheck(char *inputFile){
    
  
    char *outputFileName = appendFileExtension(inputFile, ".am");/*new file*/
    char *inputFileName = appendFileExtension(inputFile, ".as");/*original file*/
    FILE *inputFP = fopen(inputFileName,"r");
    FILE *outputFP = fopen(outputFileName,"w");
    char lineBuffer [MAX_LINE_LENGTH + 2]; 
    int lineIdx;
    char *macroName=NULL;
    boolean readingMacro = false;
    boolean success = true;
    macrosList *head = NULL;
    macrosList *macro = NULL;

  /*Checking if the fopen of the .as file was successful. If not, returning false*/
if (inputFP == NULL){
    printf("problem in opening %s", inputFileName);
    free(outputFileName);
    free(inputFileName);
    if (outputFP != NULL) {
        fclose(outputFP); /* Only close outputFP if it was successfully opened */
    }
    return false;
}

    /*Checking if the fopen of the .am file was successful. If not, returning false*/
    if (outputFP ==NULL){
        printf("problem in creating %s",outputFileName);
        free(outputFileName);
        free(inputFileName);
        fclose(inputFP);
        fclose(outputFP);
        return false;
    }



    /*Runs through the file_as line by line.*/
    while(fgets(lineBuffer,MAX_LINE_LENGTH + 2,inputFP)!=NULL){
        char *firstWord=NULL;
        lineIdx = 0;
        firstWord = readNextWord(lineBuffer,&lineIdx);
        
        /*macro write procedure*/
        if(readingMacro == false && (macro = findMacro(head,firstWord)) != NULL){
            writeMacro(macro,outputFP);
            macro = NULL;
            free(firstWord);
            continue;
        }

        /*start reading a new macro*/
        if(strcmp(firstWord,"mcr")==0){
            readingMacro = true;
            macroName = readNextWord(lineBuffer, &lineIdx);
            if (validateMacrosName(head,macroName)==false || hasRemainingContent(lineBuffer,&lineIdx) == true){
                printf("Invalid macro name or characters after name.\n");
                free(macroName);
                free(firstWord);
                success = false;
                break;
            }
            head = createMacro(head, macroName);
            free(macroName);
            free(firstWord);
            continue;
        }

        /*reading a new line to the macro*/
        if (readingMacro == true && strcmp(firstWord,"endmcr")!=0){
            if(appendLineToMacro(head,lineBuffer)==false){
                free(firstWord);
                success = false;
                break;
            }
            free(firstWord);
            continue;            
        }

        /*ending reading a macro*/
        if (strcmp(firstWord,"endmcr")==0){
            if(hasRemainingContent(lineBuffer, &lineIdx)){
                printf("characters after the endmcr");
                free(firstWord);
                success = false;
                break;
            }
            readingMacro = false;
            free(firstWord);
            continue;
        }
        
        fprintf(outputFP,"%s", lineBuffer);
        free(firstWord);
    }
    
    fclose(inputFP); 
    fclose(outputFP); 
    /*if problem in preprocessor delete the am file*/
    if(success == false){
        remove(outputFileName);
    }

     /*free all the dynamic memory*/
    free(outputFileName);
    free(inputFileName);
    freeMacroList(head);
    return success;
}
