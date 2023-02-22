#include "FirstRun.h"


void first_run(FILE* file, char* file_name) {
    char line[MAX_LINE_SIZE];
    char* token;

    size_t DC = 0;
    size_t IC = 0;

    label_node_t* labels_list = NULL;
    FILE* obj_file;

    strcat(file_name, OBJ_FILE_EXTENSION);

    obj_file = fopen(file_name, "w");
    /* Check if the file was opened successfully */
    if (obj_file == NULL) {
        file_error(FILE_OPEN_ERROR, file_name);
        return;
    }

    while(fgets(line, MAX_LINE_SIZE, file) != NULL) {
        token = strtok(line, SPACE_SEP);
        /* check if token is label */
        if (add_label(token, labels_list)) {
            token = strtok(NULL, SPACE_SEP);
        }

    }
}

/* checks if the first word is a label and if so,
 * adds it to the label table and returns 1
 * if first word is not a label, returns 0 */
int add_label(char* line, label_node_t* labelList) {
    char* token = strtok(line, ":");
    if(strtok(NULL, SPACE_SEP)) {
        /* found letters after label */


    }

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
