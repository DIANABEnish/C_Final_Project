#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "utils.h"
#include "program_tables.h"
#include "globals.h"

void addTableMammber(int type, char *name, int value, tableList **tableHead){
    tableList *pointer = *tableHead;
    
    tableList *newList = (tableList*)malloc(sizeof(tableList));
    if(newList == NULL){
        printf("Memory allocation failed\n");
        exit(1);
    }

    newList->name = (char*)malloc(strlen(name)+1);
    if(newList->name == NULL){
        printf("Memory allocation failed\n");
        free(newList);
        exit(1);
    }
    /*apply the values to new_list*/
    newList->tableType = type;
    strcpy(newList->name, name);
    newList->value = value;
    newList->nextElement = NULL;

    if(pointer == NULL){
        *tableHead = newList;
        return;
    }
    /*add new_list to the end of the list*/
    while(pointer->nextElement != NULL){
        pointer = pointer->nextElement;
    }

    pointer->nextElement = newList;    
}

boolean isMemmberInTable(char *name, tableList *tableHead){
    tableList *pointer = tableHead;
    while (pointer != NULL){
        if(strcmp(pointer->name, name) == 0){
            return true;
        }
        pointer = pointer->nextElement;
    }
    return false;
}

int getLabelType(char *name, tableList *tableHead){
    tableList *pointer = tableHead;
    while (pointer != NULL){
        if(strcmp(pointer->name, name) == 0){
            return pointer->tableType;
        }
        pointer = pointer->nextElement;
    }
    return -1;
}

boolean getConstantNum(char *name, tableList *tableHead, int *num){
    tableList *point = tableHead;
    if(point == NULL){
        return false;
    }
    while(point != NULL){
        if(strcmp(name,point->name)==0){
            *num = point->value;
			return true;
		}
        point = point->nextElement;
    }
    return false;
}

void freeTableList(tableList *tableHead){
    tableList *point = tableHead;
    tableList *point_next;

    while(point != NULL){
        point_next = point->nextElement;
        free(point->name);
        free(point);
        point = point_next;
    }
}

void addLabel(labelList **labelHead, char *name, int value){
    labelList *pointer = *labelHead;
    labelList *new = (labelList*)malloc(sizeof(labelList));
    
    if(new == NULL){
        printf("Memory allocation failed\n");
        exit(1);
    }
    /*apply the values to new*/
    new->address = value;
    new->next = NULL;
    new->name = (char*)malloc(strlen(name)+1);
    if(new->name == NULL){
        printf("Memory allocation failed\n");
        free(new);
        exit(1); 
    }
    strcpy(new->name,name);

    if(*labelHead == NULL){
        *labelHead = new;
        return;
    }
    /*add new to the end of the list*/
    while(pointer->next != NULL){
        pointer = pointer->next;
    }

    pointer->next = new;

}

boolean isLabelInList(char *name, labelList *labelHead){
    labelList *pointer = labelHead;
    while (pointer != NULL){
        if(strcmp(pointer->name, name) == 0){
            return true;
        }
        pointer = pointer->next;
    }
    return false;
}

void updateLabelValues(tableList *labelHead, int IC){
    tableList *pointer = labelHead;
    if (labelHead == NULL){
        return;
    }
    /*run through the list and add the IC to data label this will fix their value*/
    while(pointer != NULL){
        if(pointer->tableType == data_label){
            pointer->value += IC;
        }
        pointer = pointer->nextElement;
    }
}

void freeLabelList(labelList *tableHead){
    labelList *point = tableHead;
    labelList *pointNext;

    while(point != NULL){
        pointNext = point->next;
        free(point->name);
        free(point);
        point = pointNext;
    }
}


void writeLabelFile(const char* labelFileName, labelList *labelHead, tableList *tableHead) {
    FILE* labelFile = fopen(labelFileName, "w");
    if (labelFile != NULL) {
        labelList *point = labelHead;
        while (point != NULL) {
            int label_type = getLabelType(point->name, tableHead);
            if (label_type == EXTERN_LABEL) {
                fprintf(labelFile, "%s %04d\n", point->name, point->address + BASE_MEMORY_ADRESS);
            } else {
                fprintf(labelFile, "%s %04d\n", point->name, (point->address + BASE_MEMORY_ADRESS));
            }
            point = point->next;
        }
        fclose(labelFile);
    }
}

