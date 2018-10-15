//
// Created by Gabriel Carraro on 9/10/18.
//

#ifndef ESCALONA_REFACT_DS_H
#define ESCALONA_REFACT_DS_H

#include <stdlib.h>
#include <stdio.h>

typedef struct graph *Graph;
typedef struct node *Node;
typedef struct instructionsList *InstructionsList;
typedef struct intArray *IntArray;
typedef struct variable *Variable;
typedef struct varsArray *VarsArray;
typedef struct instruction Instruction;

struct instruction{
    int time;
    int transaction;
    char operation;
    char varName;
    int value;
} instruction;

struct variable{
    char name;
    int value;
} variable;

struct varsArray{
    int count;
    Variable *values;
} varsArray;

struct intArray{
    int *values;
    int count;
} intArray;

struct instructionsList{
  int count;
  Instruction *values;
} instructionsList;

struct node{
    int label;
    int neighborsCount;
    int color;
    Node *neighbors;
} node;

struct graph {
    int nodesCount;
    int edgesCount;
    Node *nodes;
    IntArray transactionsIds;
    IntArray awaiting;
    InstructionsList instructions;
};

#define WHITE 0
#define GRAY 1
#define BLACK 99


Graph initGraph();
Node newNode(int label);
Node getNode(Graph scheduling, int label);
void addNode(Graph pGraph, Node pNode);
void addInt(IntArray array, int data);
void addOrUpdateVariable(VarsArray array, Variable var, int shouldUpdate);
void removeInt(IntArray array, int data);
void removeVariable(VarsArray varsArray, Variable value);
void addInstruction(InstructionsList instructionList, Instruction instruction);
void addEdges(Graph pGraph, int, int);
void newNeighborhood(Node pNode, Node neighbor);

#endif //ESCALONA_REFACT_DS_H
