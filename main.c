#include <stdbool.h>
#include "main.h"
#include "graph.h"

bool checkOperationsBefore(List instructions, char currentTime, int operation, int entity, char i);

int findFirstWrite(List instructions, char entity);
int findLastWrite(List instructions, char entity);

int main() {
    int schedulingCount = 0;

    Graph scheduling;

    while (schedulingCount == 0 || scheduling != NULL){
        schedulingCount ++;

        scheduling = readScheduling();

        if (scheduling != NULL){
            List instructions = scheduling->instructions;

            for (int i = 0; i < instructions->count; i++) {
                if (instructions->data[i][OPERATION] == 'R')
                    checkOperationsAfter(scheduling, 'W', instructions->data[i][TRANSACTION], instructions->data[i][ENTITY],
                                         instructions->data[i][TIME]);//W(X) after R(X)
                if (instructions->data[i][OPERATION] == 'W') {
                    checkOperationsAfter(scheduling, 'R', instructions->data[i][TRANSACTION], instructions->data[i][ENTITY],
                                         instructions->data[i][TIME]);//R(X) after W(X)
                    checkOperationsAfter(scheduling, 'W', instructions->data[i][TRANSACTION], instructions->data[i][ENTITY],
                                         instructions->data[i][TIME]);//W(X) after W(X)
                }
            }

            printf("%d ", schedulingCount);
            for (int i = 0; i < scheduling->labels->count; i++)
                if (i == scheduling->labels->count - 1)
                    printf("%u ", scheduling->labels->data[i]);
                else
                    printf("%u,", scheduling->labels->data[i]);

            if (isAciclic(scheduling))
                printf("SS ");
            else
                printf("NS ");

            if (isEqual(scheduling))
                printf("SV\n");
            else
                printf("NV\n");

            free(scheduling);
        }
    }
}

bool compareByVision(List originalScheduling, List serialScheduling) {

    Array entities = malloc(sizeof(Array));
    entities->data = malloc(sizeof(char *));

    //Tests writes order
    for(int i = 0; i < serialScheduling->count; i++){
        if (serialScheduling->data[i][OPERATION] == 'C')
            continue;
        addArrayData(entities, serialScheduling->data[i][ENTITY]);

        if (serialScheduling->data[i][OPERATION] == 'R'){
            char currentSerialEntity = serialScheduling->data[i][ENTITY];

            char currentOriginalEntity = originalScheduling->data[i][ENTITY];

            int timeFirstWriteSerial = findFirstWrite(serialScheduling, currentSerialEntity);
            int timeFirstWriteOriginal = findFirstWrite(originalScheduling, currentOriginalEntity);

            if ((timeFirstWriteSerial > 0 && timeFirstWriteOriginal > 0)
                && originalScheduling->data[timeFirstWriteOriginal][TRANSACTION] != serialScheduling->data[timeFirstWriteSerial][TRANSACTION])
                return 0;
        }
    }

    // Test last writes for each entity
    while (entities->count > 0){
        char currentEntity = entities->data[0];
        int timeLastWriteSerial = findLastWrite(serialScheduling, currentEntity);
        int timeLastWriteOriginal = findLastWrite(originalScheduling, currentEntity);

        if ((timeLastWriteOriginal > 0 && timeLastWriteSerial > 0) // if there is a write
            && serialScheduling->data[timeLastWriteSerial][TRANSACTION] != originalScheduling->data[timeLastWriteOriginal][TRANSACTION]) //and the first writes arent from the same transactions
            return 0;//

        removeArrayData(entities, currentEntity);
    }

    return 1;
}

int findFirstWrite(List instructions, char entity) {

    for (int i = 0; i < instructions->count; i++)
        if (instructions->data[i][ENTITY] == entity && instructions->data[i][OPERATION] == 'W')
            return i;
    return -1;
}

int findLastWrite(List instructions, char entity) {

    for (int i = instructions->count -1; i >= 0 ; i--)
        if (instructions->data[i][ENTITY] == entity && instructions->data[i][OPERATION] == 'W')
            return i;

    return -1;
}

bool isEqual(Graph scheduling) {
    List serialInstructions = malloc(sizeof(List));
    serialInstructions->data = malloc(sizeof(char **));
    serialInstructions->count = 0;
    for(int i = 0; i < scheduling->labels->count; i++){
         // group transactions from same transaction
        for (int j = 0; j < scheduling->instructions->count; j++)
            if (scheduling->instructions->data[j][TRANSACTION] == scheduling->labels->data[i]){
                addListData(serialInstructions, scheduling->instructions->data[j]);
            }
    }

    if (compareByVision(scheduling->instructions, serialInstructions))
        return 1;

    return 0;
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
    for (int i = 0; i < pGraph->nodesCount; i++)
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
            if (aciclicTerritory(pNode->neighbors[i]) == 0)
                return 0;
    }
    pNode->color = BLACK;
    return 1;
}

Graph readScheduling() {
    char input[4];

    Graph scheduling = initGraph();

    int time = 0;
    int transaction = 0;
    while(fscanf(stdin, "%d %d %c %c\n", &time, &transaction, &input[OPERATION], &input[ENTITY]) > 0){
        input[TIME] = (unsigned char) time;
        input[TRANSACTION] = (unsigned char) transaction;
        if (input[OPERATION] == 'C'){
            addListData(scheduling->instructions, input);
            removeArrayData(scheduling->awaiting, input[TRANSACTION]);
            if (scheduling->awaiting->data == NULL)
                return scheduling;
        }
        else{
            parseInstruction(scheduling, input);
            addListData(scheduling->instructions, input);
        }
    }
    return NULL;
}

void checkOperationsAfter(Graph scheduling, char findOperation, char transaction, char entity, char time) {
    List instructions = scheduling->instructions;

    for (int j = time + 1; j < instructions->count; j++)
        if (instructions->data[j][OPERATION] == findOperation && instructions->data[j][ENTITY] == entity && instructions->data[j][TRANSACTION] != transaction) //RX em Tj
            addEdges(scheduling, instructions->data[j][TRANSACTION], transaction);
}


void parseInstruction(Graph scheduling, const char *input) {
    char transaction = input[1];

    Node transaction_node = getNode(scheduling, transaction);

    if (transaction_node == NULL){
        transaction_node = newNode(input[TRANSACTION]);
        addNode(scheduling, transaction_node);
    }
}