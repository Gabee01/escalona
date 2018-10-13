//
// Created by Gabriel Carraro on 9/10/18.
//

#include <ecpglib.h>
#include "graph.h"

#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H

Graph readScheduling();
void parseInstruction(Graph scheduling, Instruction input);
//adds an edge every time it finds a last operation on the same entity as the first operation
void checkOperationsAfter(Graph scheduling, char findOperation, int transaction, char entity, int time);
bool isAciclic(Graph pGraph);
bool aciclicTerritory(Node pNode);

bool hasEquivalent(Graph schedulingd);
bool compareByVision(InstructionList originalScheduling, InstructionList serialScheduling);
int findFirstWrite(InstructionList instructions, Variable entity);
int findLastWrite(InstructionList instructions, Variable entity);


//T2
void printInstructions(InstructionList pList);
InstructionList serializeInstructions(Graph scheduling);
void logTransactions(Graph pGraph);
void updateVariables(Graph pGraph);
Log initLog();
void addToLogs(Graph scheduling, Instruction instruction);
Log getVarValue(Log *pLog, int, char i);

#endif //PROJECT_MAIN_H
