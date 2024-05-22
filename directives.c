#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "utils.h"
#include "program_tables.h"
#include "parser.h"
#include "directives.h"
#include "program_structures.h"
#include "globals.h"

char* readDefineConstantName(char* line, int* point);

void handleStringDirective(char* line, int* point, lineInfo *lineStruct) {
    int lastPointer = strlen(line) - 1;
    int counter = 0;
    char string[MAX_STRING_LENGTH + 1] = {'\0'};

    /*Finding the first quotation mark in the line*/ 
    skipSpaces(line, point);
    if (line[*point] != 34) {
        strcpy(lineStruct->errorMessage, "Starting quotation mark missing in .string directive");
        return;
    }
    (*point)++;
    
    /*Finding the last quotation mark in the line*/ 
    while (line[lastPointer] != 34) {
        if (line[lastPointer] != '\0' && line[lastPointer] != ' ' && line[lastPointer] != '\t' && line[lastPointer] != '\n') {
            strcpy(lineStruct->errorMessage, "Characters outside of quotation marks in .string directive");
            return;
        }
        lastPointer--;
    }

    /*Process the string, ensure it can be printed, and store it in the struct*/ 
    for (; *point < lastPointer; (*point)++) {
        if (isprint(line[*point]) == 0) {
            strcpy(lineStruct->errorMessage, "Non-printable character found");
            return;
        }
        string[counter] = line[*point];
        counter++;
    }
    strcpy(lineStruct->lineData.directive.directiveInfo.stringConstent, string);
}

void handeleDefineDirective(char* line, int* point, lineInfo *lineStruct) {
    char *constantName = readDefineConstantName(line, point);
    int intSize = 0;
    int num;

    /*Check if there is a label*/ 
    if (lineStruct->labelName[0] != '\0') {
        strcpy(lineStruct->errorMessage, "Labels are not permitted before .define");
        free(constantName);
        return;
    }

    /*Check for missing data after define*/
    if (constantName[0] == '\0') {
        strcpy(lineStruct->errorMessage, "Constant name or integer missing after .define");
        free(constantName);
        return;
    }
    if (strlen(constantName) > MAX_LABEL_LENGTH) {
        strcpy(lineStruct->errorMessage, "Constant name exceeds maximum length of 31 characters");
        free(constantName);
        return;
    }

    /*Validate constant name*/ 
    if (isValidLabelOrConstantName(constantName, strlen(constantName)) == false) {
        strcpy(lineStruct->errorMessage, constantName);
        strcat(lineStruct->errorMessage, " is an invalid constant name. Must start with a letter and contain only letters and digits");
        free(constantName);
        return;
    }

    strcpy(lineStruct->lineData.define.constant, constantName);
    free(constantName);

    /* Check for the equals sign*/
    skipSpaces(line, point);
    if (line[*point] != '=') {
        strcpy(lineStruct->errorMessage, "Equals sign missing between constant name and value");
    }
    (*point)++;
    skipSpaces(line, point);

    /*Read and validate the number*/ 
    if (readInteger(line, point, &num, lineStruct, &intSize) == false) {
        return;
    }
    if (intSize == 0) {
        strcpy(lineStruct->errorMessage, "Integer value missing in .define directive");
        return;
    }

    /*Check for unwanted characters after the number*/ 
    if (hasRemainingContent(line, point) == true) {
        strcpy(lineStruct->errorMessage, "Extra characters found after integer in .define directive");
        return;
    }

    lineStruct->lineData.define.constentValue = num;
}

char* readDefineConstantName(char* line, int* point) {
    char* constant = (char*)malloc((MAX_LABEL_LENGTH + 2) * sizeof(char));
    int constantSize = 0;

    if (constant == NULL) {
        printf("Failed to allocate memory for constant name\n");
        exit(EXIT_FAILURE);
    }

    /*Extract constant name*/ 
    skipSpaces(line, point);
    while (line[*point] != '=' && line[*point] != '\0' && line[*point] != ' ' && line[*point] != '\t' && line[*point] != '\n') {
        if (constantSize <= MAX_LABEL_LENGTH) {
            constant[constantSize] = line[*point];
            constantSize++;
            (*point)++;
        } else {
            break;
        }
    }
    constant[constantSize] = '\0';
    return constant;
}

boolean readInteger(char* line, int* point, int *result, lineInfo *lineStruct, int *intSize) {
    char str[MAX_INTEGER_SIZE] = {'\0'};

    /*Scan the number in the line*/ 
    while (line[*point] != '\0' && line[*point] != ' ' && line[*point] != '\t' && line[*point] != '\n' && line[*point] != ',') {

        /*Ensure the number is not too large*/ 
        if (*intSize < MAX_INTEGER_SIZE) {

            /*Handle initial + or - sign*/ 
            if (*intSize == 0 && (line[*point] == '+' || line[*point] == '-')) {
                str[*intSize] = line[*point];
                (*intSize)++;
                (*point)++;
                continue;
            }

            /*Check for numeric character*/ 
            if (!isdigit(line[*point])) {
                strcpy(lineStruct->errorMessage, "Invalid character in integer. Only digits are allowed");
                return false;
            }

            str[*intSize] = line[*point];
            (*intSize)++;
            (*point)++;
        } else {
            strcpy(lineStruct->errorMessage, "Integer exceeds valid range -8192 to +8191");
            return false;
        }
    }

    /*Convert string to integer*/ 
    str[*intSize] = '\0';
    *result = atoi(str);

    
    if (*result > MAX_14BIT_NUMBER || *result < MIN_14BIT_NUMBER) {
        strcpy(lineStruct->errorMessage, "Integer exceeds valid range -8192 to +8191");
        return false;
    }
    return true;
}

void handleDataDirective(char* line, lineInfo *lineStruct, int *point, tableList* tableHead) {
    char *str = NULL;
    int num = 0, count = 0;

    lineStruct->lineData.directive.directiveInfo.dataInfo.dataCount = count;
    skipSpaces(line, point);

    /*Check for leading comma*/ 
    if (line[*point] == ',') {
        strcpy(lineStruct->errorMessage, "Unexpected comma before first integer/constant");
        return;
    }

    /*Parse integers/constants separated by commas*/ 
    str = strtok(line + *point, ",");
    while (str != NULL && count <= MAX_INTEGER_NUMBER) {
        if (extractInteger(str, lineStruct, tableHead, &num) == false) {
            return;
        }
        lineStruct->lineData.directive.directiveInfo.dataInfo.dataValuesAarray[count] = num;
        count++;
        str = strtok(NULL, ",");
    }
    lineStruct->lineData.directive.directiveInfo.dataInfo.dataCount = count;
}

boolean extractInteger(char* str, lineInfo* lineStruct, tableList* tableHead, int* result) {
    int leftPoint = 0;
    int count = 0;
    int rightPoint = strlen(str) - 1;
    char word[MAX_LINE_LENGTH] = {'\0'};
    int i;

    skipSpaces(str, &leftPoint);

    if (leftPoint > rightPoint) {
        strcpy(lineStruct->errorMessage, "Integer/constant missing after comma");
        return false;
    }

    /*Remove trailing whitespace*/ 
    while (str[rightPoint] == ' ' || str[rightPoint] == '\t' || str[rightPoint] == '\n') {
        rightPoint--;
        count++;
    }

    strncpy(word, str + leftPoint, strlen(str) - leftPoint - count);

    /* Check for missing integer*/
    if (strlen(word) == 0) {
        strcpy(lineStruct->errorMessage, "Integer/constant missing after comma");
        return false;
    }

    /*Check for whitespace within word*/ 
    for (i = 0; i < strlen(word); i++) {
        if (word[i] == ' ' || word[i] == '\t' || word[i] == '\n') {
            strcpy(lineStruct->errorMessage, "Comma missing between integers/constants");
            return false;
        }
    }

    /*Check if first character is digit or sign*/ 
    if (isdigit(word[0]) != 0 || word[0] == '+' || word[0] == '-') {

        
        for (i = 1; i < strlen(word); i++) {
            if (!isdigit(word[i])) {
                strcpy(lineStruct->errorMessage, "Only digits are allowed in integer");
                return false;
            }
        }

        /*Convert string to integer*/ 
        if (strlen(word) > MAX_INTEGER_SIZE) {
            strcpy(lineStruct->errorMessage, "Integer size exceeds valid range -8192 to +8191");
            return false;
        }
        *result = atoi(str);

        /*Validate integer range*/ 
        if (*result > MAX_14BIT_NUMBER || *result < MIN_14BIT_NUMBER) {
            strcpy(lineStruct->errorMessage, "Integer size exceeds valid range -8192 to +8191");
            return false;
        }
        return true;
    }

    /*Handle constants*/ 
    if (getConstantNum(word, tableHead, result) == false || getLabelType(word, tableHead) != CONSTANT) {
        strcpy(lineStruct->errorMessage, word);
        strcat(lineStruct->errorMessage, " is neither a valid integer nor a predefined constant");
        return false;
    }
    return true;
}
