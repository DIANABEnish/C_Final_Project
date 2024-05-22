#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "utils.h"
#include "macro.h"
#include "globals.h"


const char *registers[COUNT_REGISTER] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

const char *commands[COUNT_COMMAND] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", 
"dec", "jmp", "bne", "red", "prn", "jsr", "rts", "hlt"};

const char *reservedWords[COUNT_RESERVED_WORD] = {"define", "extern", "entry", "string", "data", "endmcr", "mcr"};


char* appendFileExtension(char* file, char* extension) {
    size_t fileLen = strlen(file);
    size_t extensiontLen = strlen(extension);
    char* result = (char*)malloc(fileLen + extensiontLen + 1);
    if (result) {
        strcpy(result, file);
        strcat(result, extension);
    }
    return result;
}

void skipSpaces(char* line, int* position) {
    while (line[*position] != '\0' && (line[*position] == ' ' || line[*position] == '\t')) {
        (*position)++;
    }
}

boolean isRegister(char *inputWord){
	int i;
	for(i=0; i < COUNT_REGISTER; i++){
		if (strcmp(registers[i],inputWord)==0){
			return true;
		}
	}
	return false;
}

boolean isCommand(char *inputWord){
	int i;
	for(i=0; i < COUNT_COMMAND; i++){
		if (strcmp(commands[i],inputWord)==0){
			return true;
		}
	}
	return false;
}

boolean isReservedWord(char* inputWord) {
	int i;
    for ( i = 0; i < COUNT_RESERVED_WORD; i++) {
        if (strcmp(reservedWords[i], inputWord) == 0) {
            return true;
        }
    }
    return false;
}

boolean isStartingWithLetter( char* inputWord) {
    return isalpha(inputWord[0]) != 0;
}

char* readNextWord( char* line, int* position) {
    int length = 0;
    char* word = (char*)malloc((MAX_MACRO_NAME_LENGTH + 2) * sizeof(char));
    if (word == NULL) {
        printf("Memory allocation failed in readNextWord\n");
        return NULL;
    }

    skipSpaces(line, position);

    while (line[*position] != '\0' && line[*position] != ' ' && line[*position] != '\t' && line[*position] != '\n' && length < MAX_MACRO_NAME_LENGTH + 1) {
        word[length] = line[*position];
        length++;
        (*position)++;
    }
    word[length] = '\0';

    return word;
}

boolean hasRemainingContent(char* line, int* point){
	skipSpaces(line, point);
	if(line[*point] == '\0'||line[*point] == '\n'){
		return false;
	}
	return true;
}

int getCommandIndex(char* str) {
	int i;
    if (strlen(str) == 0) {
        return -1;
    }

    for (i = 0; i < COUNT_COMMAND; i++) {
        if (strcmp(commands[i], str) == 0) {
            return i;
        }
    }
      return -1;
}