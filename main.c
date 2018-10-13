#include "main.h"

Variable newVariable(char entity, int value);

int getLastCommitedValue(Log *pLog, int count, char entity);

int main() {
    int schedulingCount = 0;

    Graph scheduling = NULL;

    while (scheduling != NULL || schedulingCount == 0){
        schedulingCount ++;

        scheduling = readScheduling();

        if (scheduling != NULL){
            InstructionList instructions = scheduling->instructions;

            for (int i = 0; i < instructions->count; i++) {
                //R(X)
                if (instructions->values[i].operation == 'R')
                    checkOperationsAfter(scheduling, 'W', instructions->values[i].transaction, instructions->values[i].entity,
                                         instructions->values[i].time);//W(X) after R(X)
                //W(X)
                if (instructions->values[i].operation == 'W') {
                    checkOperationsAfter(scheduling, 'R', instructions->values[i].transaction, instructions->values[i].entity,
                                         instructions->values[i].time);//R(X) after W(X)
                    checkOperationsAfter(scheduling, 'W', instructions->values[i].transaction, instructions->values[i].entity,
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

            logTransactions(scheduling);
            free(scheduling);
        }
    }
}

void logTransactions(Graph scheduling) {

    updateVariables(scheduling);



}

Variable newVariable(char entity, int value) {
    Variable variable = malloc(sizeof(struct variable));
    variable->name = entity;
    variable->value = value;
    return variable;
}

bool compareByVision(InstructionList originalScheduling, InstructionList serialScheduling) {

    VarsArray variables = malloc(sizeof(VarsArray));
    variables->values = malloc(sizeof(char *));

    Variable currentVar = NULL;

    //Tests writes order
    for(int i = 0; i < serialScheduling->count; i++){
        if (serialScheduling->values[i].operation == 'C')
            continue;

        currentVar = newVariable(serialScheduling->values[i].entity, NULL);
        addVariable(variables, currentVar);

        if (serialScheduling->values[i].operation == 'R'){
            Variable currentSerialVar = newVariable(serialScheduling->values[i].entity, NULL);

            Variable currentOriginalVar = newVariable(originalScheduling->values[i].entity, NULL);

            int timeFirstWriteSerial = findFirstWrite(serialScheduling, currentSerialVar);
            int timeFirstWriteOriginal = findFirstWrite(originalScheduling, currentOriginalVar);

            if ((timeFirstWriteSerial > 0 && timeFirstWriteOriginal > 0)
                && originalScheduling->values[timeFirstWriteOriginal].transaction != serialScheduling->values[timeFirstWriteSerial].transaction)
                return 0;
        }
    }

    // Test last writes for each entity
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

int findFirstWrite(InstructionList instructions, Variable variable) {

    for (int i = 0; i < instructions->count; i++)
        if (instructions->values[i].entity == variable->name && instructions->values[i].operation == 'W')
            return i;
    return -1;
}

int findLastWrite(InstructionList instructions, Variable variable) {

    for (int i = instructions->count -1; i >= 0 ; i--)
        if (instructions->values[i].entity == variable->name && instructions->values[i].operation == 'W')
            return i;

    return -1;
}

void printInstructions(InstructionList serialInstructions) {

    for(int i = 0; i < serialInstructions->count; i++){
        printf("\n Time: %d", serialInstructions->values[i].time);
        printf(", Operation: %c", serialInstructions->values[i].operation);
        printf(", Transaction: %d", serialInstructions->values[i].transaction);
        printf(", Entity: %c", serialInstructions->values[i].entity);
        printf(", Value: %d", serialInstructions->values[i].value);
    }

}

InstructionList serializeInstructions(Graph scheduling) {
    InstructionList serialInstructions = malloc(sizeof(InstructionList));
    serialInstructions->values = malloc(sizeof(char **));
    serialInstructions->count = 0;
    for(int i = 0; i < scheduling->transactionsIds->count; i++)
        // group instructions from same transaction
        for (int j = 0; j < scheduling->instructions->count; j++)
            if (scheduling->instructions->values[j].transaction == scheduling->transactionsIds->values[i])
                addInstruction(serialInstructions, scheduling->instructions->values[j]);

    return serialInstructions;
}
bool hasEquivalent(Graph scheduling) {

    InstructionList serialInstructions = serializeInstructions(scheduling);

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
    Instruction input;

    Graph scheduling = initGraph();

    while(fscanf(stdin, "%d %d %c %c ", &input.time, &input.transaction, &input.operation, &input.entity) > 0){
        if (input.operation == 'W'){
            fscanf(stdin, "%d\n", &input.value);
            printf("\nvalue read: %d", input.value);
        }
        else{
            fseek(stdin, 2, SEEK_CUR);
            input.value = 0;
        }

        if (input.operation == 'A'){
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

void updateVariables(Graph scheduling) {

    InstructionList serialInstructions = serializeInstructions(scheduling);
    int commitedTransaction = serialInstructions->values[0].transaction;
    int numOfTransactions = scheduling->nodesCount;

    printInstructions(serialInstructions);

//    for each commited transaction
    for(int currentTransacion = commitedTransaction; currentTransacion < numOfTransactions; currentTransacion++){
        puts("New transaction");
        for (int currentInstruction = 0; currentInstruction < serialInstructions->count; currentInstruction++){
            addToLogs(scheduling, serialInstructions->values[currentInstruction]);
            int instructionTime = serialInstructions->values[currentInstruction].time;
            printf("\n%d;T%d;start", instructionTime, currentTransacion);
        }
    }
}
void addToLogs(Graph scheduling, Instruction instruction) {
    scheduling->logs = realloc(scheduling->logs, sizeof(struct tLog) * scheduling->logsCount + 1);

    scheduling->logs[scheduling->logsCount] = initLog();

    Log log = scheduling->logs[scheduling->logsCount];

    log->initialValue = getLastCommitedValue(scheduling->logs, scheduling->logsCount, instruction.entity);

    log->varName = instruction.entity;

    scheduling->logsCount++;
}

int getLastCommitedValue(Log *pLog, int count, char entity) {
    return 0;
}

Log initLog() {
    Log log = malloc(sizeof(struct tLog));
    log->initialValue = NULL;
    log->newValue = NULL;
    log->varName = NULL;
}

void checkOperationsAfter(Graph scheduling, char findOperation, int transaction, char entity, int time) {
    InstructionList instructions = scheduling->instructions;

    for (int j = time + 1; j < instructions->count; j++)
        if (instructions->values[j].operation == findOperation && instructions->values[j].entity == entity && instructions->values[j].transaction != transaction) //RX em Tj
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
//    addToLogs(scheduling, input, value);
}
