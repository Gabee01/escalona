//
// Created by Gabriel Carraro on 9/11/18.
//

#include "graph.h"

Graph initGraph() {
    Graph g = malloc(sizeof(Graph));

    g->nodes = malloc(sizeof(Node *));
    g->labels = malloc(sizeof(Array));
    g->labels->data = malloc(sizeof(char *));
    g->labels->count = 0;
    g->awaiting = malloc(sizeof(Array));
    g->awaiting->data = malloc(sizeof(char *));
    g->awaiting->count = 0;
    g->instructions = malloc(sizeof(List));
    g->instructions->data = malloc(sizeof(char **));
    g->instructions->count = 0;
    g->edgesCount = 0;
    g->nodesCount = 0;

    return g;
}

Node newNode(const char label) {

    Node node = malloc(sizeof(Node));
    node->label = label; // Label = transaction name
    node->neighborsCount = 0;

    return node;
}


void addNode(Graph pGraph, Node pNode){

    if (pGraph->nodesCount == 0)
        pGraph->nodes = malloc(sizeof(Node *));

    pGraph->nodes[pGraph->nodesCount] = malloc(sizeof(Node));

    pGraph->nodes[pGraph->nodesCount] = pNode;
    addArrayData(pGraph->labels, pNode->label);
    addArrayData(pGraph->awaiting, pNode->label);

    pGraph->nodesCount++;
}

void addArrayData(Array array, char data) {
    realloc(array->data, (sizeof(char) * array->count + 1));
    if (array->count > 0)
        for (int i = 0; i < array->count; i++)
            if (array->data[i] == data)
                return;

    array->data[array->count] = data;
    array->count++;
}

void addListData(List list, const char data[4]) {
    list->data[list->count] = malloc(sizeof(char) * 4);

    list->data[list->count][TIME] = data[TIME];
    list->data[list->count][TRANSACTION] = data[TRANSACTION];
    list->data[list->count][OPERATION] = data[OPERATION];
    list->data[list->count][ENTITY] = data[ENTITY];

    list->count++;
}

Node getNode(Graph scheduling, char label) {

    for (int i = 0; i < scheduling->nodesCount; i++) {
        if (scheduling->labels->data[i] == label) {
            return scheduling->nodes[i];
        }
    }

    return NULL;
}

void removeArrayData(Array array, char data) {

    for (int i = 0; i < array->count; i++){
        if (array->data[i] == data){
            array->data[i] = array->data[array->count - 1];
            array->count--;
            if (array->count == 0)
                array->data = NULL;
            else
                realloc(array->data, sizeof(char) * array->count);
        }
    }
}

void addEdges(Graph pGraph, char src, char dst) {

    Node nodeSrc = getNode(pGraph, src);
    Node nodeDst = getNode(pGraph, dst);

    addNeighbor(nodeSrc, nodeDst);
    pGraph->edgesCount++;
}

void addNeighbor(Node pNode, Node neighbor) {

    if (pNode->neighborsCount == 0)
        pNode->neighbors = malloc(sizeof(Node *));
    else
        realloc(pNode->neighbors, (sizeof(Node *) * pNode->neighborsCount + 1));

    pNode->neighbors[pNode->neighborsCount] = neighbor;
    pNode->neighborsCount++;
}

void operationAfter(Graph scheduling, char findOperation, char entity, int time) {

    List instructions = scheduling->instructions;

//    for(int i = 0; i < instructions->count; i++)
//        if (instructions->data[i][OPERATION] == firstOperation) //WX em Ti
            for (int j = time + 1; j < instructions->count; j++)
                if (instructions->data[j][OPERATION] == findOperation && instructions->data[j][ENTITY] == entity) //RX em Tj
                    addEdges(scheduling, instructions->data[time][TRANSACTION], instructions->data[j][TRANSACTION]);

}
