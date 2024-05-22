

#ifndef FIRST_RUN_H
#define FIRST_RUN_H

#include "program_tables.h"
#include "program_structures.h"
#include "globals.h"

/**
 * @brief Processes an assembly file and populates the given data structures.
 *
 * @param file_name The name of the assembly file to be processed.
 * @param tableHead A pointer to the head of the table list.
 * @param IC A pointer to the instruction counter.
 * @param DC A pointer to the data counter.
 * @param memoryImage A pointer to the memory image array.
 * @param externHead A pointer to the head of the extern label list.
 * @param entryHead A pointer to the head of the entry label list.
 *
 * @return true if the file was processed successfully, false otherwise.
 */
boolean firstPassAssembly(char *file_name, tableList **tableHead, int *IC, int *DC, int *memoryImage, labelList **externHead, labelList **entryHead);


/**
 * @brief Handles directive lines in the assembly file.
 *
 * @param lineStruct The line structure containing the directive information.
 * @param IC A pointer to the instruction counter.
 * @param DC A pointer to the data counter.
 * @param DC_memoryImage A pointer to the data memory image array.
 * @param tableHead A pointer to the head of the table list.
 * @param externHead A pointer to the head of the extern label list.
 * @param entryHead A pointer to the head of the entry label list.
 * @param success A pointer to a boolean indicating the success of the operation.
 * @param fileName The name of the assembly file being processed.
 * @param lineCounter The current line number in the assembly file.
 */
void handleDirectiveLine(lineInfo, int *IC, int *DC, int **DC_memoryImage, tableList **tableHead, labelList **externHead, labelList **entryHead, boolean *success, char *fileName, int lineCounter);


/**
 * @brief Handles instruction lines in the assembly file.
 *
 * @param lineStruct The line structure containing the instruction information.
 * @param IC A pointer to the instruction counter.
 * @param memoryImage A pointer to the memory image array.
 * @param tableHead A pointer to the head of the table list.
 * @param success A pointer to a boolean indicating the success of the operation.
 * @param fileName The name of the assembly file being processed.
 * @param lineCounter The current line number in the assembly file.
 */
void handleInstructionLine(lineInfo, int *IC, int *memoryImage, tableList **tableHead, boolean *success, char *fileName, int lineCounter);


/**
 * @brief Handles define lines in the assembly file.
 *
 * @param lineStruct The line structure containing the define information.
 * @param tableHead A pointer to the head of the table list.
 * @param success A pointer to a boolean indicating the success of the operation.
 * @param fileName The name of the assembly file being processed.
 * @param lineCounter The current line number in the assembly file.
 */
void handleDefineLine(lineInfo, tableList **tableHead, boolean *success,  char *fileName, int lineCounter);


/**
 * @brief Handles data and string directives in the assembly file.
 *
 * @param lineStruct The line structure containing the data or string directive information.
 * @param DC A pointer to the data counter.
 * @param DC_memoryImage A pointer to the data memory image array.
 * @param tableHead A pointer to the head of the table list.
 * @param success A pointer to a boolean indicating the success of the operation.
 * @param fileName The name of the assembly file being processed.
 * @param lineCounter The current line number in the assembly file.
 */
void handleDataStringDirective(lineInfo, int *DC, int **DC_memoryImage, tableList **tableHead, boolean *success, char *fileName, int lineCounter);


/**
 * @brief Copies data from the data memory image to the main memory image.
 *
 * @param memoryImage A pointer to the main memory image array.
 * @param IC A pointer to the instruction counter.
 * @param DC A pointer to the data counter.
 * @param DC_memoryImage A pointer to the data memory image array.
 */
void dataToMemmoryImage(int *memoryImage, int *IC, int *DC, int *DC_memoryImage);


/**
 * @brief Stores data and strings in the data memory image.
 *
 * @param lineStruct The line structure containing the data or string information.
 * @param DC A pointer to the data counter.
 * @param DC_memoryImage A pointer to the data memory image array.
 */
void storeDataAndStringsInMemory(lineInfo*lineStruct, int *DC, int **DC_memoryImage);


/**
 * @brief Handles extern and entry directives in the assembly file.
 *
 * @param lineStruct The line structure containing the extern or entry directive information.
 * @param IC The instruction counter value.
 * @param tableHead A pointer to the head of the table list.
 * @param externHead A pointer to the head of the extern label list.
 * @param entryHead A pointer to the head of the entry label list.
 * @param success A pointer to a boolean indicating the success of the operation.
 */
void handleExternAndEntryDirectives(lineInfo*lineStruct, int IC, tableList** tableHead, labelList **externHead, labelList **entryHead, boolean *success);


/**
 * @brief Allocates or reallocates memory for the data memory image.
 *
 * @param DC A pointer to the data counter.
 * @param DC_memoryImage A pointer to the data memory image array.
 * @param count The number of additional elements to allocate.
 */
void allocat_DC_Memmory (int *DC, int **DC_memoryImage, int count);


/**
 * @brief Encodes an instruction into the memory image.
 *
 * @param lineStruct The line structure containing the instruction information.
 * @param IC A pointer to the instruction counter.
 * @param memoryImage A pointer to the memory image array.
 */
void encodeInstruction(lineInfo,int *IC,int *memoryImage);


/**
 * @brief Encodes an operand into the memory image.
 *
 * @param IC A pointer to the instruction counter.
 * @param memoryImage A pointer to the memory image array.
 * @param operand The operand to be encoded.
 * @param type The type of the operand.
 */
void encodeOperand(int *IC, int *memoryImage, operandInfo operand,int type);

/**
 * @brief Checks if a word is a valid identifier.
 *
 * @param word The word to be checked.
 * @param lineStruct The line structure containing the word.
 * @param tableHead A pointer to the head of the table list.
 *
 * @return true if the word is a valid identifier, false otherwise.
 */
boolean isValliedWord(char *word, lineInfo*lineStruct, tableList* tableHead);



#endif /* FIRST_RUN_H */