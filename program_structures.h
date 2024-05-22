#ifndef PROGRAM_STRUCTURES_H
#define PROGRAM_STRUCTURES_H

#include "globals.h"


/**/
/*This structure containing all the information of the operand */
typedef struct operandInfo
{
    enum{
        immediate,
        direct,
        index,
        registerAddress,
        noAddress=-1
    }operandType;
    union 
    {
        int immediateInt;
        char constant[MAX_LABEL_LENGTH+1];
        char labelName[MAX_LABEL_LENGTH+1];
        struct 
        {
            char constant[MAX_LABEL_LENGTH+1];
            int jump;
        }indexInfo;
        int registerNumber;
        
    }addressData;
}operandInfo;





/* data structure that contains each line information */
typedef struct lineInfo
{
    /*In case of error it should be be !=NULL*/
    char errorMessage[MAX_ERROR_MESSAGE_LENGTH+1];

    /*In case of label in the bigining it should be !=NULL*/
    char labelName[MAX_LABEL_LENGTH+1];
    
    /*Several line types*/
    enum {
        commentLine,
        emptyLine,
        directiveLine,     /*.data, .string, .entry, .extern*/
        instructionLine,   /*add, sub, mov, ... 16 tottal*/
        defineLine
    }lineType;
    union /*Only 1 type of line so it will be better to do a union*/
    {
        /*Handle all the directive types*/
        struct
        {
            enum{
                dataDirective=0,
                stringDirective=1,
                entryDirective=2,
                externDirective=3
            } directiveType;
            union /*only 1 type of info needed*/
            {
                struct{
                    int dataValuesAarray[MAX_INTEGER_NUMBER];/* data */
                    int dataCount;
                }dataInfo;
                char stringConstent[MAX_STRING_LENGTH+1];
                char label[MAX_LABEL_LENGTH+1];         
            }directiveInfo;
        }directive;
    
        /*handel the type of instruction*/
        struct 
        {
            char instructionName[MAX_OPCODE_LENGTH+1];
            operandInfo firstOperand;
            operandInfo secondOperand;
        }instruction;
        
        /*handel the defyne type*/
        struct 
        {
            char constant[MAX_LABEL_LENGTH+1];
            int constentValue;
        }define;
    }lineData;

}lineInfo;


#endif
