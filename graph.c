//
// Created by Gabriel Carraro on 9/10/18.
//

#include "graph.h"

Graph initGraph() {
    Graph g = malloc(sizeof(struct graph));

    g->edgesCount = 0;
    g->nodesCount = 0;

    g->nodes = malloc(sizeof(Node));
    g->transactionsIds = malloc(sizeof(Array));
    g->transactionsIds->data = malloc(sizeof(char *));
    g->transactionsIds->count = 0;

    g->awaiting = malloc(sizeof(Array));
    g->awaiting->data = malloc(sizeof(char *));
    g->awaiting->count = 0;

    g->instructions = malloc(sizeof(List));
    g->instructions->data = malloc(sizeof(char **));
    g->instructions->count = 0;

    g->nodes = NULL;

    return g;
}

Node newNode(const char label) {
    Node node = malloc(sizeof(struct node));
    node->label = label; // Label = transaction id
    node->neighbors = malloc(sizeof(Node));
    node->neighborsCount = 0;
    node->color = WHITE;
    node->log = malloc(sizeof(List));

    return node;
}

void addNode(Graph pGraph, Node pNode){
    pGraph->nodes = realloc(pGraph->nodes, (sizeof(struct node) * (pGraph->nodesCount + 1)));

    pGraph->nodes[pGraph->nodesCount] = malloc(sizeof(struct node));
    pGraph->nodes[pGraph->nodesCount]->neighbors = malloc(sizeof(Node));
    pGraph->nodes[pGraph->nodesCount]->neighborsCount = 0;
    pGraph->nodes[pGraph->nodesCount]->color = pNode->color;
    pGraph->nodes[pGraph->nodesCount]->label = pNode->label;

    addArrayData(pGraph->transactionsIds, pNode->label);
    addArrayData(pGraph->awaiting, pNode->label);

    pGraph->nodesCount++;
}

Node getNode(Graph scheduling, char label) {
    for (int i = 0; i < scheduling->nodesCount; i++)
        if (scheduling->transactionsIds->data[i] == label)
            return scheduling->nodes[i];

    return NULL;
}

void newNeighborhood(Node pNode, Node neighbor) {
    pNode->neighbors = (Node *)realloc(pNode->neighbors, (sizeof(Node) * pNode->neighborsCount + 1));

    pNode->neighbors[pNode->neighborsCount] = neighbor;
    pNode->neighborsCount = pNode->neighborsCount + 1;
}

void addEdges(Graph pGraph, char src, char dst) {
    Node nodeSrc = getNode(pGraph, src);
    Node nodeDst = getNode(pGraph, dst);

    newNeighborhood(nodeSrc, nodeDst);
    pGraph->edgesCount++;
}

void addArrayData(Array array, char data) {
    //if have data, see if already exists
    if (array->count > 0)
        for (int i = 0; i < array->count; i++)
            if (array->data[i] == data)
                return;

    array->data = (char *) realloc(array->data, (sizeof(char) * (array->count + 1)));
    array->data[array->count] = data;
    array->count++;
}

void addListData(List list, const char data[4 + MAX_VALUE_SIZE]) {
    list->data = (char **) realloc(list->data, (sizeof(char *) * list->count + 1));

    list->data[list->count] = malloc(sizeof(char[4 + MAX_VALUE_SIZE]));

    list->data[list->count][TIME] = data[TIME];
    list->data[list->count][TRANSACTION] = data[TRANSACTION];
    list->data[list->count][OPERATION] = data[OPERATION];
    list->data[list->count][ENTITY] = data[ENTITY];
    list->data[list->count][VALUE] = data[VALUE];

    list->count++;
}

void removeArrayData(Array array, char data) {
    for (int i = 0; i < array->count; i++){
        if (array->data[i] == data){
            array->data[i] = array->data[array->count - 1];
            array->count--;
            if (array->count == 0)
                array->data = NULL;
            else
                array->data = realloc(array->data, (sizeof(char) * array->count));
        }
    }
}