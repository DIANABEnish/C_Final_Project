#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "utils.h"
#include "program_tables.h"
#include "program_structures.h"
#include "directives.h"
#include "parser.h"
#include "globals.h"

/*
 * This function creates a line structure from a given line of code and a table list.
 * The line structure contains information about the type of line, label, instruction,
 * operands, and any warnings or errors.
 */
lineInfo createLineStructur(char* line, tableList* tableHead) {
    int point = 0;
    lineInfo lineStruct = {0};
    char *word;

    lineStruct.errorMessage[0] = '\0';
    lineStruct.labelName[0] = '\0';

    /*Verify if the line length exceeds the maximum allowed length (80)*/
    if(strlen(line) > MAX_LINE_LENGTH) {
        strcpy(lineStruct.errorMessage, "The line exceeds the maximum permitted length");
        return lineStruct;
    }

    skipSpaces(line, &point);
    /*comment_line*/
    if(line[point] == ';') {
        if(point != 0) {
            strcpy(lineStruct.errorMessage, "There should be no whitespace characters before a comment line");
        }
        lineStruct.lineType = commentLine;
        return lineStruct;
    }

    /*blank_line*/
    if(line == NULL || line[point] == '\n') {
        lineStruct.lineType = emptyLine;
        return lineStruct;
    }

    word = readNextWord(line, &point);

    /*check the size of the word*/
    if(strlen(word) == MAX_LABEL_LENGTH + 1) {
        if(line[point] != '\0' && line[point] != ' ' && line[point] != '\t' && line[point] != '\n') {
            strcpy(lineStruct.errorMessage, "The maximum buffer size for labels and directives is 31");
            free(word);
            return lineStruct;
        }
    }
/*the line starts with a label*/
if(word[strlen(word) - 1] == ':') {
 if(isValidLabelOrConstantName(word, strlen(word) - 1) == false) {
    if(strlen(word) + strlen(" is not a valid label name. Labels can only start with a letter and can contain letters and numbers") < sizeof(lineStruct.errorMessage)) {
        strcpy(lineStruct.errorMessage, word);
        strcat(lineStruct.errorMessage, " is not a valid label name. Labels can only start with a letter and can contain letters and numbers");
    } else {
        strcpy(lineStruct.errorMessage, "Error: label name is not valid and the error message is too long to display");
    }
    free(word);
    return lineStruct;
}
    word[strlen(word) - 1] = '\0';
    strcpy(lineStruct.labelName, word);
    free(word);
    word = readNextWord(line, &point);
}

    /*directive line*/
    if(word[0] == '.') {
        char *directive[] = {".data", ".string", ".entry", ".extern", ".define", "NULL"};
        int i = 0;
        int length = 0;

        while(directive[i] != NULL) {
            length = (int)strlen(directive[i]);

            /*if there aren't any white characters after the directive command*/
            if(strncmp(word, directive[i], length) == 0 && length != strlen(word)) {
                strcpy(lineStruct.errorMessage, "There should be a whitespace character after ");
                strcat(lineStruct.errorMessage, directive[i]);
                free(word);
                return lineStruct;
            }

            /*constant definition .define*/
            if(strcmp(".define", word) == 0) {
                lineStruct.lineType = defineLine;
                handeleDefineDirective(line, &point, &lineStruct);
                free(word);
                return lineStruct;
            }

            if(strcmp(directive[i], word) == 0 && strcmp(".define", directive[i]) != 0) {
                lineStruct.lineType = directiveLine;
                lineStruct.lineData.directive.directiveType = i;

                switch(lineStruct.lineData.directive.directiveType) {
                    /*data line*/
                    case dataDirective:
                        handleDataDirective(line, &lineStruct, &point, tableHead);
                        free(word);
                        return lineStruct;
                        break;
                    /*string line*/
                    case stringDirective:
                        handleStringDirective(line, &point, &lineStruct);
                        free(word);
                        return lineStruct;
                        break;

                    /*entry and extern line*/
                    case entryDirective:
                    case externDirective:
                        free(word);
                        word = readNextWord(line, &point);

                        /*check for missing label*/
                        if(word[0] == '\0') {
                            strcpy(lineStruct.errorMessage, "Label is missing after: ");
                            strcat(lineStruct.errorMessage, directive[i]);
                            free(word);
                            return lineStruct;
                        }
                        /*Check if the label size exceeds 31 characters*/
                        if(strlen(word) > MAX_LABEL_LENGTH) {
                            strcpy(lineStruct.errorMessage, "The maximum size of a label is 31");
                            free(word);
                            return lineStruct;
                        }
                        /*check if there are any extra characters after the label*/
                        if(hasRemainingContent(line, &point) == true) {
                            strcpy(lineStruct.errorMessage, "Unwanted characters found after the label declaration");
                            free(word);
                            return lineStruct;
                        }
                        /*check spelling and assign the label to the struct*/
                        if(isValidLabelOrConstantName(word, strlen(word))) {
                            strcpy(lineStruct.lineData.directive.directiveInfo.label, word);
                        } else {
                            strcpy(lineStruct.errorMessage, word);
                            strcat(lineStruct.errorMessage, " is not a valid label name. Labels can only start with a letter and can contain letters and numbers");
                        }

                        free(word);
                        return lineStruct;
                        break;
                }
            }
            i++;
        }

        strcpy(lineStruct.errorMessage, "Unrecognized directive name: ");
        strcat(lineStruct.errorMessage, word);
        free(word);
        return lineStruct;
    }

    /*instruction line*/
    if(isValidInstruction(word, &lineStruct) == true) {
        free(word);

        if(parseOperandAddress(line, &point, &lineStruct, &lineStruct.lineData.instruction.firstOperand, tableHead) == false) {
            return lineStruct;
        }

        skipSpaces(line, &point);

        /*missing comma between operands*/
        if(line[point] != '\0' && line[point] != '\n' && line[point] != ',') {
            strcpy(lineStruct.errorMessage, "Missing comma between source operand and target operand");
            return lineStruct;
        }
        if(line[point] == ',') {
            point++;
            if(parseOperandAddress(line, &point, &lineStruct, &lineStruct.lineData.instruction.secondOperand, tableHead) == false) {
                return lineStruct;
            }
            /*missing target operand after comma*/
            if(lineStruct.lineData.instruction.secondOperand.operandType == noAddress) {
                strcpy(lineStruct.errorMessage, "Target operand is missing after the comma");
                return lineStruct;
            }
        } else {
            lineStruct.lineData.instruction.secondOperand.operandType = noAddress;
            }
       validateOperand(&lineStruct);
       return lineStruct;
   }

   free(word);
   return lineStruct;
}

/*
* This function checks if a given word is a valid label or constant name.
* A valid name can only start with a letter and can contain letters and numbers.
*/
boolean isValidLabelOrConstantName(char *word, int size) {
   int i;

   /*check if the word starts with a character*/
   if(!isalpha(word[0])) {
       return false;
   }

   /*check if the word name contains only numbers and characters*/
   for(i = 0; i < size; i++) {
       if(!isdigit(word[i]) && !isalpha(word[i])) {
           return false;
       }
   }
   return true;
}

/*
* This function checks if a given word is a valid instruction.
* If the word is a valid instruction, it updates the line structure with the instruction name.
*/
boolean isValidInstruction(char* word, lineInfo *lineStruct) {
   int i;
   /*if word is an instruction*/
   for(i = 0; i < COUNT_COMMAND; i++) {
       if(strcmp(commands[i], word) == 0) {
           lineStruct->lineType = instructionLine;
           strcpy(lineStruct->lineData.instruction.instructionName, word);
           return true;
       }
   }
   strcpy(lineStruct->errorMessage, "\"");
   strcat(lineStruct->errorMessage, word);
   strcat(lineStruct->errorMessage, "\" is not a reserved command in the system. A whitespace character is needed after the command");
   return false;
}

/*
* This function parses the operand address from a given line.
* It updates the address structure in the line structure with the appropriate address type and data.
*/
boolean parseOperandAddress(char *line, int *point, lineInfo *lineStruct, operandInfo *addressD, tableList *tableHead) {
   char *address;

   skipSpaces(line, point);

   /*read word up until , or [ or blank or size 32, 31 for word and 1 for #*/
   if((address = extractAddress(line, point, lineStruct)) == NULL) {
       return false;
   }
   /*if no address*/
   if(strlen(address) == 0) {
       free(address);
       /*check if there are any unwanted characters after the register*/
       if(hasUnwantedChar(line, point, lineStruct) == false) {
           return false;
       }
       addressD->operandType = noAddress;
       return true;
   }

   /*if register*/
   if(isRegister(address) == true) {
       char registerNum;
       /*check if there are any unwanted characters after the register*/
       if(hasUnwantedChar(line, point, lineStruct) == false) {
           free(address);
           return false;
       }
       registerNum = address[1];
       addressD->operandType = registerAddress;
       addressD->addressData.registerNumber = registerNum - '0';
       free(address);
       return true;
   }

   /*immediate address*/
   if(address[0] == '#') {
       int num = 0;
       /*Check for unwanted characters after the address*/
       if(hasUnwantedChar(line, point, lineStruct) == false) {
           free(address);
           return false;
       }

       /*Convert the address to a number using a constant or an integer*/
       if(read_12_bitNum(address, &num, lineStruct, tableHead) == false) {
           free(address);
           return false;
       }
       addressD->operandType = immediate;
       addressD->addressData.immediateInt = num;
       free(address);
       return true;
   }

   /*direct and index*/
   if(isValidLabelOrConstantName(address, strlen(address)) == true) {
       /*direct*/
       if(line[*point] != '[') {
           /*Check for unwanted characters after the address*/
           if(hasUnwantedChar(line, point, lineStruct) == false) {
               free(address);
               return false;
           }
           addressD->operandType = direct;
           strcpy(addressD->addressData.labelName, address);
           free(address);
           return true;
       }

       /*index*/
       if(line[*point] == '[') {
           int num = 0;

           addressD->operandType = index;
           strcpy(addressD->addressData.indexInfo.constant, address);
           (*point)++;
           free(address);

           /*Get the argument in []*/
           if((address = extractAddress(line, point, lineStruct)) == NULL) {
               return false;
           }
           /*Convert the address to a number using a constant or an integer*/
           if(read_12_bitNum(address, &num, lineStruct, tableHead) == false) {
               free(address);
               return false;
           }
           addressD->addressData.indexInfo.jump = num;
           free(address);
           /*check for missing ]*/
           if(line[*point] != ']') {
               strcpy(lineStruct->errorMessage, "Closing bracket ']' is missing after the index declaration; no whitespace characters are allowed");
               return false;
           }
           (*point)++;
           return hasUnwantedChar(line, point, lineStruct);
       }
   }
   strcpy(lineStruct->errorMessage, "The address is not a valid label, immediate, register, or array pointer");
   return false;
}

/*
* This function checks if there are any unwanted characters after the address.
* If there are, it checks if the next character is a comma, which is allowed.
* Otherwise, it sets a warning in the line structure.
*/
boolean hasUnwantedChar(char *line, int *point, lineInfo *lineStruct) {
   /*Check for characters after and if there are, make sure it's a comma*/
   if(hasRemainingContent(line, point) == true) {
       skipSpaces(line, point);
       if(line[*point] != ',') {
           strcpy(lineStruct->errorMessage, "Unwanted characters found after the address declaration");
           return false;
       }
   }
   return true;
}

/*
* This function extracts the address from a given line.
* It stops when it encounters '[', ']', ',', end of line, or reaches the maximum label size.
*/
char* extractAddress(char *line, int *point, lineInfo *lineStruct) {
   char* address = (char*)malloc((MAX_LABEL_LENGTH + 2) * sizeof(char));
   int adressSize = 0;

   if(address == NULL) {
       printf("Memory allocation failed while reading next word\n");
       exit(EXIT_FAILURE);
   }
   skipSpaces(line, point);
   /*run through the line and stop when an end of word, '[', ']', ',', or maximum word size is reached*/
   while(line[*point] != '[' && line[*point] != ']' && line[*point] != ',' && line[*point] != '\0' && line[*point] != ' ' && line[*point] != '\t' && line[*point] != '\n') {
       if(adressSize < MAX_LABEL_LENGTH) {
           address[adressSize] = line[*point];
           adressSize++;
           (*point)++;
       } else {
           strcpy(lineStruct->errorMessage, "Maximum buffer size for labels is 31, and integers are limited to -2048 to +2047");
           free(address);
           return NULL;
       }
   }
   address[adressSize] = '\0';
   return address;
}


boolean read_12_bitNum(char *address, int *num, lineInfo *lineStruct, tableList* tableHead) {
   int point = 0;
   int intSize = 0;
   if(address[point] == '#') {
       point++;
   }

   /*input is a number*/
   if(isdigit(address[point]) != 0 || address[point] == '-' || address[point] == '+') {
       while(point < strlen(address)) {
           /*if the address starts with + or -, it's okay*/
           if(intSize == 0 && (address[point] == '-' || address[point] == '+')) {
               point++;
               intSize++;
               continue;
           }

           /*check that all characters are numbers*/
           if(isdigit(address[point]) == 0) {
               strcpy(lineStruct->errorMessage, "Undefined character found in number");
               return false;
           }
           point++;
           intSize++;
       }
       if(intSize > MAX_INTEGER_SIZE) {
           strcpy(lineStruct->errorMessage, "The language supports 12-bit numbers in the range of +2023 to -2024");
           return false;
       }
       /*casting string to integer*/
       if(address[0] == '#') {
           char *endptr;
           *num = strtol(address + 1, &endptr, 10);
       } else {
           *num = atoi(address);
       }

       /*check if the number is within the 12-bit limit*/
       if(*num > MAX_12BIT_NUMBER || *num < MIN_12BIT_NUMBER) {
           strcpy(lineStruct->errorMessage, "The language supports 12-bit numbers in the range of +2047 to -2048");
           return false;
       }
       return true;
   }

   /*if address is not a predefined constant*/
   if(getConstantNum(address + point, tableHead, num) == false || getLabelType(address + point, tableHead) != CONSTANT) {
       strcpy(lineStruct->errorMessage, "Undefined constant name");
       return false;
   }

   /*check if the number is within the 12-bit limit*/
   if(*num > MAX_12BIT_NUMBER || *num < MIN_12BIT_NUMBER) {
       strcpy(lineStruct->errorMessage, "The language supports 12-bit numbers in the range of +2047 to -2048");
       return false;
   }
   return true;
}

/*
* This function validates the operands for a given instruction.
* It checks if the operands are valid based on the instruction type.
* If any operand is invalid, it sets a warning in the line structure.
*/
void validateOperand(lineInfo* lineStruct) {
   int instruction_name;

   /*get the number of the instruction*/
   for(instruction_name = 0; instruction_name < COUNT_COMMAND; instruction_name++) {
       if(strcmp(commands[instruction_name], lineStruct->lineData.instruction.instructionName) == 0) {
           break;
       }
   }

   /*Each instruction has its legal operands*/
   switch(instruction_name) {
       case 0: /*mov*/
       case 2: /*add*/
       case 3: /*sub*/
           if(lineStruct->lineData.instruction.firstOperand.operandType == noAddress) {
               strcpy(lineStruct->errorMessage, "Source operand is missing after the instruction ");
               strcat(lineStruct->errorMessage, lineStruct->lineData.instruction.instructionName);
               return;
           }
           if(lineStruct->lineData.instruction.secondOperand.operandType == noAddress || lineStruct->lineData.instruction.secondOperand.operandType == immediate) {
               strcpy(lineStruct->errorMessage, "Invalid target operand after the instruction ");
               strcat(lineStruct->errorMessage, lineStruct->lineData.instruction.instructionName);
               return;
           }
           break;
       case 1: /*cmp*/
           if(lineStruct->lineData.instruction.firstOperand.operandType == noAddress) {
               strcpy(lineStruct->errorMessage, "Source operand is missing after the instruction ");
               strcat(lineStruct->errorMessage, lineStruct->lineData.instruction.instructionName);
               return;
           }
           if(lineStruct->lineData.instruction.secondOperand.operandType == noAddress) {
               strcpy(lineStruct->errorMessage, "Target operand is missing after the instruction ");
               strcat(lineStruct->errorMessage, lineStruct->lineData.instruction.instructionName);
               return;
           }
           break;
       case 4: /*not*/
       case 5: /*clr*/
       case 7: /*inc*/
       case 8: /*dec*/
       case 11: /*red*/
           if(lineStruct->lineData.instruction.firstOperand.operandType == noAddress || lineStruct->lineData.instruction.firstOperand.operandType == immediate) {
               strcpy(lineStruct->errorMessage, "Invalid target operand after the instruction ");
               strcat(lineStruct->errorMessage, lineStruct->lineData.instruction.instructionName);
               return;
           }
           if(lineStruct->lineData.instruction.secondOperand.operandType != noAddress) {
               strcpy(lineStruct->errorMessage, "Source operand is not allowed after the instruction ");
               strcat(lineStruct->errorMessage, lineStruct->lineData.instruction.instructionName);
               return;
           }
           break;

       case 6: /*lea*/
           if(lineStruct->lineData.instruction.firstOperand.operandType != direct && lineStruct->lineData.instruction.firstOperand.operandType != index) {
               strcpy(lineStruct->errorMessage, "Invalid source operand after the instruction ");
               strcat(lineStruct->errorMessage, lineStruct->lineData.instruction.instructionName);
               return;
           }
           if(lineStruct->lineData.instruction.secondOperand.operandType == noAddress || lineStruct->lineData.instruction.secondOperand.operandType == immediate) {
               strcpy(lineStruct->errorMessage, "Invalid target operand after the instruction ");
               strcat(lineStruct->errorMessage, lineStruct->lineData.instruction.instructionName);
               return;
           }
           break;

       case 9: /*jmp*/
       case 10: /*bne*/
       case 13: /*jsr*/
           if(lineStruct->lineData.instruction.firstOperand.operandType != direct && lineStruct->lineData.instruction.firstOperand.operandType != registerAddress) {
               strcpy(lineStruct->errorMessage, "Invalid target operand after the instruction ");
               strcat(lineStruct->errorMessage, lineStruct->lineData.instruction.instructionName);
               return;
           }
           if(lineStruct->lineData.instruction.secondOperand.operandType != noAddress) {
               strcpy(lineStruct->errorMessage, "Source operand is not allowed after the instruction ");
               strcat(lineStruct->errorMessage, lineStruct->lineData.instruction.instructionName);
               return;
           }
           break;
       case 12: /*prn*/
           if(lineStruct->lineData.instruction.firstOperand.operandType == noAddress) {
               strcpy(lineStruct->errorMessage, "Target operand is missing after the instruction ");
               strcat(lineStruct->errorMessage, lineStruct->lineData.instruction.instructionName);
               return;
           }
           if(lineStruct->lineData.instruction.secondOperand.operandType != noAddress) {
               strcpy(lineStruct->errorMessage, "Source operand is not allowed after the instruction ");
strcat(lineStruct->errorMessage, lineStruct->lineData.instruction.instructionName);
return;
}
break;
case 14: /*rts*/
case 15: /*hlt*/
if(lineStruct->lineData.instruction.firstOperand.operandType != noAddress) {
strcpy(lineStruct->errorMessage, "No target operand is allowed after the instruction ");
strcat(lineStruct->errorMessage, lineStruct->lineData.instruction.instructionName);
return;
}
if(lineStruct->lineData.instruction.secondOperand.operandType != noAddress) {
strcpy(lineStruct->errorMessage, "Source operand is not allowed after the instruction ");
strcat(lineStruct->errorMessage, lineStruct->lineData.instruction.instructionName);
return;
}
break;
}
}
/*

This function prints a warning message with the file name, line number, and warning string
from the line structure.
*/
void printErrorMessage(char *filename, int lineCounter, lineInfo *lineStruct) {
printf("\nCaution! file: %s line number: %d, %s", filename, lineCounter, lineStruct->errorMessage);
}