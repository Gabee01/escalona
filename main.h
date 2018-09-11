//
// Created by Gabriel Carraro on 9/10/18.
//

#include "graph.h"

#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H

Graph readScheduling();
void parseInstruction(Graph scheduling, const char *input);
bool isEqual(Graph schedulingd);
void compareByVision(List originalScheduling, List serialScheduling);
#endif PROJECT_MAIN_H
