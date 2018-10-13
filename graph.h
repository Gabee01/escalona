//
// Created by Gabriel Carraro on 9/10/18.
//

#ifndef ESCALONA_REFACT_GRAPH_H
#define ESCALONA_REFACT_GRAPH_H

#include <stdlib.h>
#include <stdio.h>

typedef struct graph *Graph;
typedef struct node *Node;
typedef struct tLog *Log;
typedef struct instructionsList *InstructionList;
typedef struct intArray *IntArray;
typedef struct variable *Variable;
typedef struct varsArray *VarsArray;
typedef struct instruction Instruction;

struct instruction{
    int time;
    int transaction;
    char operation;
    char entity;
    int value;
} instruction;

struct variable{
    char name;
    int value;
} variable;

struct varsArray{
    Variable *values;
    int count;
} varsArray;

struct intArray{
    int *values;
    int count;
} intArray;

struct instructionsList{
  Instruction *values;
  int count;
} instructionsList;

struct tLog{
    char varName;
    int initialValue;
    int newValue;
} tLog;

struct node{
    int label;
    Node *neighbors;
    int neighborsCount;
    int color;
} node;

struct graph {
    int nodesCount;
    int edgesCount;
    Node *nodes;
    IntArray transactionsIds;
    IntArray awaiting;
    InstructionList instructions;
    Log *logs;
    int logsCount;
};

#define WHITE 0
#define GRAY 1
#define BLACK 99

Graph initGraph();
Node newNode(int label);
Node getNode(Graph scheduling, int label);
void addNode(Graph pGraph, Node pNode);
void addInt(IntArray array, int data);
void addVariable(VarsArray array, Variable data);
void removeInt(IntArray array, int data);
void removeVariable(VarsArray varsArray, Variable value);
void addInstruction(InstructionList instructionList, Instruction instruction);
void addEdges(Graph pGraph, int, int);
void newNeighborhood(Node pNode, Node neighbor);

#endif //ESCALONA_REFACT_GRAPH_H
