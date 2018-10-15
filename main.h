//
// Created by Gabriel Carraro on 9/10/18.
//

#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H

#include "logs.h"
Graph readScheduling();
void parseInstruction(Graph scheduling, Instruction input);
//adds an edge every time it finds a last operation on the same varName as the first operation
void checkOperationsAfter(Graph scheduling, char findOperation, int transaction, char entity, int time);
int isAciclic(Graph pGraph);
int aciclicTerritory(Node pNode);

int hasEquivalent(Graph schedulingd);
int compareByVision(InstructionsList originalScheduling, InstructionsList serialScheduling);
int findFirstWrite(InstructionsList instructions, Variable entity);
int findLastWrite(InstructionsList instructions, Variable entity);


//T2
InstructionsList serializeInstructions(Graph scheduling);
Variable newVariable(char entity, int value);
void printInstructions(InstructionsList pList);


#endif //PROJECT_MAIN_H
