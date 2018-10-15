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
typedef struct instructionsList *InstructionsList;
typedef struct logsList *LogsList;
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

struct logsList{
    int count;
    Log *values;
} logsList;

struct tLog{
    int time;
    int action;
    int transaction;
    char varName;
    int initialValue;
    int newValue;
} tLog;

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

#define ACTION_START 77
#define ACTION_CHANGEVAR 88
#define ACTION_COMMIT 99
#define ACTION_ABORT 11


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

#endif //ESCALONA_REFACT_GRAPH_H
