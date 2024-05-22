
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "utils.h"
#include "program_tables.h"
#include "parser.h"
#include "program_structures.h"
#include "second_run.h"
#include "globals.h"


boolean secondPassAssembly(char *fileName, tableList* tableHead, int* IC, int* DC, int *memoryImage, labelList **externHead, labelList **entryHead){
    char *outputFileName= appendFileExtension(fileName, ".am");
    FILE *file_am = fopen(outputFileName,"r");
    char line[MAX_LINE_LENGTH + 2];
    lineInfo lineStruct;
    boolean success = true;
    int counter = 0;
    int lineCounter = 0;

    while(fgets(line,MAX_LINE_LENGTH + 2,file_am)!=NULL){
        lineCounter++;
        lineStruct = createLineStructur(line, tableHead);

        /*fills in the gaps in memory, need to check each option of operand to maintain counter of IC*/
        if(lineStruct.lineType == instructionLine){
            counter++;
            /*if first operand is direct*/
            if(lineStruct.lineData.instruction.firstOperand.operandType == direct){
                if(lineStruct.errorMessage[0] == '\0'){
                    if (validateDirectLabel(tableHead, &lineStruct, lineStruct.lineData.instruction.firstOperand.addressData.labelName) == false){
                        success = false;
                        printErrorMessage(outputFileName, lineCounter, &lineStruct);
                    }else{
                        setLabelInMemory(tableHead, externHead, memoryImage, counter, lineStruct.lineData.instruction.firstOperand.addressData.labelName);
                    }
                }
                counter++;
            }
            /*if first operand is index*/
            if(lineStruct.lineData.instruction.firstOperand.operandType == index){
                if(lineStruct.errorMessage[0] == '\0'){
                    if (validateIndexLabel(tableHead, &lineStruct, lineStruct.lineData.instruction.firstOperand.addressData.indexInfo.constant) == false){
                        success = false;
                        printErrorMessage(outputFileName, lineCounter, &lineStruct);
                    }else{
                        setLabelInMemory(tableHead, externHead, memoryImage, counter, lineStruct.lineData.instruction.firstOperand.addressData.indexInfo.constant);
                    }
                }
                counter+=2;
            }
            /*if source and target operands are register*/
            if(lineStruct.lineData.instruction.firstOperand.operandType == registerAddress && lineStruct.lineData.instruction.secondOperand.operandType == registerAddress){
                counter++;
                continue;
            }
            /*if first operand is register or immediate*/
            if(lineStruct.lineData.instruction.firstOperand.operandType == registerAddress || lineStruct.lineData.instruction.firstOperand.operandType == immediate){
                counter++;
            }
            /*if second operand is direct*/
            if(lineStruct.lineData.instruction.secondOperand.operandType == direct){
                if(lineStruct.errorMessage[0] == '\0'){
                    if (validateDirectLabel(tableHead, &lineStruct, lineStruct.lineData.instruction.secondOperand.addressData.labelName) == false){
                        success = false;
                        printErrorMessage(outputFileName, lineCounter, &lineStruct);
                    }else{
                        setLabelInMemory(tableHead, externHead, memoryImage, counter, lineStruct.lineData.instruction.secondOperand.addressData.labelName);
                    }
                }
                counter++;
            }
            /*if second operand is index*/
            if(lineStruct.lineData.instruction.secondOperand.operandType == index){
                if(lineStruct.errorMessage[0] == '\0'){
                    if (validateIndexLabel(tableHead, &lineStruct, lineStruct.lineData.instruction.secondOperand.addressData.indexInfo.constant) == false){
                        success = false;
                        printErrorMessage(outputFileName, lineCounter, &lineStruct);
                    }else{
                        setLabelInMemory(tableHead, externHead, memoryImage, counter, lineStruct.lineData.instruction.secondOperand.addressData.indexInfo.constant);
                    }
                }
                counter+=2;
            }
            /*if second operand is register or immediate*/
            if(lineStruct.lineData.instruction.secondOperand.operandType == registerAddress || lineStruct.lineData.instruction.secondOperand.operandType == immediate){
                counter++;
            }
        }

        if(lineStruct.lineType == directiveLine && lineStruct.lineData.directive.directiveType == entryDirective && lineStruct.errorMessage[0] == '\0'){
            if(isMemmberInTable(lineStruct.lineData.directive.directiveInfo.label, tableHead) == false || 
            (getLabelType(lineStruct.lineData.directive.directiveInfo.label, tableHead) != CODE_LABEL && getLabelType(lineStruct.lineData.directive.directiveInfo.label, tableHead) != DATA_LABEL)){
                strcpy(lineStruct.errorMessage, "Entry label must be declerd in the program as a code label or a data label");
            }else{
                int label_location;
                getConstantNum(lineStruct.lineData.directive.directiveInfo.label, tableHead, &label_location);
                addLabel(entryHead, lineStruct.lineData.directive.directiveInfo.label, label_location);
            }
        }
    }

    free(outputFileName);
    fclose(file_am);
    return success;
}


boolean setLabelInMemory(tableList* tableHead, labelList **externHead, int *memoryImage, int counter, char *label){
    int labelLocation;

    if(getLabelType(label, tableHead) == EXTERN_LABEL){
        memoryImage[counter] = EXTERNAL;
        addLabel(externHead, label, counter);
    }else{
        getConstantNum(label, tableHead, &labelLocation);
        labelLocation+=BASE_MEMORY_ADRESS;
        memoryImage[counter] = (labelLocation) << 2 | RELOCATABLE;
    }
    return true;    
}

boolean validateIndexLabel(tableList* tableHead, lineInfo *lineStruct, char *label){
    /*if not in tableHead, and not data label or extern */
    if(isMemmberInTable(label, tableHead) == false || (getLabelType(label, tableHead) != DATA_LABEL && getLabelType(label, tableHead) != EXTERN_LABEL)){
        strcpy(lineStruct->errorMessage, label);
        strcat(lineStruct->errorMessage,"Should be defined as a data label or extern");
        return false;
    }
    return true;    
}

boolean validateDirectLabel(tableList* tableHead, lineInfo *lineStruct, char *label){
    /*if not in tableHead*/
    if(isMemmberInTable(label, tableHead) == false){
        strcpy(lineStruct->errorMessage, label);
        strcat(lineStruct->errorMessage,"Should be defined as a label or extern");
        return false;
    }
    return true;    
}
