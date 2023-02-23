#include "FirstRun.h"

void firstRun(FILE* file, char* base_file_name) {
    char* line = (char*) malloc(MAX_LINE_SIZE * sizeof (char));
    size_t line_number = 0;
    char* token;

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
        if(strchr(line,LABEL_SEP)) {
            if (isValidLabelName(token)) {
                label_flag = true;
                current_label = token;
                current_label[strlen(current_label)-1] = '\0';
                token = strtok(NULL,SPACE_SEP);
            } else {
                line_error(LABEL_SYNTAX_ERROR, base_file_name, line_number);
                continue;
            }
        }

        if (IS_DATA_SYMBOL(token)) {
            if (label_flag) {
                if (findLabel(current_label,label_list))
                    line_error(MULTIPLE_LABEL_DEFINITIONS, base_file_name,line_number);
                else {
                    label_list = addLabelNode(label_list, current_label, DC, Data);
                    if (is_equal(token, DATA_SYMBOL)) {
                        /* calculate data length */
                        while((token = strtok(NULL,COMMA_SEP)) != NULL) {
                            if (is_number(token)) {
                                DC++;
                            } else {
                                line_error(DATA_SYNTAX_ERROR, base_file_name,line_number);
                                break;
                            }
                        }
                    }
                    else if (is_equal(token, STRING_SYMBOL)) {
                        token = strtok(NULL,SPACE_SEP);
                        DC += strlen(token)-1;
                    }
                }
            }
        } else if (IS_EXTERN_SYMBOL(token) || IS_ENTRY_SYMBOL(token)) {
            if(IS_EXTERN_SYMBOL(token)) {

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
        if (endLabel) return false;
        if (*label_name == LABEL_SEP) {
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
