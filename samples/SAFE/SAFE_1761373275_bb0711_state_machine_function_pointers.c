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
    STATE_OUTPUT,
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
    if (len == 0) return STATE_ERROR;
    if (len > MAX_INPUT_LEN) return STATE_ERROR;
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') {
            return STATE_ERROR;
        }
    }
    return STATE_PROCESSING;
}

State handle_processing(const char* input) {
    if (input == NULL) return STATE_ERROR;
    size_t len = strlen(input);
    if (len == 0) return STATE_ERROR;
    
    int sum = 0;
    for (size_t i = 0; i < len; i++) {
        int digit = input[i] - '0';
        if (sum > (INT_MAX - digit) / 10) {
            return STATE_ERROR;
        }
        sum = sum * 10 + digit;
    }
    
    printf("Processed number: %d\n", sum);
    return STATE_OUTPUT;
}

State handle_output(const char* input) {
    if (input == NULL) return STATE_ERROR;
    printf("Output complete. Enter new number or 'quit': ");
    return STATE_IDLE;
}

State handle_error(const char* input) {
    if (input == NULL) return STATE_ERROR;
    printf("Error: Invalid input. Please enter numbers only.\n");
    return STATE_IDLE;
}

int main() {
    StateHandler handlers[MAX_STATES] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_output,
        handle_error
    };
    
    State current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 2];
    
    printf("Number Processor State Machine\n");
    printf("Enter numbers to process or 'quit' to exit:\n");
    
    while (1) {
        if (current_state == STATE_IDLE) {
            printf("Enter number: ");
        }
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        } else if (len >= MAX_INPUT_LEN) {
            current_state = STATE_ERROR;
            continue;
        }
        
        if (strcmp(input_buffer, "quit") == 0) {
            break;
        }
        
        if (current_state >= 0 && current_state < MAX_STATES) {
            current_state = handlers[current_state](input_buffer);
        } else {
            current_state = STATE_ERROR;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}