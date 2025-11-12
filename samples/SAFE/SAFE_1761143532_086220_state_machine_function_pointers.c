//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
} State;

typedef State (*StateHandler)(const char*);

State handle_idle(const char* input) {
    if (input == NULL) return STATE_IDLE;
    if (strlen(input) == 0) return STATE_IDLE;
    if (strcmp(input, "start") == 0) return STATE_READY;
    return STATE_ERROR;
}

State handle_ready(const char* input) {
    if (input == NULL) return STATE_READY;
    if (strlen(input) == 0) return STATE_READY;
    if (strcmp(input, "process") == 0) return STATE_PROCESSING;
    if (strcmp(input, "reset") == 0) return STATE_IDLE;
    return STATE_ERROR;
}

State handle_processing(const char* input) {
    if (input == NULL) return STATE_PROCESSING;
    if (strlen(input) == 0) return STATE_PROCESSING;
    if (strcmp(input, "complete") == 0) return STATE_COMPLETE;
    if (strcmp(input, "error") == 0) return STATE_ERROR;
    return STATE_PROCESSING;
}

State handle_complete(const char* input) {
    if (input == NULL) return STATE_COMPLETE;
    if (strlen(input) == 0) return STATE_COMPLETE;
    if (strcmp(input, "reset") == 0) return STATE_IDLE;
    return STATE_COMPLETE;
}

State handle_error(const char* input) {
    if (input == NULL) return STATE_ERROR;
    if (strlen(input) == 0) return STATE_ERROR;
    if (strcmp(input, "reset") == 0) return STATE_IDLE;
    return STATE_ERROR;
}

int main() {
    State current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1];
    StateHandler handlers[MAX_STATES] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_complete,
        handle_error
    };
    
    printf("State Machine Demo\n");
    printf("Available commands: start, process, complete, error, reset\n");
    printf("Type 'quit' to exit\n\n");
    
    while (1) {
        const char* state_names[] = {"IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"};
        printf("Current state: %s\n", state_names[current_state]);
        printf("Enter command: ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        }
        
        if (strcmp(input_buffer, "quit") == 0) {
            break;
        }
        
        if (strlen(input_buffer) >= MAX_INPUT_LEN) {
            printf("Input too long\n");
            continue;
        }
        
        if (current_state < 0 || current_state >= MAX_STATES) {
            printf("Invalid state\n");
            break;
        }
        
        State new_state = handlers[current_state](input_buffer);
        
        if (new_state < 0 || new_state >= MAX_STATES) {
            printf("Invalid state transition\n");
            break;
        }
        
        if (new_state != current_state) {
            printf("State transition: %s -> %s\n", 
                   state_names[current_state], state_names[new_state]);
            current_state = new_state;
        }
    }
    
    printf("Exiting state machine\n");
    return 0;
}