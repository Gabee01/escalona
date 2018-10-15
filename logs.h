//
// Created by Gabriel Carraro on 10/13/18.
//

#ifndef ESCALONA_REFACT_LOGS_H
#define ESCALONA_REFACT_LOGS_H

#include "dataStructures.h"
typedef struct tLog *Log;
typedef struct logsList *LogsList;

struct logsList{
    int count;
    Log *values;
} logsList;

struct tLog{
    int time;
    int action;
    int transaction;
    char varName;
    int initialValue;
    int newValue;
} tLog;

LogsList initLogs();
void logTransactions(InstructionsList serializedInstructions, int transactionsCount, LogsList logs);
int isReadOnly(LogsList pList, int transaction);
void generateLog(LogsList logs, Instruction instruction);
void addLog(LogsList logs, Log log);
void addStartTransactionLog(LogsList logs, Instruction instruction);
int getVarInitialValue(LogsList logs, Instruction instruction);
int getNextLogTime(LogsList logs, Instruction instruction);
void printLogs(LogsList logs);

#define ACTION_START 77
#define ACTION_CHANGEVAR 88
#define ACTION_COMMIT 99
#define ACTION_ABORT 11

#endif //ESCALONA_REFACT_LOGS_H
