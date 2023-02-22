#include "FirstRun.h"


void firstRun(FILE* file, char* file_name) {
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

    bool label_flag = false;

    while(fgets(line, MAX_LINE_SIZE, file) != NULL) {
        /* add label */
        char* label = strtok(line, LABEL_SEP);
        label_flag = (label != NULL);

    }
}

/* checks if the first word is a label and if so,
 * adds it to the label table and returns 1
 * if first word is not a label, returns 0 */
int add_label(char* line, label_node_t** labelList) {
    char* label = strtok(line, LABEL_SEP);
    if(label != NULL) {
        /* found label */
        *labelList = addLabelNode(*labelList, label, 0);

    }

}


label_node_t* addLabelNode( label_node_t* head, char* label, int place) {
    label_node_t* newLabelNode = (label_node_t*) malloc(sizeof(label_node_t));
    label_t* newLabel = (label_t*) malloc(sizeof(label_t));
    newLabel->name = label;
    newLabel->place = place;
    newLabelNode->label = newLabel;
    newLabelNode->next = head;
    return newLabelNode;
}
