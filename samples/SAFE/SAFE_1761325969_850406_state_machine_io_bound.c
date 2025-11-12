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
    if (str == NULL || str[0] == '\0') return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

int is_valid_text(const char *str) {
    if (str == NULL || str[0] == '\0') return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i]) && str[i] != ' ') return 0;
    }
    return 1;
}

int parse_input(struct StateMachine *sm, const char *input) {
    if (input == NULL) return 0;
    
    size_t len = strlen(input);
    if (len == 0 || len > 63) return 0;
    
    if (strcmp(input, "exit") == 0) {
        sm->current_state = EXIT;
        return 1;
    }
    else if (strcmp(input, "number") == 0) {
        sm->current_state = PROCESS_NUMBER;
        return 1;
    }
    else if (strcmp(input, "text") == 0) {
        sm->current_state = PROCESS_TEXT;
        return 1;
    }
    else if (is_valid_number(input)) {
        long val = strtol(input, NULL, 10);
        if (val >= 0 && val <= 1000000) {
            sm->number = (int)val;
            sm->current_state = PROCESS_NUMBER;
            return 1;
        }
    }
    else if (is_valid_text(input)) {
        if (strlen(input) < sizeof(sm->text)) {
            strncpy(sm->text, input, sizeof(sm->text) - 1);
            sm->text[sizeof(sm->text) - 1] = '\0';
            sm->text_len = strlen(sm->text);
            sm->current_state = PROCESS_TEXT;
            return 1;
        }
    }
    
    sm->current_state = ERROR;
    return 0;
}

void process_state(struct StateMachine *sm) {
    switch (sm->current_state) {
        case INIT:
            printf("State machine initialized. Enter commands: 'number', 'text', a number (0-1000000), text (letters/spaces), or 'exit'.\n");
            sm->current_state = READ_COMMAND;
            break;
            
        case READ_COMMAND:
            printf("Ready for command: ");
            break;
            
        case PROCESS_NUMBER:
            printf("Processing number: %d\n", sm->number);
            printf("Number squared: %ld\n", (long)sm->number * (long)sm->number);
            sm->current_state = READ_COMMAND;
            break;
            
        case PROCESS_TEXT:
            printf("Processing text: %s\n", sm->text);
            printf("Text length: %d\n", sm->text_len);
            sm->current_state = READ_COMMAND;
            break;
            
        case EXIT:
            printf("Exiting state machine.\n");
            break;
            
        case ERROR:
            printf("Error: Invalid input. Please try again.\n");
            sm->current_state = READ_COMMAND;
            break;
    }
}

int main(void) {
    struct StateMachine sm;
    char input[64];
    
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