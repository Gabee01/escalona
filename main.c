#include <stdbool.h>
#include "main.h"
#include "graph.h"

void parseInstruction(Graph scheduling, const char *input);

int charToInt(char c);

int main() {

    Graph scheduling = initGraph();

    while (scheduling != NULL){
        scheduling = readScheduling();
        List instructions = scheduling->instructions;

        for (int i = 0; i < instructions->count; i++) {
            if (instructions->data[i][OPERATION] == 'R')
                operationAfter(scheduling, 'W', instructions->data[i][ENTITY],
                               instructions->data[i][TIME]);//W(X) after R(X)
            if (instructions->data[i][OPERATION] == 'W') {
                operationAfter(scheduling, 'R', instructions->data[i][ENTITY],
                               instructions->data[i][TIME]);//R(X) after W(X)
                operationAfter(scheduling, 'W', instructions->data[i][ENTITY],
                               instructions->data[i][TIME]);//W(X) after W(X)
            }
        }

        if (isAciclic(scheduling))
            printf("SS");
        else
            printf("NS");

        free(scheduling);
    }
}

//    Digrafo-Acíclico (n, Adj)
//    1 para  u ← 1  até  n  faça
//    2     cor[u] ← branco
//    3 para  r ← 1  até  n  faça
//    4     se  cor[r] = branco
//    5         então  x ← Terr-Acicl (r)
//    6             se  x = 0  então devolva  0  e pare
//    7 devolva  1
bool isAciclic(Graph pGraph) {
    for (int i = 0; i < pGraph->nodesCount; i++)
        pGraph->nodes[i]->color = WHITE;
    for (int i = 1; i < pGraph->nodesCount; i++)
        if (pGraph->nodes[i]->color == WHITE)
            if (!aciclicTerritory(pGraph->nodes[i]))
                return 0;
    return 1;
}

//Terr-Acicl-R (u)
//1  cor[u] ← cinza
//2  para cada  v  em  Adj[u]  faça
//3     se  cor[v] = cinza
//4         então  devolva  0  e pare
//5     se  cor[v] = branco
//6         então  x ← Terr-Acicl-R (v)
//7             se  x = 0  então  devolva  0  e pare
//8  cor[u] ← preto
//9  devolva   1
bool aciclicTerritory(Node pNode) {

    pNode->color = GRAY;
    for (int i = 0; i < pNode->neighborsCount; i++){
        if (pNode->neighbors[i]->color == GRAY)
            return 0;
        if (pNode->neighbors[i]->color == WHITE)
            if (!aciclicTerritory(pNode->neighbors[i]))
                return 0;
    }
    pNode->color = BLACK;
    return 1;
}

Graph readScheduling() {
    char input[6];
    int lastCommitedTransaction = 0;

    Graph scheduling = initGraph();

    while(fscanf(stdin, "%c %c %c %c\n", &input[TIME], &input[TRANSACTION], &input[OPERATION], &input[ENTITY])){
        if (input[2] == 'C'){
            removeArrayData(scheduling->awaiting, input[TRANSACTION]);
            addListData(scheduling->instructions, input);
            if (lastCommitedTransaction < charToInt(input[TRANSACTION])){
                lastCommitedTransaction = charToInt(input[TRANSACTION]);
            }
            if (scheduling->awaiting->data == NULL)
                return scheduling;
        }
        else{
            parseInstruction(scheduling, input);
        }
    }

    return NULL;
}

int charToInt(char c) {
    return ((int) c - 48);
}

void parseInstruction(Graph scheduling, const char *input) {
    char transaction = input[1];

    Node transaction_node = getNode(scheduling, transaction);

    if (transaction_node == NULL){
        transaction_node = newNode(input[1]);
        addNode(scheduling, transaction_node);
    }
    else{
        addListData(scheduling->instructions, input);
    }
}