/**
 * @file directives.h
 * @brief Header file for handling assembly directives.
 */

#ifndef DIRECTIVES_H
#define DIRECTIVES_H


#include "program_tables.h"
#include "program_structures.h"





/**
 * @brief Handles the .string directive.
 *
 * This function reads the string from the input line and stores it in the
 * lineInfo.
 *
 * @param line The input line containing the .string directive.
 * @param point A pointer to the current position in the line.
 * @param lineStruct A pointer to the lineInfo structure where the string
 *                   will be stored.
 */
void handleStringDirective(char* line, int* point, lineInfo *lineStruct);

/**
 * @brief Handles the .define directive.
 *
 * This function reads the constant name and value from the input line and
 * stores them in the lineInfo.
 *
 * @param line The input line containing the .define directive.
 * @param point A pointer to the current position in the line.
 * @param lineStruct A pointer to the lineInfo structure where the constant
 *                   name and value will be stored.
 */
void handeleDefineDirective(char* line, int* point, lineInfo *lineStruct);


/**
 * @brief Reads an integer value from the input line.
 *
 * This function reads an integer value from the input line and stores it in the
 * result parameter.
 *
 * @param line The input line containing the integer value.
 * @param point A pointer to the current position in the line.
 * @param result A pointer to an integer where the value will be stored.
 * @param lineStruct A pointer to the lineInfo structure.
 * @param intSize A pointer to an integer where the size of the integer will be
 *                stored.
 * @return true if the integer value is successfully read, false otherwise.
 */
boolean readInteger(char* line, int* point, int *result, lineInfo *lineStruct, int *intSize);

/**
 * @brief Handles the .data directive.
 *
 * This function reads the integer values from the input line and stores them in
 * the lineInfo.
 *
 * @param line The input line containing the .data directive.
 * @param lineStruct A pointer to the lineInfo structure where the integer
 *                   values will be stored.
 * @param point A pointer to the current position in the line.
 * @param tableHead A pointer to the table of predefined constants.
 */
void handleDataDirective(char*line, lineInfo *lineStruct, int *point, tableList* tableHead);

/**
 * @brief Extracts an integer value or a predefined constant from a string.
 *
 * This function extracts an integer value or a predefined constant from the
 * given string and stores it in the result parameter.
 *
 * @param str The input string containing the integer value or constant.
 * @param lineStruct A pointer to the lineInfo structure.
 * @param tableHead A pointer to the table of predefined constants.
 * @param result A pointer to an integer where the value will be stored.
 * @return true if the integer value or constant is successfully extracted,
 *         false otherwise.
 */
boolean extractInteger(char* str, lineInfo* lineStruct, tableList* tableHead, int* result);

#endif /* DIRECTIVES_H */