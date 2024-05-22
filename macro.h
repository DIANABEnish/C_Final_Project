#ifndef MACRO_H
#define MACRO_H

/*list of line in a specific macro*/
typedef struct lineList
{
	char *line;						/*a line in the macro */
	struct lineList *nextLine; 	/*pointer for the next line*/
}lineList;

/*List of Macros Each macro has its own list of macros*/
typedef struct macrosList
{
	char *name;						/* name of the macro */
	struct lineList *lineHead; 	/*head of the lines*/
	struct macrosList *nextMacro; 	/*pointer for the next macro*/
}macrosList;

/**
 * @brief Retrieves a macro from the macro list based on its name.
 *
 * @param macroList The head of the macro list.
 * @param name The name of the macro to retrieve.
 * @return The macro structure if found, NULL otherwise.
 */
macrosList* findMacro(macrosList* macroList, char* name);

/**
 * @brief Writes the contents of a macro to a file.
 *
 * @param macro The macro structure to be written.
 * @param file The file pointer to write the macro contents to.
 */
void writeMacro(macrosList *macro, FILE *file);

/**
 * @brief Adds a line to an existing macro.
 *
 * @param macro The macro structure to add the line to.
 * @param line The line to be added.
 * @return 1 if the line was added successfully, 0 otherwise.
 */
boolean appendLineToMacro(macrosList* macro, char* line);

/**
 * @brief Creates a new macro and adds it to the macro list.
 *
 * @param macroList The head of the macro list.
 * @param name The name of the new macro.
 * @return The updated head of the macro list.
 */
macrosList* createMacro(macrosList* macroList, char* name);

/**
 * @brief Frees the memory allocated for the macro list.
 *
 * @param macroList The head of the macro list.
 */
void freeMacroList(macrosList* head);

/**
 * @brief Validates a macro name based on specified rules.
 *
 * @param macro_list The head of the macro list.
 * @param name The macro name to be validated.
 * @return 1 if the macro name is valid, 0 otherwise.
 */
boolean validateMacrosName(macrosList* head,char *name);

#endif