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
    long sum = 0;
    size_t len = strlen(input);
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return STATE_ERROR;
        int digit = input[i] - '0';
        if (sum > (LONG_MAX - digit) / 10) return STATE_ERROR;
        sum = sum * 10 + digit;
    }
    printf("Sum of digits: %ld\n", sum);
    return STATE_FINISHED;
}

State handle_finished(const char* input) {
    if (input == NULL) return STATE_ERROR;
    return STATE_START;
}

State handle_error(const char* input) {
    if (input == NULL) return STATE_ERROR;
    printf("Error: Invalid input\n");
    return STATE_START;
}

int main() {
    StateHandler handlers[MAX_STATES] = {
        handle_start,
        handle_reading,
        handle_processing,
        handle_finished,
        handle_error
    };
    
    State current_state = STATE_START;
    char input_buffer[MAX_INPUT_LEN + 1];
    
    printf("Enter numbers (empty line to exit):\n");
    
    while (current_state != STATE_ERROR) {
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
        
        if (current_state < 0 || current_state >= MAX_STATES) {
            current_state = STATE_ERROR;
            continue;
        }
        
        StateHandler handler = handlers[current_state];
        if (handler == NULL) {
            current_state = STATE_ERROR;
            continue;
        }
        
        current_state = handler(input_buffer);
    }
    
    printf("Goodbye!\n");
    return 0;
}