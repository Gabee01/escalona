//
// Created by Gabriel Carraro on 10/13/18.
//
#include "logs.h"

LogsList initLogs() {
    LogsList logs;

    logs = malloc(sizeof(LogsList));
    logs->values = NULL;
    logs->count = 0;

    return logs;
}

void logTransactions(InstructionsList serializedInstructions, int transactionsCount, LogsList logs) {
    int committedTransaction = serializedInstructions->values[0].transaction;
    int lastTransaction = committedTransaction + transactionsCount;

//    printInstructions(serializedInstructions);
    int instructionIndex = 0;
//    for each committed transaction
    for(int currentTransaction = committedTransaction; currentTransaction < lastTransaction; currentTransaction++){
        addStartTransactionLog(logs, serializedInstructions->values[instructionIndex]);

        while (serializedInstructions->values[instructionIndex].transaction == currentTransaction){
            generateLog(logs, serializedInstructions->values[instructionIndex]);
            instructionIndex++;
        }
    }
}

void addStartTransactionLog(LogsList logs, Instruction instruction) {
    int lastLogTime = getNextLogTime(logs, instruction);
    Log log = malloc(sizeof(struct tLog));
    log->transaction = instruction.transaction;
    log->time = lastLogTime;
    log->action = ACTION_START;

    addLog(logs, log);
}

void printLogs(LogsList logs) {
    if (logs == NULL)
        return;

    Log currentLog;
    for(int i = 0; i <= logs->count; i++){
        currentLog = logs->values[i];
        if (i > 0)
            printf("\n");
        printf("%d;T%d;",currentLog->time, currentLog->transaction);
        switch(currentLog->action){
            case ACTION_START:
                printf("start");
                break;
            case ACTION_ABORT:
                printf("abort");
                break;
            case ACTION_COMMIT:
                printf("commit");
                break;
            case ACTION_CHANGEVAR:
                printf("%c;", currentLog->varName);
                if (currentLog->initialValue == NULL)
                    printf("NULL;");
                else
                    printf("%d;", currentLog->initialValue);
                printf("%d", currentLog->newValue);
            default:
                break;
        }
    }
}

void generateLog(LogsList logs, Instruction instruction){
    if (instruction.operation == 'R')
        return;

    int lastLogTime = getNextLogTime(logs, instruction);

    Log log = malloc(sizeof(struct tLog));
    log->transaction = instruction.transaction;
    log->time = lastLogTime;

    switch (instruction.operation){
        case 'W':
            log->action = ACTION_CHANGEVAR;
            log->varName = instruction.varName;
            log->newValue = instruction.value;
            log->initialValue = getVarInitialValue(logs, instruction);
            break;
        case 'C':
            log->action = ACTION_COMMIT;
            break;
        case 'A':
            log->action = ACTION_ABORT;
            break;
        default:
            break;
    }

    addLog(logs, log);
}

int getVarInitialValue(LogsList logs, Instruction instruction) {
    Log currentLog;
    int valueBeforeTransaction = NULL;

    for (int i = 0; i <= logs->count; i++){
        currentLog = logs->values[i];
        if (currentLog->transaction == instruction.transaction)
            return valueBeforeTransaction;

        if (currentLog->action == ACTION_CHANGEVAR && currentLog->varName == instruction.varName)
            valueBeforeTransaction = currentLog->newValue;
    }

    return valueBeforeTransaction;
}

int getNextLogTime(LogsList logs, Instruction instruction) {
    int lastLogTime;
    if(logs->values == NULL){
        return 1;
    }

    lastLogTime = logs->values[logs->count]->time;

    if (lastLogTime > instruction.time)
        return lastLogTime + 1;
    else
        return instruction.time;

}

void addLog(LogsList logs, Log log) {
    if (logs->values == NULL){
        logs->count = 0;
        logs->values = malloc(sizeof(struct tLog *));
    }
    else{
        if(log->action == ACTION_COMMIT && isReadOnly(logs, log->transaction)){
            logs->count--;
            return;
        }
        else{
            logs->count++;
            logs->values = realloc(logs->values, sizeof(struct tLog *) * (logs->count + 1));
        }
    }
    logs->values[logs->count] = malloc(sizeof(struct tLog));

    Log newLog = logs->values[logs->count];

    newLog->time = log->time;
    newLog->action = log->action;
    newLog->transaction = log->transaction;
    newLog->varName = log->varName;
    newLog->initialValue = log->initialValue;
    newLog->newValue = log->newValue;
}

int isReadOnly(LogsList logs, int transaction) {
    Log currentLog;
    for (int i = 0; i <= logs->count; i++){
        currentLog =logs->values[i];
        if (currentLog->transaction == transaction && currentLog->action == ACTION_CHANGEVAR)
            return 0;
    }

    return 1;
}