#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "utils.h"
#include "macro.h"
#include "globals.h"

macrosList* findMacro(macrosList* macro_list, char* name){
	macrosList *curreentNode = macro_list;
	if(curreentNode==NULL){
		return NULL;
	}
	while (curreentNode!=NULL){
		if(strcmp(name,curreentNode->name)==0){
			return curreentNode;
		}
		curreentNode = curreentNode->nextMacro;
	}
	return NULL;
}

void writeMacro(macrosList *macro, FILE *file){
	lineList *line = macro->lineHead;
	while (line != NULL){
		fprintf(file,"%s", line->line);
		line = line->nextLine;
	}
}


boolean appendLineToMacro(macrosList* macro, char* line){
	lineList *head = macro->lineHead;
	lineList *currentLine = head;
	lineList *newLine = (lineList *)malloc(sizeof(lineList));
	if (newLine==NULL){
		printf("Failed to allocate memory for macro line.\n");
		return false;
	}

	newLine->nextLine = NULL;
	newLine->line = malloc(strlen(line) + 1);
	if (newLine->line==NULL){
		printf("failed to allocate mamory for macro.\n");
		free(newLine);
		return false;
	}
	strcpy(newLine->line,line);
	if (head==NULL){
		macro->lineHead = newLine;
	}else{
		while (currentLine->nextLine!=NULL){
			currentLine = currentLine->nextLine;
		}
		currentLine->nextLine = newLine;
	}
	return true;
}

macrosList* createMacro(macrosList* macroList, char* name){
	macrosList *newMacro = (macrosList *)malloc(sizeof(macrosList));
	if (newMacro == NULL){
		printf("Failed to allocate memory for macro.\n");
		return NULL;
	}
	
	newMacro->name = malloc(strlen(name) + 1);
	if (newMacro->name==NULL){
		printf("Failed to allocate memory for macro name.\n");
		free(newMacro);
		return NULL;
	}

	strcpy(newMacro->name,name);
	newMacro->lineHead = NULL;
	newMacro->nextMacro = macroList;

	return newMacro;
}

boolean validateMacrosName(macrosList* head,char *name){
	if(name==NULL || findMacro(head , name) != NULL){
		return false;
	}
	/*is_register(word)==false, is_comendline(word) == false, is_start_letter(word) = true than return true, strlen(word)<=MAX_MACRO_NAME_SIZE = true */
	return !(isRegister(name) || isCommand(name) || !isStartingWithLetter(name) || !(strlen(name)<=MAX_MACRO_NAME_LENGTH));
}

void freeMacroList(macrosList* head){
	macrosList *currentMacro = head;
	macrosList *nextMacro;
	lineList *currentLine;
	lineList *nextLine;

	if(head == NULL){
		return;
	}

	
	while (currentMacro != NULL){
		nextMacro = currentMacro->nextMacro;
		currentLine = currentMacro->lineHead;

		
		while (currentLine != NULL)
		{
			nextLine = currentLine->nextLine;
			free(currentLine->line);
			free(currentLine);
			currentLine = nextLine;
		}
		free(currentMacro->name);
		free(currentMacro);
		currentMacro = nextMacro;		
	}
}

