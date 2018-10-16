//
// Created by Gabriel Carraro on 9/10/18.
//
#include "dataStructures.h"

Graph initGraph() {
    Graph g = malloc(sizeof(struct graph));

    g->edgesCount = 0;
    g->nodesCount = 0;

//    g->nodes = malloc(sizeof(Node));
    g->transactionsIds = malloc(sizeof(IntArray));
    g->transactionsIds->values = malloc(sizeof(int *));
    g->transactionsIds->count = 0;

    g->awaiting = malloc(sizeof(IntArray));
    g->awaiting->values = malloc(sizeof(int *));
    g->awaiting->count = 0;

    g->instructions = malloc(sizeof(InstructionsList));
    g->instructions->values = malloc(sizeof(char **));
    g->instructions->count = 0;

    g->nodes = NULL;
    return g;
}

Node newNode(int label) {
    Node node = malloc(sizeof(struct node));
    node->label = label; // Label = transaction id
    node->neighbors = malloc(sizeof(Node));
    node->neighborsCount = 0;
    node->color = WHITE;

    return node;
}

void addNode(Graph pGraph, Node pNode){
    pGraph->nodes = realloc(pGraph->nodes, (sizeof(struct node) * (pGraph->nodesCount + 1)));

    pGraph->nodes[pGraph->nodesCount] = malloc(sizeof(struct node));
    pGraph->nodes[pGraph->nodesCount]->neighbors = malloc(sizeof(Node));
    pGraph->nodes[pGraph->nodesCount]->neighborsCount = 0;
    pGraph->nodes[pGraph->nodesCount]->color = pNode->color;
    pGraph->nodes[pGraph->nodesCount]->label = pNode->label;

    addInt(pGraph->transactionsIds, pNode->label);
    addInt(pGraph->awaiting, pNode->label);

    pGraph->nodesCount++;
}

Node getNode(Graph scheduling, int label) {
    for (int i = 0; i < scheduling->nodesCount; i++)
        if (scheduling->transactionsIds->values[i] == label)
            return scheduling->nodes[i];

    return NULL;
}

void newNeighborhood(Node pNode, Node neighbor) {
    pNode->neighbors = (Node *)realloc(pNode->neighbors, (sizeof(Node) * (pNode->neighborsCount + 1)));

    pNode->neighbors[pNode->neighborsCount] = neighbor;
    pNode->neighborsCount = pNode->neighborsCount + 1;
}

void addEdges(Graph pGraph, int src, int dst) {
    Node nodeSrc = getNode(pGraph, src);
    Node nodeDst = getNode(pGraph, dst);

    newNeighborhood(nodeSrc, nodeDst);
    pGraph->edgesCount++;
}

void addInt(IntArray array, int data) {
    //if have values, see if already exists
    if (array->count > 0)
        for (int i = 0; i < array->count; i++)
            if (array->values[i] == data)
                return;

    array->values = (int *) realloc(array->values, (sizeof(int) * (array->count + 1)));
    array->values[array->count] = data;
    array->count++;
}

void addOrUpdateVariable(VarsArray array, Variable var, int shouldUpdate) {
    //if have values, see if already exists
    if (array->count > 0 && shouldUpdate){
        for (int i = 0; i < array->count; i++){
            if (array->values[i] == var) {
                array->values[i]->value = var->value;
                return;
            }
        }
    }
    if (array->count == 0){
//        printf("\narray count: %d", array->count);
        array->values = malloc(sizeof(struct variable *));
        array->values[array->count] = malloc(sizeof(struct variable));
    }
    else{
        array->count++;
        array->values[array->count] = realloc(array->values[array->count], (sizeof(struct variable) * (array->count)));
    }

    array->values[array->count]->value = var->value;
    array->values[array->count]->name = var->name;
}


void addInstruction(InstructionsList instructionList, Instruction input) {
    instructionList->values = (Instruction *) realloc(instructionList->values, (sizeof(struct instruction) * (instructionList->count + 1)));


    instructionList->values[instructionList->count].time = input.time;
    instructionList->values[instructionList->count].transaction = input.transaction;
    instructionList->values[instructionList->count].operation = input.operation;
    instructionList->values[instructionList->count].varName = input.varName;
    instructionList->values[instructionList->count].value = input.value;
    instructionList->count++;
}

void removeVariable(VarsArray varsArray, Variable value) {
    for (int i = 0; i < varsArray->count; i++){
        if (varsArray->values[i] == value){
            varsArray->values[i] = varsArray->values[varsArray->count - 1];
            varsArray->count--;
            if (varsArray->count == 0)
                varsArray->values = NULL;
            else
                varsArray->values = realloc(varsArray->values, (sizeof(char) * varsArray->count));
        }
    }
}

void removeInt(IntArray array, int value) {
    for (int i = 0; i < array->count; i++){
        if (array->values[i] == value){
            array->values[i] = array->values[array->count - 1];
            array->count--;
            if (array->count == 0)
                array->values = NULL;
            else
                array->values = realloc(array->values, (sizeof(char) * array->count));
        }
    }
}