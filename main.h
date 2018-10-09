//
// Created by Gabriel Carraro on 9/10/18.
//

#include <ecpglib.h>
#include "graph.h"

#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H

Graph readScheduling();
void parseInstruction(Graph scheduling, const char *input);
//adds an edge every time it finds a last operation on the same entity as the first operation
void checkOperationsAfter(Graph scheduling, char findOperation, char entity, char time, char i);
bool isAciclic(Graph pGraph);
bool aciclicTerritory(Node pNode);

bool hasEquivalent(Graph schedulingd);
bool compareByVision(List originalScheduling, List serialScheduling);
int findFirstWrite(List instructions, char entity);
int findLastWrite(List instructions, char entity);

#endif
