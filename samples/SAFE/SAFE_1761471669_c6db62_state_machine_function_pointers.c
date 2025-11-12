//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINAL,
    STATE_ERROR
} State;

typedef State (*StateHandler)(const char*);

State handle_start(const char* input) {
    if (input == NULL) return STATE_ERROR;
    if (strlen(input) == 0) return STATE_START;
    return STATE_READING;
}

State handle_reading(const char* input) {
    if (input == NULL) return STATE_ERROR;
    size_t len = strlen(input);
    if (len == 0) return STATE_START;
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
    
    int sum = 0;
    size_t len = strlen(input);
    
    for (size_t i = 0; i < len; i++) {
        int digit = input[i] - '0';
        if (sum > (INT_MAX - digit) / 10) {
            return STATE_ERROR;
        }
        sum = sum * 10 + digit;
    }
    
    printf("Processed number: %d\n", sum);
    return STATE_FINAL;
}

State handle_final(const char* input) {
    if (input == NULL) return STATE_ERROR;
    printf("Processing complete. Enter new input or empty to exit.\n");
    return STATE_START;
}

State handle_error(const char* input) {
    if (input == NULL) return STATE_ERROR;
    printf("Error: Invalid input detected.\n");
    return STATE_START;
}

int main() {
    StateHandler handlers[MAX_STATES] = {
        handle_start,
        handle_reading,
        handle_processing,
        handle_final,
        handle_error
    };
    
    State current_state = STATE_START;
    char input_buffer[MAX_INPUT_LEN + 2];
    
    printf("Enter numbers (empty line to exit):\n");
    
    while (1) {
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
            len--;
        }
        
        if (len >= sizeof(input_buffer) - 1) {
            current_state = STATE_ERROR;
        } else {
            current_state = handlers[current_state](input_buffer);
        }
        
        if (current_state == STATE_START && len == 0) {
            break;
        }
        
        if (current_state >= MAX_STATES) {
            current_state = STATE_ERROR;
        }
    }
    
    printf("Exiting program.\n");
    return 0;
}