#ifndef PROGRAM_TABLES_H
#define PROGRAM_TABLES_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "utils.h"
#include "globals.h"


/*table for labels and constant*/
typedef struct tableList{
    enum{
        constant,
        code_label,     /*before command*/
        data_label,     /*before string and array*/
        extern_labal   /*can be used cannot be defined*/
    }tableType;
	char *name;        /* The name of the table element */
    int value;			/* The value associated with the table element */			
	struct tableList *nextElement; 	/*pointer for the next constant*/
}tableList;

/**
 * @struct labelList
 * @brief Structure to represent an element in the label list.
 */
typedef struct labelList {
    char *name;                   /**< The name of the label */
    int address;                  /**< The address of the label */
    struct labelList *next;       /**< Pointer to the next element in the label list */
} labelList;

/**
 * @brief Adds a new member to the table list.
 * 
 * @param type The type of the table member.
 * @param name The name of the table member.
 * @param value The value associated with the table member.
 * @param tableHead Pointer to the head of the table list.
 */
void addTableMammber(int type, char *name, int value, tableList **tableHead);

/**
 * @brief Checks if a member is in the table list.
 * 
 * @param name The name of the table member to check.
 * @param tableHead Pointer to the head of the table list.
 * @return true if the member is in the table, false otherwise.
 */
boolean isMemmberInTable(char *name, tableList *tableHead);

/**
 * @brief Gets the type of a label from the table list.
 * 
 * @param name The name of the label.
 * @param tableHead Pointer to the head of the table list.
 * @return The type of the label, or -1 if not found.
 */
int getLabelType(char *name, tableList *tableHead);

/**
 * @brief Gets the constant number associated with a name in the table list.
 * 
 * @param name The name to look up.
 * @param tableHead Pointer to the head of the table list.
 * @param num Pointer to an integer to store the value.
 * @return true if the name is found and the value is set, false otherwise.
 */
boolean getConstantNum(char *name, tableList *tableHead, int *num);

/**
 * @brief Frees the memory allocated for the table list.
 * 
 * @param tableHead Pointer to the head of the table list.
 */
void freeTableList(tableList *tableHead);

/**
 * @brief Adds a new label to the label list.
 * 
 * @param labelHead Pointer to the head of the label list.
 * @param name The name of the label.
 * @param value The address of the label.
 */
void addLabel(labelList **labelHead, char *name, int value);

/**
 * @brief Checks if a label is in the label list.
 * 
 * @param name The name of the label to check.
 * @param labelHead Pointer to the head of the label list.
 * @return true if the label is in the list, false otherwise.
 */
boolean isLabelInList(char *name, labelList *labelHead);

/**
 * @brief Updates the values of data labels by adding the instruction counter (IC).
 * 
 * @param label_head Pointer to the head of the table list.
 * @param IC The instruction counter to add to data labels.
 */
void updateLabelValues(tableList *label_head, int IC);

/**
 * @brief Frees the memory allocated for the label list.
 * 
 * @param tableHead Pointer to the head of the label list.
 */
void freeLabelList(labelList *tableHead);

/**
 * @brief Writes the label list to a file.
 * 
 * @param labelFileName The name of the file to write to.
 * @param labelHead Pointer to the head of the label list.
 * @param tableHead Pointer to the head of the table list.
 */
void writeLabelFile(const char* labelFileName, labelList *labelHead, tableList *tableHead);

#endif /* PROGRAM_TABLES_H */
