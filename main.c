#include "main.h"

Log initLog();

void addToLogs(Graph scheduling, char *string);

Log getVarValue(Log *pLog, int, char i);

int main() {
    int schedulingCount = 0;

    Graph scheduling = NULL;

    while (scheduling != NULL || schedulingCount == 0){
        schedulingCount ++;

        scheduling = readScheduling();

        if (scheduling != NULL){
            List instructions = scheduling->instructions;

            for (int i = 0; i < instructions->count; i++) {
                //R(X)
                if (instructions->data[i][OPERATION] == 'R')
                    checkOperationsAfter(scheduling, 'W', instructions->data[i][TRANSACTION], instructions->data[i][ENTITY],
                                         instructions->data[i][TIME]);//W(X) after R(X)
                //W(X)
                if (instructions->data[i][OPERATION] == 'W') {
                    checkOperationsAfter(scheduling, 'R', instructions->data[i][TRANSACTION], instructions->data[i][ENTITY],
                                         instructions->data[i][TIME]);//R(X) after W(X)
                    checkOperationsAfter(scheduling, 'W', instructions->data[i][TRANSACTION], instructions->data[i][ENTITY],
                                         instructions->data[i][TIME]);//W(X) after W(X)
                }
            }

// - - - - - - - - - - - T1 - - - - - - - - - - -
//            printf("%d ", schedulingCount);
//            for (int i = 0; i < scheduling->transactionsIds->count; i++)
//                if (i == scheduling->transactionsIds->count - 1)
//                    printf("%u ", scheduling->transactionsIds->data[i]);
//                else
//                    printf("%u,", scheduling->transactionsIds->data[i]);
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

            logTransactions(scheduling);
            free(scheduling);
        }
    }
}

void logTransactions(Graph scheduling) {

    updateVariables(scheduling);



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

void printInstructions(List serialInstructions) {

    for(int i = 0; i < serialInstructions->count; i++){
        printf("\n Time: %d", serialInstructions->data[i][TIME]);
        printf(", Operation: %c", serialInstructions->data[i][OPERATION]);
        printf(", Transaction: %d", serialInstructions->data[i][TRANSACTION]);
        printf(", Entity: %c", serialInstructions->data[i][ENTITY]);
        if (serialInstructions->data[i][VALUE] == NULL)
            printf(", Value: %d", serialInstructions->data[i][VALUE]);
    }

}

List serializeInstructions(Graph scheduling) {
    List serialInstructions = malloc(sizeof(List));
    serialInstructions->data = malloc(sizeof(char **));
    serialInstructions->count = 0;
    for(int i = 0; i < scheduling->transactionsIds->count; i++)
        // group instructions from same transaction
        for (int j = 0; j < scheduling->instructions->count; j++)
            if (scheduling->instructions->data[j][TRANSACTION] == scheduling->transactionsIds->data[i])
                addListData(serialInstructions, scheduling->instructions->data[j]);

    return serialInstructions;
}
bool hasEquivalent(Graph scheduling) {

    List serialInstructions = serializeInstructions(scheduling);

//    printInstructions(serialInstructions);

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
    char input[DATASIZE];

    Graph scheduling = initGraph();

    int time = 0;
    int transaction = 0;
    int value = 0;

    while(fscanf(stdin, "%d %d %c %c ", &time, &transaction, &input[OPERATION], &input[ENTITY]) > 0){
        if (input[OPERATION] == 'W'){
            fscanf(stdin, "%d\n", &value);
            sprintf(input[VALUE], "%d", value);
            printf("\nvalue read: %d", input[VALUE]);
        }
        else{
            fseek(stdin, 2, SEEK_CUR);
            input[VALUE] = NULL;
        }

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
        }
    }
    return NULL;
}

void updateVariables(Graph scheduling) {

    List serialInstructions = serializeInstructions(scheduling);
    int commitedTransaction = serialInstructions->data[0][TRANSACTION];
    int numOfTransactions = scheduling->nodesCount;

    printInstructions(serialInstructions);

    if (scheduling->logs == NULL)
            scheduling->logs = initLog();

    //for each commited transaction
    for(int currentTransacion = commitedTransaction; currentTransacion < numOfTransactions; currentTransacion++){
//        1;T1;start
        for (int currentInstruction = 0; currentInstruction < serialInstructions->count; currentInstruction++){
            addToLogs(scheduling, serialInstructions->data[currentInstruction]);
//                int instructionTime = serialInstructions->data[currentInstruction][TIME];
//                printf("\n%d;T%d;start", instructionTime, currentTransacion);
        }
    }
}
void addToLogs(Graph scheduling, char *instruction) {
    Log log = scheduling->logs[scheduling->logsCount];

    if (log == NULL)
        log = malloc(sizeof(struct tLog));
    else
        log = realloc(log, sizeof(struct tLog) * scheduling->logsCount + 1);

//    log->initialValue = getVarValue(scheduling->logs, scheduling->logsCount, instruction[ENTITY]);

    log->varName = instruction[ENTITY];

    scheduling->logsCount++;
}

Log getVarValue(Log *currentLogs, int logSize, char i) {
    Log varLog = initLog();
    for(int j = 0; j < logSize; j++){
        if (currentLogs[i]->varName == i){
            if (varLog->initialValue == NULL)
                varLog->initialValue = currentLogs[i]->initialValue;
            varLog->newValue = currentLogs[i]->newValue;
        }
    }

    return varLog;
}

Log initLog() {
    Log log = malloc(sizeof(struct tLog));
    log->initialValue = NULL;
    log->newValue = NULL;
    log->varName = NULL;
}

void checkOperationsAfter(Graph scheduling, char findOperation, char transaction, char entity, char time) {
    List instructions = scheduling->instructions;

    for (int j = time + 1; j < instructions->count; j++)
        if (instructions->data[j][OPERATION] == findOperation && instructions->data[j][ENTITY] == entity && instructions->data[j][TRANSACTION] != transaction) //RX em Tj
            addEdges(scheduling, instructions->data[j][TRANSACTION], transaction);
}

void parseInstruction(Graph scheduling, char *input) {
    char transaction = input[TRANSACTION];

    Node transaction_node = getNode(scheduling, transaction);

    if (transaction_node == NULL){
        transaction_node = newNode(input[TRANSACTION]);
        addNode(scheduling, transaction_node);
    }

    addListData(scheduling->instructions, input);
//    addToLogs(scheduling, input, value);
}
