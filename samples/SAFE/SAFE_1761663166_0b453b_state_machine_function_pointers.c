//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR
} State;

typedef State (*StateHandler)(const char*);

State handle_idle(const char* input) {
    if (input == NULL) return STATE_ERROR;
    if (strlen(input) == 0) return STATE_IDLE;
    if (strlen(input) > MAX_INPUT_LEN) return STATE_ERROR;
    return STATE_READING;
}

State handle_reading(const char* input) {
    if (input == NULL) return STATE_ERROR;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return STATE_ERROR;
    
    int valid = 1;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') {
            valid = 0;
            break;
        }
    }
    
    return valid ? STATE_PROCESSING : STATE_ERROR;
}

State handle_processing(const char* input) {
    if (input == NULL) return STATE_ERROR;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return STATE_ERROR;
    
    long sum = 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return STATE_ERROR;
        int digit = input[i] - '0';
        if (sum > (LONG_MAX - digit) / 10) return STATE_ERROR;
        sum = sum * 10 + digit;
    }
    
    printf("Processed value: %ld\n", sum);
    return STATE_WRITING;
}

State handle_writing(const char* input) {
    if (input == NULL) return STATE_ERROR;
    printf("Writing completed for: %s\n", input);
    return STATE_IDLE;
}

State handle_error(const char* input) {
    printf("Error state reached. Invalid input detected.\n");
    return STATE_IDLE;
}

int main() {
    StateHandler handlers[MAX_STATES] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_error
    };
    
    State current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1];
    
    printf("State Machine Demo - Enter numbers or 'quit' to exit\n");
    
    while (1) {
        printf("Current state: %d | Enter input: ", current_state);
        
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
        
        if (current_state < 0 || current_state >= MAX_STATES) {
            current_state = STATE_ERROR;
        }
        
        StateHandler handler = handlers[current_state];
        if (handler == NULL) {
            current_state = STATE_ERROR;
            continue;
        }
        
        current_state = handler(input_buffer);
        
        if (current_state < 0 || current_state >= MAX_STATES) {
            current_state = STATE_ERROR;
        }
    }
    
    printf("State machine terminated.\n");
    return 0;
}