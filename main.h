//
// Created by Gabriel Carraro on 9/10/18.
//

#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

#ifndef PROJECT_MAIN_H
#define PROJECT_MAIN_H

#define MAX_TRANSACTIONS 5
#define MAX_SCHEDULINGS 5

#define COMMAND_SIZE 4
#define TIME_INDEX 0
#define TRANSACTION_INDEX 1
#define OP_INDEX 2
#define ATTR_INDEX 3

void newTransactionInstrucion(char transaction, char **instruction);
Graph parseInput();
Graph readScheduling();


void addEdges();
void addToCommandsList();
void printCommandsList(char **matrix, int size);

void findCicle();
#endif PROJECT_MAIN_H
