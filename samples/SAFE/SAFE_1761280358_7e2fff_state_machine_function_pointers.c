//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR
} state_t;

typedef state_t (*state_handler_t)(const char*);

state_t handle_idle(const char* input) {
    if (input == NULL) return STATE_ERROR;
    if (strlen(input) == 0) return STATE_IDLE;
    if (strcmp(input, "start") == 0) return STATE_READING;
    return STATE_ERROR;
}

state_t handle_reading(const char* input) {
    if (input == NULL) return STATE_ERROR;
    size_t len = strlen(input);
    if (len == 0) return STATE_READING;
    if (len > MAX_INPUT_LEN) return STATE_ERROR;
    if (strcmp(input, "process") == 0) return STATE_PROCESSING;
    return STATE_READING;
}

state_t handle_processing(const char* input) {
    if (input == NULL) return STATE_ERROR;
    if (strcmp(input, "write") == 0) return STATE_WRITING;
    if (strcmp(input, "reset") == 0) return STATE_IDLE;
    return STATE_PROCESSING;
}

state_t handle_writing(const char* input) {
    if (input == NULL) return STATE_ERROR;
    if (strcmp(input, "done") == 0) return STATE_IDLE;
    if (strcmp(input, "reset") == 0) return STATE_IDLE;
    return STATE_WRITING;
}

state_t handle_error(const char* input) {
    if (input == NULL) return STATE_ERROR;
    if (strcmp(input, "reset") == 0) return STATE_IDLE;
    return STATE_ERROR;
}

state_handler_t get_handler(state_t state) {
    static state_handler_t handlers[] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error
    };
    
    if (state < 0 || state >= MAX_STATES) {
        return handle_error;
    }
    
    return handlers[state];
}

const char* state_name(state_t state) {
    static const char* names[] = {
        "IDLE",
        "READING",
        "PROCESSING",
        "WRITING",
        "ERROR"
    };
    
    if (state < 0 || state >= MAX_STATES) {
        return "UNKNOWN";
    }
    
    return names[state];
}

int main() {
    state_t current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 2];
    
    printf("State Machine Controller\n");
    printf("Available commands: start, process, write, done, reset\n");
    printf("Type 'quit' to exit\n\n");
    
    while (1) {
        printf("Current state: %s\n", state_name(current_state));
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
            current_state = STATE_ERROR;
            printf("Input too long\n");
            continue;
        }
        
        state_handler_t handler = get_handler(current_state);
        state_t new_state = handler(input_buffer);
        
        if (new_state == STATE_ERROR && current_state != STATE_ERROR) {
            printf("Invalid command for current state\n");
        }
        
        current_state = new_state;
    }
    
    printf("Exiting state machine\n");
    return 0;
}