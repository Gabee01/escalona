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
typedef struct list *List;
typedef struct array *Array;

struct array{
    char *data;
    int count;
} array;

struct list{
  char **data;
  int count;
} list;

struct tLog{
    char varName;
    int initialValue;
    int newValue;
} tLog;

struct node{
    char label;
    Node *neighbors;
    int neighborsCount;
    int color;
} node;

struct graph {
    int nodesCount;
    int edgesCount;
    Node *nodes;
    Array transactionsIds;
    Array awaiting;
    List instructions;
    Log *logs;
    int logsCount;
};

#define WHITE 0
#define GRAY 1
#define BLACK 99
#define TIME 0
#define TRANSACTION 1
#define OPERATION 2
#define ENTITY 3
#define VALUE 4
#define DATASIZE (4 + 12)

Graph initGraph();
Node newNode(char label);
Node getNode(Graph scheduling, char label);
void addNode(Graph pGraph, Node pNode);
void addArrayData(Array array, char data);
void removeArrayData(Array array, char data);
void addListData(List list, const char data[DATASIZE]);
void addEdges(Graph pGraph, char, char);
void newNeighborhood(Node pNode, Node neighbor);

#endif //ESCALONA_REFACT_GRAPH_H
