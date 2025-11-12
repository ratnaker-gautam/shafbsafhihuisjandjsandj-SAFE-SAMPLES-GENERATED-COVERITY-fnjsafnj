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
    STATE_FINISHED,
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
        if (input[i] < '0' || input[i] > '9') return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

State handle_processing(const char* input) {
    if (input == NULL) return STATE_ERROR;
    size_t len = strlen(input);
    if (len == 0) return STATE_ERROR;
    int sum = 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return STATE_ERROR;
        int digit = input[i] - '0';
        if (sum > (INT_MAX - digit)) return STATE_ERROR;
        sum += digit;
    }
    printf("Sum of digits: %d\n", sum);
    return STATE_FINISHED;
}

State handle_finished(const char* input) {
    if (input == NULL) return STATE_ERROR;
    if (strlen(input) == 0) return STATE_START;
    return STATE_READING;
}

State handle_error(const char* input) {
    if (input == NULL) return STATE_ERROR;
    printf("Invalid input. Please enter only digits (max %d characters).\n", MAX_INPUT_LEN);
    return STATE_START;
}

int main(void) {
    StateHandler handlers[MAX_STATES] = {
        handle_start,
        handle_reading,
        handle_processing,
        handle_finished,
        handle_error
    };
    
    State current_state = STATE_START;
    char input_buffer[MAX_INPUT_LEN + 2];
    
    printf("Digit Sum Calculator State Machine\n");
    printf("Enter numbers (digits only) or empty line to exit.\n");
    
    while (1) {
        if (current_state == STATE_FINISHED) {
            current_state = STATE_START;
        }
        
        printf("State %d > ", current_state);
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
            len--;
        }
        
        if (len == 0 && current_state == STATE_START) {
            break;
        }
        
        if (current_state >= 0 && current_state < MAX_STATES) {
            current_state = handlers[current_state](input_buffer);
        } else {
            current_state = STATE_ERROR;
        }
    }
    
    printf("Exiting state machine.\n");
    return 0;
}