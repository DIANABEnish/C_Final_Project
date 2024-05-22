#ifndef OUTPUT_FILE_H
#define OUTPUT_FILE_H

#include "program_tables.h"
#include "program_structures.h"

/**
 * @brief Writes the output files for the assembler program.
 * 
 * This function writes the output files containing the object code, 
 * the external labels (if any), and the entry labels (if any).
 * 
 * @param fileName The base name of the output files.
 * @param tableHead The head of the symbol table.
 * @param IC The instruction counter.
 * @param DC The data counter.
 * @param memoryImage The memory image containing the assembled code and data.
 * @param externHead The head of the list of external labels.
 * @param entryHead The head of the list of entry labels.
 */
void writeOutputFiles(char *fileName, tableList* tableHead, int* IC, int* DC, int *memoryImage, labelList *externHead, labelList *entryHead);

/**
 * @brief Writes the object file (.ob) containing the assembled code and data.
 * 
 * This function writes the object file in the format required by the assembler program.
 * 
 * @param objectFileName The name of the object file.
 * @param IC The instruction counter.
 * @param DC The data counter.
 * @param memoryImage The memory image containing the assembled code and data.
 */
void writeObjFile(char* objectFileName, int IC, int DC,  int* memoryImage);

/**
 * @brief Converts binary representation to base-4 representation.
 * 
 * This function converts the given binary number to its base-4 representation.
 * 
 * @param binary The binary number to be converted.
 * @param encode The character array to store the base-4 representation.
 */
void binaryToBase4(int binary, char *encode);

/**
 * @brief Encodes two bits to a corresponding character in the encripted map.
 * 
 * This function maps two bits to a corresponding character in the encripted map.
 * 
 * @param two_bits The two bits to be encoded.
 * @return The corresponding character in the encripted map.
 */
char encodeToEncripted(int two_bits);

#endif /* OUTPUTFILE_H */
