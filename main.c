#include <stdbool.h>
#include "main.h"
#include "graph.h"

Graph initGraph();
void parseInstruction(Graph scheduling, const char *input);
Node getNode(Graph scheduling, char label);
void addNode(Graph pGraph, Node pNode);
void addArrayData(Array array, char data);
void removeFromArray(Array array, char value);
void addListData(List list, const char data[4]) ;

int main() {

    Graph S = readScheduling();

    if (hasCicle(S))
        printf("SS");
    else
        printf("NS");
}

Graph readScheduling(){
    char input[4];
    Graph scheduling = initGraph();

    while(fscanf(stdin, "%c %c %c %c\n", &input[0], &input[1], &input[2], &input[3])){
        if (input[2] == 'C'){
            removeFromArray(scheduling->awaiting, input[1]);
            if (scheduling->awaiting->data == NULL)
                return scheduling;
        }

        parseInstruction(scheduling, input);
    }
    return NULL;
}

void removeFromArray(Array array, char value) {

    for (int i = 0; i < array->count; i++){
        if (array->data[i] == value){
            array->data[i] = array->data[array->count - 1];
            array->count--;
            if (array->count == 0)
                array->data = NULL;
        }
    }
}

Node newNode(const char label) {

    Node node = malloc(sizeof(struct node *));
    node->label = label; // Label = transaction name

    return node;
}

void parseInstruction(Graph scheduling, const char *input) {
    char transaction = input[1];

//    if (scheduling == NULL)
//        scheduling = initGraph();

    Node transaction_node = getNode(scheduling, transaction);

    if (transaction_node == NULL){
        transaction_node = newNode(input[1]);
        addNode(scheduling, transaction_node);
    }
    else{
        addListData(scheduling->instructions, input);
    }
}

void addNode(Graph pGraph, Node pNode){

    if (pGraph->nodesCount == 0)
        pGraph->nodes = malloc(sizeof(Node));
    else
        realloc(pGraph->nodes, (sizeof(Node *) * (pGraph->nodesCount + 1)));

    pGraph->nodes[pGraph->nodesCount] = pNode;
    addArrayData(pGraph->labels, pNode->label);
    addArrayData(pGraph->awaiting, pNode->label);

    pGraph->nodesCount++;
}

Node getNode(Graph scheduling, char label) {

     for (int i = 0; i < scheduling->nodesCount; i++) {
         if (scheduling->labels->data[i] == label){
             return scheduling->nodes[i];
         }
     }

     return NULL;
}

Graph initGraph() {
    Graph g = malloc(sizeof(Graph));

    g->nodes = malloc(sizeof(Node *));
    g->labels = malloc(sizeof(Array));
    g->labels->count = 0;
    g->awaiting = malloc(sizeof(Array));
    g->awaiting->count = 0;
    g->instructions = malloc(sizeof(List));
    g->instructions->count = 0;
    g->edgesCount = 0;
    g->nodesCount = 0;

    return g;
}

void addArrayData(Array array, char data) {
    if (array->count == 0)
        array->data = malloc(sizeof(char *));
    else
        realloc(array->data, sizeof(char *) * (array->count + 1));

    array->data[array->count] = data;
    array->count++;
}

void addListData(List list, const char data[4]) {
    if (list->count == 0)
        list->data = malloc((sizeof(char **)));
    else
        realloc(list->data, (sizeof(char **) * (list->count + 1)));

    list->data[list->count] = malloc(sizeof(char) * 4);

    list->data[list->count][0] = data[0];
    list->data[list->count][1] = data[1];
    list->data[list->count][2] = data[2];
    list->data[list->count][3] = data[3];

    list->count++;
}
