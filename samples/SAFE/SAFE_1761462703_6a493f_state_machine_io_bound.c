//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    INIT,
    READ_COMMAND,
    PROCESS_NUMBER,
    PROCESS_TEXT,
    EXIT,
    ERROR
};

struct StateMachine {
    enum State current_state;
    int number;
    char text[64];
    int text_len;
};

void state_machine_init(struct StateMachine *sm) {
    sm->current_state = INIT;
    sm->number = 0;
    sm->text_len = 0;
    memset(sm->text, 0, sizeof(sm->text));
}

int is_valid_number(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

int is_valid_text(const char *str) {
    if (str == NULL || *str == '\0') return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i]) && str[i] != ' ') return 0;
    }
    return 1;
}

int parse_input(struct StateMachine *sm, const char *input) {
    if (input == NULL) return 0;
    
    size_t len = strlen(input);
    if (len == 0 || len > 63) return 0;
    
    char cmd[64];
    char arg[64];
    int items = sscanf(input, "%63s %63[^\n]", cmd, arg);
    
    if (items < 1) return 0;
    
    if (strcmp(cmd, "number") == 0 && items == 2) {
        if (is_valid_number(arg)) {
            sm->number = atoi(arg);
            sm->current_state = PROCESS_NUMBER;
            return 1;
        }
    } else if (strcmp(cmd, "text") == 0 && items == 2) {
        if (is_valid_text(arg)) {
            strncpy(sm->text, arg, sizeof(sm->text) - 1);
            sm->text[sizeof(sm->text) - 1] = '\0';
            sm->text_len = strlen(sm->text);
            sm->current_state = PROCESS_TEXT;
            return 1;
        }
    } else if (strcmp(cmd, "exit") == 0) {
        sm->current_state = EXIT;
        return 1;
    }
    
    sm->current_state = ERROR;
    return 0;
}

void process_state(struct StateMachine *sm) {
    switch (sm->current_state) {
        case INIT:
            printf("State machine initialized. Commands: number <value>, text <string>, exit\n");
            sm->current_state = READ_COMMAND;
            break;
            
        case READ_COMMAND:
            printf("Enter command: ");
            break;
            
        case PROCESS_NUMBER:
            printf("Processed number: %d\n", sm->number);
            sm->current_state = READ_COMMAND;
            break;
            
        case PROCESS_TEXT:
            printf("Processed text: %s (length: %d)\n", sm->text, sm->text_len);
            sm->current_state = READ_COMMAND;
            break;
            
        case ERROR:
            printf("Error: Invalid command or argument\n");
            sm->current_state = READ_COMMAND;
            break;
            
        case EXIT:
            printf("Exiting state machine\n");
            break;
            
        default:
            sm->current_state = ERROR;
            break;
    }
}

int main(void) {
    struct StateMachine sm;
    char input[128];
    
    state_machine_init(&sm);
    
    while (1) {
        process_state(&sm);
        
        if (sm.current_state == EXIT) {
            break;
        }
        
        if (sm.current_state == READ_COMMAND) {
            if (fgets(input, sizeof(input), stdin) == NULL) {
                break;
            }
            
            size_t len = strlen(input);
            if (len > 0 && input[len - 1] == '\n') {
                input[len - 1] = '\0';
            }
            
            if (!parse_input(&sm, input)) {
                sm.current_state = ERROR;
            }
        }
    }
    
    return 0;
}