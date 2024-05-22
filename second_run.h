

#ifndef SECOND_RUN_H
#define SECOND_RUN_H

#include "program_tables.h"
#include "program_structures.h"

/**
 * @brief Performs the second pass of the assembly process.
 *
 * @param fileName The name of the input file.
 * @param tableHead The head of the symbol table list.
 * @param IC Pointer to the instruction counter.
 * @param DC Pointer to the data counter.
 * @param memoryImage Pointer to the memory image array.
 * @param externHead Pointer to the head of the extern label list.
 * @param entryHead Pointer to the head of the entry label list.
 * @return true if the second pass is successful, false otherwise.
 */
boolean secondPassAssembly(char *fileName, tableList* tableHead, int* IC, int* DC, int *memoryImage, labelList **externHead, labelList **entryHead);

/**
 * @brief Sets a label in the memory image.
 *
 * @param tableHead The head of the symbol table list.
 * @param externHead Pointer to the head of the extern label list.
 * @param memoryImage Pointer to the memory image array.
 * @param counter The current index in the memory image.
 * @param label The label to set.
 * @return true if the label is set successfully, false otherwise.
 */
boolean setLabelInMemory(tableList* tableHead, labelList **externHead, int *memoryImage, int counter, char *label);

/**
 * @brief Validates an index label.
 *
 * @param tableHead The head of the symbol table list.
 * @param lineStruct Pointer to the line information structure.
 * @param label The label to validate.
 * @return true if the label is valid, false otherwise.
 */
boolean validateIndexLabel(tableList* tableHead, lineInfo *lineStruct, char *label);

/**
 * @brief Validates a direct label.
 *
 * @param tableHead The head of the symbol table list.
 * @param lineStruct Pointer to the line information structure.
 * @param label The label to validate.
 * @return true if the label is valid, false otherwise.
 */
boolean validateDirectLabel(tableList* tableHead, lineInfo *lineStruct, char *label);

#endif /* SECOND_RUN_H */