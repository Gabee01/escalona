//
// Created by Gabriel Carraro on 9/10/18.
//

#ifndef ESCALONA_REFACT_GRAPH_H
#define ESCALONA_REFACT_GRAPH_H

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
    Array neighbors;
} node;

struct graph {
    int nodesCount;
    int edgesCount;
    Node *nodes;
    Array labels;
    Array awaiting;
    List instructions;
};

#endif //ESCALONA_REFACT_GRAPH_H
