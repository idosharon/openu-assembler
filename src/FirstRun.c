#include "FirstRun.h"

void firstRun(FILE* file, char* base_file_name) {
    char* line = (char*) malloc(MAX_LINE_SIZE * sizeof (char));
    size_t line_number = 0;
    char* token;
    char* temp_token;

    label_node_t * label_list = NULL;
    label_node_t * next_node = label_list;
    char* current_label = NULL;
    bool label_flag = false;

    size_t IC = 0;
    size_t DC = 0;

    /* read new line from file */
    while(fgets(line, MAX_LINE_SIZE, file) != NULL) {
        /* increase line counter */
        line_number++;

        /* split line into tokens */
        token = strtok(strdup(line), SPACE_SEP);

        /* check if there is a label */

        if(strchr(line,':')) {
            if (isValidLabelName(token)) {
                label_flag = true;
                current_label = token;
                token = strtok(NULL,SPACE_SEP);
            } else {
                line_error(LABEL_SYNTAX_ERROR, base_file_name, line_number);
                continue;
            }
        }
        if (strcmp(token,".data") == 0 ||
                strcmp(token,".string") == 0) {
            if (label_flag) {
                if (findLabel(current_label,label_list))
                    line_error(MULTIPLE_LABEL_DEFINITIONS,base_file_name,line_number);
                else {
                    if (strcmp(token,".data") == 0) {
                        label_list = addLabelNode(label_list, current_label, DC,Data);
                    }

                }
            }
        }
    }
    info_file("Finished first run", base_file_name);
}

label_t* findLabel(char* name, label_node_t * head) {
    while(head != NULL) {
        if(strcmp(head->label->name, name) == 0) {
            return head->label;
        }
        head = (label_node_t *) head->next;
    }
    return NULL;
}

bool isValidLabelName(char* label_name) {
    /* check if label name is valid - only contains alphabetic and numbers */
    bool endLabel = false;
    if (!isalpha(*label_name))
        return false;
    for(; *label_name != '\0'; label_name++) {
        if (endLabel)
            return false;
        if (*label_name == ':') {
            endLabel = true;
        }
        else if(!isalpha(*label_name) && !isdigit(*label_name))
            return false;
    }
    return true;
}

label_node_t * addLabelNode(label_node_t * head, char* name, int place , label_type labelType) {
    /* set current label to the new label */
    label_t * new_label = (label_t*) (malloc(sizeof (label_t)));
    label_node_t* new_label_node = (label_node_t*) (malloc(sizeof (label_node_t)));

    /* set label name and allocate its data */
    new_label->name = name;
    new_label->place = place;
    new_label->type = labelType;

    /* create new label node and append it to the start of list */
    new_label_node->label = new_label;
    new_label_node->next = (struct label_node_t *) head;
    return new_label_node;
}