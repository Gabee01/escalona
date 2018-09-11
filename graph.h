//
// Created by Gabriel Carraro on 9/10/18.
//

#ifndef ESCALONA_REFACT_GRAPH_H
#define ESCALONA_REFACT_GRAPH_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct graph *Graph;
typedef struct node *Node;
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
    Array labels;
    Array awaiting;
    List instructions;
};

Graph initGraph();
Node newNode(char label);
Node getNode(Graph scheduling, char label);
void addNode(Graph pGraph, Node pNode);
void addArrayData(Array array, char data);
void removeArrayData(Array array, char data);
void addListData(List list, const char data[4]);
void addEdges(Graph pGraph, char, char);
void addNeighbor(Node pNode, Node neighbor);
//adds an edge every time it finds a last operation on the same entity as the first operation
void checkOperationsAfter(Graph scheduling, char findOperation, char entity, char time, char i);

#define WHITE 0
#define GRAY 1
#define BLACK 99

bool isAciclic(Graph pGraph);
bool aciclicTerritory(Node pNode);

#define TIME 0
#define TRANSACTION 1
#define OPERATION 2
#define ENTITY 3


#endif //ESCALONA_REFACT_GRAPH_H
