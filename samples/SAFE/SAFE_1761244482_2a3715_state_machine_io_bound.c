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

int parse_input(const char *input, struct StateMachine *sm) {
    char command[16];
    char value[64];
    
    if (sscanf(input, "%15s %63s", command, value) != 2) {
        return 0;
    }
    
    if (strcmp(command, "number") == 0) {
        if (is_valid_number(value)) {
            sm->number = atoi(value);
            sm->current_state = PROCESS_NUMBER;
            return 1;
        }
    } else if (strcmp(command, "text") == 0) {
        if (is_valid_text(value)) {
            strncpy(sm->text, value, sizeof(sm->text) - 1);
            sm->text[sizeof(sm->text) - 1] = '\0';
            sm->text_len = strlen(sm->text);
            sm->current_state = PROCESS_TEXT;
            return 1;
        }
    } else if (strcmp(command, "exit") == 0) {
        sm->current_state = EXIT;
        return 1;
    }
    
    return 0;
}

void process_state(struct StateMachine *sm) {
    switch (sm->current_state) {
        case INIT:
            printf("State machine initialized. Enter commands: 'number <value>', 'text <value>', or 'exit'\n");
            sm->current_state = READ_COMMAND;
            break;
            
        case READ_COMMAND:
            printf("Ready for command\n");
            break;
            
        case PROCESS_NUMBER:
            printf("Processed number: %d\n", sm->number);
            sm->current_state = READ_COMMAND;
            break;
            
        case PROCESS_TEXT:
            printf("Processed text: %s (length: %d)\n", sm->text, sm->text_len);
            sm->current_state = READ_COMMAND;
            break;
            
        case EXIT:
            printf("Exiting state machine\n");
            break;
            
        case ERROR:
            printf("Error state reached\n");
            sm->current_state = READ_COMMAND;
            break;
    }
}

int main(void) {
    struct StateMachine sm;
    char input[128];
    
    state_machine_init(&sm);
    
    printf("State Machine Demo\n");
    printf("==================\n");
    
    while (sm.current_state != EXIT) {
        process_state(&sm);
        
        if (sm.current_state == READ_COMMAND) {
            printf("> ");
            fflush(stdout);
            
            if (fgets(input, sizeof(input), stdin) == NULL) {
                break;
            }
            
            input[strcspn(input, "\n")] = '\0';
            
            if (strlen(input) == 0) {
                continue;
            }
            
            if (!parse_input(input, &sm)) {
                sm.current_state = ERROR;
            }
        }
    }
    
    return 0;
}