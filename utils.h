/**
* @file utils.h
* @brief This header file contains utility functions and constant arrays used in the program.
*/

#ifndef UTILS_H
#define UTILS_H

#include "globals.h"


/**
* @brief Array of register names
*/
extern const char *registers[];

/**
* @brief Array of command names
*/
extern const char *commands[];

/**
* @brief Array of reserved words
*/
extern const char *reservedWords[];

/*Creating a new type "boolean" where false = 0 and true = 1*/
typedef enum {
   false ,
   true
} boolean;


/**
* @brief Appends a file extension to a given file name.
*
* @param file The file name
* @param extension The extension to append
* @return A dynamically allocated string containing the file name with the extension appended
*/
char *appendFileExtension(char *file, char *extension);

/**
* @brief Skips over spaces and tabs in a given line.
*
* @param line The line to process
* @param position Pointer to the current position in the line
*/
void skipSpaces(char *line, int *position);

/**
* @brief Checks if a given word is a register name.
*
* @param inputWord The word to check
* @return true if the word is a register name, false otherwise
*/
boolean isRegister(char *inputWord);

/**
* @brief Checks if a given word is a command name.
*
* @param inputWord The word to check
* @return true if the word is a command name, false otherwise
*/
boolean isCommand(char *inputWord);

/**
* @brief Checks if a given word is a reserved word.
*
* @param inputWord The word to check
* @return true if the word is a reserved word, false otherwise
*/
boolean isReservedWord(char *inputWord);

/**
* @brief Checks if a given word starts with a letter.
*
* @param inputWord The word to check
* @return true if the word starts with a letter, false otherwise
*/
boolean isStartingWithLetter(char *inputWord);

/**
* @brief Reads the next word from a given line.
*
* @param line The line to read from
* @param position Pointer to the current position in the line
* @return A dynamically allocated string containing the next word, or NULL if an error occurred
*/
char *readNextWord(char *line, int *position);

/**
* @brief Checks if a given line has remaining content after skipping leading spaces.
*
* @param line The line to check
* @param point Pointer to the current position in the line
* @return true if there is remaining content, false otherwise
*/
boolean hasRemainingContent(char *line, int *point);

/**
* @brief Gets the index of a given command string in the commands array.
*
* @param str The command string
* @return The index of the command in the commands array, or -1 if not found
*/
int getCommandIndex(char *str);

#endif /* UTILS_H */