#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "macro.h"
#include "outputFile.h"
#include "globals.h"


void writeOutputFiles(char *fileName, tableList* tableHead, int* IC, int* DC, int *memoryImage, labelList *externHead, labelList *entryHead){
    char *file_ob_name = appendFileExtension(fileName, ".ob");
    char *file_ent_name = appendFileExtension(fileName, ".ent");
    char *file_ext_name = appendFileExtension(fileName, ".ext");
    
    writeObjFile(file_ob_name, *IC, *DC, memoryImage);

    if(externHead != NULL){
        writeLabelFile(file_ext_name, externHead,  tableHead);
    }

    if(entryHead != NULL){
        writeLabelFile(file_ent_name, entryHead, tableHead);
    }
    /*free the names of the files*/
    free(file_ob_name);
    free(file_ent_name);
    free(file_ext_name);
}


void writeObjFile(char* objectFileName, int IC, int DC,  int* memoryImage) {
    int memoryIndex;
    const int totalMemoryEntries = IC + DC;
    char encodedValue[BASE_4_WORD_SIZE+1]; /* Null-terminated string for encoded value*/
        FILE* objectFile = fopen(objectFileName, "w");
    /* Write the header line with instruction and data counts*/
    fprintf(objectFile, "\t%d   %d\n", IC, DC);

    for (memoryIndex = 0; memoryIndex < totalMemoryEntries; ++memoryIndex) {
        binaryToBase4(memoryImage[memoryIndex], encodedValue);
        fprintf(objectFile, "%04d %s\n",( memoryIndex + BASE_MEMORY_ADRESS), encodedValue);
    }

    fclose(objectFile);
}



void binaryToBase4(int binary, char *encode) {
    int two_bits;
    int i;

    for (i = 0; i < BASE_4_WORD_SIZE; i++) {
        two_bits = (binary >> (i * 2)) & 3;
        encode[BASE_4_WORD_SIZE - 1 - i] = encodeToEncripted(two_bits);
    }

    encode[BASE_4_WORD_SIZE] = '\0';
}

char encodeToEncripted(int two_bits) {
    const char char_map[] = "*#%!";
    if (two_bits >= 0 && two_bits < 4) {
        return char_map[two_bits];
    }
    return '\0';
}

