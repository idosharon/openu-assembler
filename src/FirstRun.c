#include "FirstRun.h"


void first_run(FILE* file, char* file_name) {
    char line[MAX_LINE_SIZE];
    char* token;
    int DC = 0;
    int IC = 0;
    label_node_t* label_list = NULL;

    FILE* obj_file = fopen("binary_test.o", "w");
    // Check if the file was opened successfully
    if (obj_file == NULL) {
        fprintf(stderr,"[Error]: Could not create object file\n");
        return;
    }

    while(fgets(line, MAX_LINE_SIZE, file) != NULL) {
        /* remove spaces from start */
        while (add_label(&line,label_list)) {}
    }
}

/* checks if the first word is a label and if so,
 * adds it to the label table and returns 1
 * if first word is not a label, returns 0 */
int add_label(char** line, label_node_t* labelList) {
    char* lab_end;
    char* label;
    char* first_space;
    lab_end = strchr(*line,':');
    if (lab_end == NULL)
        return 0;
    first_space =  strchr(*line, ' ');
    if (first_space < lab_end || !isalpha(**line)) {
        fprintf(stderr,"[Error]: Invalid label");
    }
    strncpy(label,*line,lab_end-first_space);
    labelList = addLabelNode(labelList,label,1);
}


label_node_t* addLabelNode( label_node_t* head, char* label, int place) {
    // Allocate memory for a new node
    label_node_t* newLabelNode = (label_node_t*)malloc(sizeof(label_node_t));
    // Assign the data to the new node
    label_t* newLabel = (label_t *)malloc(sizeof(label_t));
    newLabel->name = label;
    newLabel->place = place;

    newLabelNode->label = newLabel;
    // Set the next pointer to the current head of the list
    newLabelNode->next = head;
    // Set the head of the list to the new node
    head = newLabelNode;
    return head;
}
