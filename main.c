//
// Created by Gabriel Carraro on 9/8/18.
//

#include "main.h"

int getVarInitialValue(LogsList pList, Instruction instruction);

int main() {
    int schedulingCount = 0;

    Graph scheduling = NULL;
    LogsList transactionsLogs = initLogs();
    while (scheduling != NULL || schedulingCount == 0){
        schedulingCount ++;

        scheduling = readScheduling();

        if (scheduling != NULL){
            InstructionsList instructions = scheduling->instructions;

            for (int i = 0; i < instructions->count; i++) {
                //R(X)
                if (instructions->values[i].operation == 'R')
                    checkOperationsAfter(scheduling, 'W', instructions->values[i].transaction, instructions->values[i].varName,
                                         instructions->values[i].time);//W(X) after R(X)
                //W(X)
                if (instructions->values[i].operation == 'W') {
                    checkOperationsAfter(scheduling, 'R', instructions->values[i].transaction, instructions->values[i].varName,
                                         instructions->values[i].time);//R(X) after W(X)
                    checkOperationsAfter(scheduling, 'W', instructions->values[i].transaction, instructions->values[i].varName,
                                         instructions->values[i].time);//W(X) after W(X)
                }
            }

// - - - - - - - - - - - T1 - - - - - - - - - - -
//            printf("%d ", schedulingCount);
//            for (int i = 0; i < scheduling->transactionsIds->count; i++)
//                if (i == scheduling->transactionsIds->count - 1)
//                    printf("%u ", scheduling->transactionsIds->values[i]);
//                else
//                    printf("%u,", scheduling->transactionsIds->values[i]);
//
//            if (isAciclic(scheduling))
//                printf("SS ");
//            else
//                printf("NS ");
//
//            if (hasEquivalent(scheduling))
//                printf("SV\n");
//            else
//                printf("NV\n");

// - - - - - - - - - - - T2 - - - - - - - - - - -

            InstructionsList serializedInstructions = serializeInstructions(scheduling);
            logTransactions(serializedInstructions, scheduling->nodesCount, transactionsLogs);

            free(scheduling);
        }
    }
    printLogs(transactionsLogs);
}

Variable newVariable(char entity, int value) {
    Variable variable = malloc(sizeof(struct variable));
    variable->name = entity;
    variable->value = value;
    return variable;
}

int compareByVision(InstructionsList originalScheduling, InstructionsList serialScheduling) {

    VarsArray variables = malloc(sizeof(VarsArray));
    variables->values = malloc(sizeof(char *));

    Variable currentVar = NULL;

    //Tests writes order
    for(int i = 0; i < serialScheduling->count; i++){
        if (serialScheduling->values[i].operation == 'C')
            continue;

        currentVar = newVariable(serialScheduling->values[i].varName, 0);
        addOrUpdateVariable(variables, currentVar, 0);

        if (serialScheduling->values[i].operation == 'R'){
            Variable currentSerialVar = newVariable(serialScheduling->values[i].varName, 0);

            Variable currentOriginalVar = newVariable(originalScheduling->values[i].varName, 0);

            int timeFirstWriteSerial = findFirstWrite(serialScheduling, currentSerialVar);
            int timeFirstWriteOriginal = findFirstWrite(originalScheduling, currentOriginalVar);

            if ((timeFirstWriteSerial > 0 && timeFirstWriteOriginal > 0)
                && originalScheduling->values[timeFirstWriteOriginal].transaction != serialScheduling->values[timeFirstWriteSerial].transaction)
                return 0;
        }
    }

    // Test last writes for each varName
    while (variables->count > 0){
        currentVar = variables->values[0];
        int timeLastWriteSerial = findLastWrite(serialScheduling, currentVar);
        int timeLastWriteOriginal = findLastWrite(originalScheduling, currentVar);

        if ((timeLastWriteOriginal > 0 && timeLastWriteSerial > 0) // if there is a write
            && serialScheduling->values[timeLastWriteSerial].transaction != originalScheduling->values[timeLastWriteOriginal].transaction) //and the first writes arent from the same transactions
            return 0;//

        removeVariable(variables, currentVar);
    }

    return 1;
}

int findFirstWrite(InstructionsList instructions, Variable variable) {

    for (int i = 0; i < instructions->count; i++)
        if (instructions->values[i].varName == variable->name && instructions->values[i].operation == 'W')
            return i;
    return -1;
}

int findLastWrite(InstructionsList instructions, Variable variable) {

    for (int i = instructions->count -1; i >= 0 ; i--)
        if (instructions->values[i].varName == variable->name && instructions->values[i].operation == 'W')
            return i;

    return -1;
}

void printInstruction(Instruction instruction){
    printf("\n Time: %d", instruction.time);
    printf(", Operation: %c", instruction.operation);
    printf(", Transaction: %d", instruction.transaction);
    printf(", Entity: %c", instruction.varName);
    if (instruction.operation == 'W')
        printf(", Value: %d", instruction.value);
}

void printInstructions(InstructionsList serialInstructions) {
    for(int i = 0; i < serialInstructions->count; i++)
        printInstruction(serialInstructions->values[i]);
}

InstructionsList serializeInstructions(Graph scheduling) {
    InstructionsList serialInstructions = malloc(sizeof(InstructionsList));
    serialInstructions->values = malloc(sizeof(char **));
    serialInstructions->count = 0;
    for(int i = 0; i < scheduling->transactionsIds->count; i++)
        // group instructions from same transaction
        for (int j = 0; j < scheduling->instructions->count; j++)
            if (scheduling->instructions->values[j].transaction == scheduling->transactionsIds->values[i])
                addInstruction(serialInstructions, scheduling->instructions->values[j]);

    return serialInstructions;
}
int hasEquivalent(Graph scheduling) {
    InstructionsList serialInstructions = serializeInstructions(scheduling);

//    printInstructions(serializedInstructions);

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
int isAciclic(Graph pGraph) {
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
int aciclicTerritory(Node pNode) {

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
    Instruction input;

    Graph scheduling = initGraph();

    while(fscanf(stdin, "%d %d %c %c ", &input.time, &input.transaction, &input.operation, &input.varName) > 0){
        if (input.operation == 'W'){
            fscanf(stdin, "%d\n", &input.value);
        }
        else{
            fseek(stdin, 2, SEEK_CUR);
            input.value = 0;
        }

        if (input.operation == 'A'){
            addInstruction(scheduling->instructions, input);
            removeInt(scheduling->awaiting, input.transaction);
            if (scheduling->awaiting->values == NULL)
                return scheduling;
        }
        if (input.operation == 'C'){
            addInstruction(scheduling->instructions, input);
            removeInt(scheduling->awaiting, input.transaction);
            if (scheduling->awaiting->values == NULL)
                return scheduling;
        }
        else{
            parseInstruction(scheduling, input);
        }
    }
    return NULL;
}

void checkOperationsAfter(Graph scheduling, char findOperation, int transaction, char entity, int time) {
    InstructionsList instructions = scheduling->instructions;

    for (int j = time + 1; j < instructions->count; j++)
        if (instructions->values[j].operation == findOperation && instructions->values[j].varName == entity && instructions->values[j].transaction != transaction) //RX em Tj
            addEdges(scheduling, instructions->values[j].transaction, transaction);
}

void parseInstruction(Graph scheduling, Instruction input) {
    int transaction = input.transaction;

    Node transaction_node = getNode(scheduling, transaction);

    if (transaction_node == NULL){
        transaction_node = newNode(input.transaction);
        addNode(scheduling, transaction_node);
    }

    addInstruction(scheduling->instructions, input);
}
