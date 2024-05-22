
#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "utils.h"
#include "program_tables.h"
#include "program_structures.h"
#include "globals.h"


/**
 * @brief Create a line structure from a given line.
 *
 * @param line The line to parse.
 * @param tableHead The head of the table list.
 * @return A populated lineInfo.
 */
lineInfo createLineStructur(char* line, tableList* tableHead);

/**
 * @brief Validate if a given word is a valid label or constant name.
 *
 * @param word The word to validate.
 * @param size The size of the word.
 * @return true if valid, false otherwise.
 */
boolean isValidLabelOrConstantName(char *word, int size);

/**
 * @brief Validate if a given word is a valid instruction.
 *
 * @param word The word to validate.
 * @param lineStruct The line structure to update.
 * @return true if valid, false otherwise.
 */
boolean isValidInstruction(char* word, lineInfo *lineStruct);

/**
 * @brief Parse an operand address from a line.
 *
 * @param line The line to parse.
 * @param point The current parsing position in the line.
 * @param lineStruct The line structure to update.
 * @param addressD The address definition to populate.
 * @param tableHead The head of the table list.
 * @return true if successfully parsed, false otherwise.
 */
boolean parseOperandAddress(char *line, int *point, lineInfo *lineStruct, operandInfo *addressD, tableList *tableHead);

/**
 * @brief Check if there are unwanted characters after an address.
 *
 * @param line The line to check.
 * @param point The current parsing position in the line.
 * @param lineStruct The line structure to update.
 * @return true if no unwanted characters, false otherwise.
 */
boolean hasUnwantedChar(char *line, int *point, lineInfo *lineStruct);

/**
 * @brief Extract an address from a line.
 *
 * @param line The line to parse.
 * @param point The current parsing position in the line.
 * @param lineStruct The line structure to update.
 * @return The extracted address as a string.
 */
char* extractAddress(char *line, int *point, lineInfo *lineStruct);

/**
 * @brief Read a 12-bit number from an address string.
 *
 * @param address The address string to parse.
 * @param num The number to populate.
 * @param lineStruct The line structure to update.
 * @param tableHead The head of the table list.
 * @return true if successfully parsed, false otherwise.
 */
boolean read_12_bitNum(char *address, int *num, lineInfo *lineStruct, tableList* tableHead);

/**
 * @brief Validate the operands in a line structure.
 *
 * @param lineStruct The line structure to validate.
 */
void validateOperand(lineInfo* lineStruct);

/**
 * @brief Print a warning message.
 *
 * @param file_name The name of the file.
 * @param line_counr The line number.
 * @param line_s The line structure containing the warning.
 */
void printErrorMessage(char *filename, int lineCounter, lineInfo *lineStruct);

#endif 