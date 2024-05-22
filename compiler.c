#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "macro.h"
#include "program_tables.h"
#include "program_structures.h"
#include "preprocessor.h"
#include "first_run.h"
#include "second_run.h"
#include "outputFile.h"

/**
 * @brief The entry point of the assembler program.
 * This function serves as the entry point of the assembler program. It processes
 * the given assembly files, performs the first and second runs of the assembler,
 * and generates the output files containing the assembled code and data.
 * 
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return Returns 1 upon successful execution.
 */

int main (int argc, char *argv[]) {
    int i, IC, DC;

    for (i = 1; i < argc; i++) {
        int memoryImage[MEMORY_CAPACITY + 2] = {0};
        tableList* tableHead = NULL;
        labelList* externHead = NULL;
        labelList* entryHead = NULL;

        if (!preprocessorFileCheck(argv[i])) {
            printf("There is a problem in file: %s\n", argv[i]);
            continue;
        }

        IC = 0; 
        DC = 0;

        if (firstPassAssembly(argv[i], &tableHead, &IC, &DC, memoryImage, &externHead, &entryHead) == true && 
            secondPassAssembly(argv[i], tableHead, &IC, &DC, memoryImage, &externHead, &entryHead) == true) {
            writeOutputFiles(argv[i], tableHead, &IC, &DC, memoryImage, externHead, entryHead);
        }

        freeTableList(tableHead);
        freeLabelList(externHead);
        freeLabelList(entryHead);
    }

    return 1;
}


