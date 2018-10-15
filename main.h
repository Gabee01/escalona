//
// Created by Gabriel Carraro on 9/10/18.
//

#include "graph.h"

#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H

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
void logTransactions(Graph pGraph, LogsList logs);
Variable newVariable(char entity, int value);
void generateLog(LogsList logs, Instruction instruction);
void addLog(LogsList logs, Log log);
void printLogs(LogsList logs);
void addStartLog(LogsList logs, Instruction instruction);
int getLogTime(LogsList logs, Instruction instruction);
void printInstructions(InstructionsList pList);
//Log getVarValue(Log *pLog, int, char i);
//int getLastCommitedValue(Log *pLog, int count, char entity);
//int hasLog(Graph pGraph, int transaction);

#endif //PROJECT_MAIN_H
