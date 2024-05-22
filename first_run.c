#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "utils.h"
#include "program_tables.h"
#include "parser.h"
#include "program_structures.h"
#include "first_run.h"
#include "globals.h"




boolean firstPassAssembly(char *file_name, tableList **tableHead, int *IC, int *DC, int *memoryImage, labelList **externHead, labelList **entryHead) {
    char *outputFileName = appendFileExtension(file_name, ".am");
    FILE *file_am;
    boolean success = true;
    int lineCounter = 0;
    char line[MAX_LINE_LENGTH + 2];
    int *DC_memoryImage = NULL;
    lineInfo lineStruct;

    file_am = fopen(outputFileName, "r");
    if (file_am == NULL) {
        perror("Error opening file");
        free(outputFileName);
        return false;
    }

    while (fgets(line, MAX_LINE_LENGTH + 2, file_am) != NULL) {
        lineStruct = createLineStructur(line, *tableHead);
        lineCounter++;
        if (lineStruct.errorMessage[0] != '\0') {
            success = false;
            printErrorMessage(outputFileName, lineCounter, &lineStruct);
            continue;
        }

        switch (lineStruct.lineType) {
            case commentLine:
            case emptyLine:
                break;

            case directiveLine:
                handleDirectiveLine(lineStruct, IC, DC, &DC_memoryImage, tableHead, externHead, entryHead, &success, outputFileName, lineCounter);
                break;

            case instructionLine:
                handleInstructionLine(lineStruct, IC, memoryImage, tableHead, &success, outputFileName, lineCounter);
                break;

            case defineLine:
                handleDefineLine(lineStruct, tableHead, &success, outputFileName, lineCounter);
                break;
        }
    }

    if ((*IC) + (*DC) > MEMORY_CAPACITY) {
        printf("Insufficient program memory\n");
        success = false;
    } else {
        dataToMemmoryImage(memoryImage, IC, DC, DC_memoryImage);
    }

    updateLabelValues(*tableHead, *IC);
    free(DC_memoryImage);
    free(outputFileName);
    fclose(file_am);
    return success;
}

void handleDirectiveLine(lineInfo lineStruct, int *IC, int *DC, int **DC_memoryImage, tableList **tableHead, labelList **externHead, labelList **entryHead, boolean *success, char *fileName, int lineCounter) {
    if (lineStruct.lineData.directive.directiveType == dataDirective || lineStruct.lineData.directive.directiveType == stringDirective) {
        handleDataStringDirective(lineStruct, DC, DC_memoryImage, tableHead, success, fileName, lineCounter);
    } else if (lineStruct.lineData.directive.directiveType == entryDirective || lineStruct.lineData.directive.directiveType == externDirective) {
        handleExternAndEntryDirectives(&lineStruct, *IC, tableHead, externHead, entryHead, success);
    }
}

void handleInstructionLine(lineInfo lineStruct, int *IC, int *memoryImage, tableList **tableHead, boolean *success, char *fileName, int lineCounter) {
    if (lineStruct.labelName[0] != '\0') {
        if (isValliedWord(lineStruct.labelName, &lineStruct, *tableHead)) {
            addTableMammber(CODE_LABEL, lineStruct.labelName, *IC, tableHead);
        }
    }

    if (lineStruct.errorMessage[0] != '\0') {
        *success = false;
        printErrorMessage(fileName, lineCounter, &lineStruct);
    } else if (*IC < MEMORY_CAPACITY) {
        encodeInstruction(lineStruct, IC, memoryImage);
    } else {
        printf("Insufficient program memory\n");
        *success = false;
    }
}

void handleDefineLine(lineInfo lineStruct, tableList **tableHead, boolean *success,  char *fileName, int lineCounter) {
    if (lineStruct.errorMessage[0] != '\0' || !isValliedWord(lineStruct.lineData.define.constant, &lineStruct, *tableHead)) {
        *success = false;
        printErrorMessage(fileName, lineCounter, &lineStruct);
    } else {
        addTableMammber(CONSTANT, lineStruct.lineData.define.constant, lineStruct.lineData.define.constentValue, tableHead);
    }
}

void handleDataStringDirective(lineInfo lineStruct, int *DC, int **DC_memoryImage, tableList **tableHead, boolean *success, char *fileName, int lineCounter) {
    if (lineStruct.labelName[0] != '\0' && isValliedWord(lineStruct.labelName, &lineStruct, *tableHead)) {
        addTableMammber(DATA_LABEL, lineStruct.labelName, *DC, tableHead);
    }

    if (lineStruct.errorMessage[0] != '\0') {
        *success = false;
        printErrorMessage(fileName, lineCounter, &lineStruct);
    } else {
        storeDataAndStringsInMemory(&lineStruct, DC, DC_memoryImage);
    }
}

void dataToMemmoryImage(int *memoryImage, int *IC, int *DC, int *DC_memoryImage) {
    int i;
    for (i = 0; i < *DC; i++) {
        memoryImage[(*IC) + i] = DC_memoryImage[i];
    }
}


void storeDataAndStringsInMemory(lineInfo *lineStruct, int *DC, int **DC_memoryImage){
    int i;

    /*building a memory block for data*/
    if(lineStruct->lineData.directive.directiveType == dataDirective){

        /*allocating dynamic memory*/
        allocat_DC_Memmory (DC, DC_memoryImage, lineStruct->lineData.directive.directiveInfo.dataInfo.dataCount);

        /*Go through the array and put it in a DC memory image*/
        for(i=0; i < lineStruct->lineData.directive.directiveInfo.dataInfo.dataCount; i++){
            (*DC_memoryImage)[*DC] = lineStruct->lineData.directive.directiveInfo.dataInfo.dataValuesAarray[i];
            (*DC)++;
        }
    }

    /*building a memory block for string*/
    if(lineStruct->lineData.directive.directiveType == stringDirective){
        int string_length = strlen(lineStruct->lineData.directive.directiveInfo.stringConstent);
        
        /*allocating dynamic memory*/
        allocat_DC_Memmory (DC, DC_memoryImage, string_length+1);
        
        /*Go through the string and put it in a DC memory image*/
        for(i=0; i < string_length; i++){
            (*DC_memoryImage)[*DC] = (int)lineStruct->lineData.directive.directiveInfo.stringConstent[i];
            (*DC)++;
        }
        /*add '\0' in the end*/
        (*DC_memoryImage)[*DC] = 0;
        (*DC)++;
    }
}

void handleExternAndEntryDirectives(lineInfo *lineStruct, int IC, tableList** tableHead, labelList **externHead, labelList **entryHead, boolean *success){

    /*the line is entry or extern and the same label name cannot contain both*/
    if(isLabelInList(lineStruct->lineData.directive.directiveInfo.label, *externHead) == true || isLabelInList(lineStruct->lineData.directive.directiveInfo.label, *entryHead)){
            strcpy(lineStruct->errorMessage, lineStruct->lineData.directive.directiveInfo.label);
            strcat(lineStruct->errorMessage,"This identifier is already defined as an entry or extern label");
            success = false;
            return;
    }

    /*no need to build a memory block for entry will happen in second run*/
    
    /*building a memory block for extern*/
    if(lineStruct->lineData.directive.directiveType == externDirective){

        /*extern cannot be defined as another label*/
        if(isValliedWord(lineStruct->lineData.directive.directiveInfo.label, lineStruct, *tableHead) == false){
            success = false;
            return;
        }
        addTableMammber(EXTERN_LABEL, lineStruct->lineData.directive.directiveInfo.label, -1, tableHead);
    }    

}

void allocat_DC_Memmory (int *DC, int **DC_memoryImage, int count){
    if(*DC == 0){
            *DC_memoryImage = (int*)malloc(sizeof(int)*count);
            if(*DC_memoryImage == NULL){
                printf("Failed to allocate memory\n");
                exit(1);  
            }
        }else{
            *DC_memoryImage = (int*)realloc(*DC_memoryImage, sizeof(int) * (*DC + count));
            if(*DC_memoryImage == NULL){
                printf("Failed to allocate memory\n");
                exit(1);  
            }
        }
}

void encodeInstruction(lineInfo lineStruct,int *IC,int *memoryImage){
    int instructionCode = getCommandIndex(lineStruct.lineData.instruction.instructionName);
    int sourceOperand, targetOperand;
   
    /*if target and source are no_address*/
    if(lineStruct.lineData.instruction.firstOperand.operandType == noAddress){
        sourceOperand = 0;
        targetOperand = 0;
        memoryImage[*IC ] = (instructionCode << 6);
        (*IC)++;

    /*if only 1 operand and it is target*/
    }else if (lineStruct.lineData.instruction.secondOperand.operandType == noAddress){
        sourceOperand = 0;
        targetOperand = lineStruct.lineData.instruction.firstOperand.operandType;
        memoryImage[*IC] = (instructionCode << 6) | (targetOperand << 2);
        (*IC)++;
        encodeOperand(IC, memoryImage, lineStruct.lineData.instruction.firstOperand, TARGET_OPERAND);
    
    /*both operand exist*/
    }else{
        sourceOperand = lineStruct.lineData.instruction.firstOperand.operandType;
        targetOperand = lineStruct.lineData.instruction.secondOperand.operandType;
        memoryImage[*IC] = (instructionCode << 6) | (sourceOperand << 4) | (targetOperand << 2);
        (*IC)++;

        /*is source and target are register that have the same line*/
        if(lineStruct.lineData.instruction.firstOperand.operandType == registerAddress && lineStruct.lineData.instruction.secondOperand.operandType ==registerAddress){
            memoryImage[*IC] = lineStruct.lineData.instruction.firstOperand.addressData.registerNumber << 5 | lineStruct.lineData.instruction.secondOperand.addressData.registerNumber << 2;
            (*IC)++;
        }else{
            encodeOperand(IC, memoryImage, lineStruct.lineData.instruction.firstOperand, SOURCE_OPERAND);
            encodeOperand(IC, memoryImage, lineStruct.lineData.instruction.secondOperand, TARGET_OPERAND);
        }
    }
}

void encodeOperand(int *IC, int *memoryImage, operandInfo operand,int type){
    switch (operand.operandType)
    {
    case immediate:
        memoryImage[*IC] = operand.addressData.immediateInt << 2;
        (*IC)++;
        break;

    case direct:
        memoryImage[*IC] = -1;/*the label tabel isn't ready in this stage and will be filled in second pass*/
        (*IC)++;
        break;

    case index:
        memoryImage[*IC] = -1;/*the label tabel isn't ready in this stage and will be filled in second pass*/
        (*IC)++;
        memoryImage[*IC] = operand.addressData.indexInfo.jump << 2;
        (*IC)++;
    break;

    case registerAddress:
        if(type == TARGET_OPERAND){
            memoryImage[*IC] = operand.addressData.registerNumber << 2;
        }else{
            memoryImage[*IC] = operand.addressData.registerNumber << 5;
        }
        (*IC)++;
        break;
    default:
        return;
        break;
    }
}

boolean isValliedWord(char *word, lineInfo *lineStruct, tableList* tableHead){
    /*Check if already defined*/
    if(lineStruct->errorMessage[0] == '\0' && isMemmberInTable(word, tableHead) == true){
        strcpy(lineStruct->errorMessage, word);
        strcat(lineStruct->errorMessage,"This identifier is already defined in the program");
        return false;
    }
    /*check if reserved word in the system*/
    if(lineStruct->errorMessage[0] == '\0' && (isCommand(word) == true || isRegister(word) == true || isReservedWord(word) == true)){
        strcpy(lineStruct->errorMessage, word);
        strcat(lineStruct->errorMessage,"This is a reserved word in the language");
        return false;
    }
    return true;
}