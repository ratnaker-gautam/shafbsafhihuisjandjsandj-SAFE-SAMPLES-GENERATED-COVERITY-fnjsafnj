//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_DONE,
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
    if (len > MAX_INPUT_LEN) return STATE_ERROR;
    if (len == 0) return STATE_START;
    return STATE_PROCESSING;
}

State handle_processing(const char* input) {
    if (input == NULL) return STATE_ERROR;
    size_t len = strlen(input);
    if (len == 0) return STATE_ERROR;
    
    int digit_count = 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] >= '0' && input[i] <= '9') {
            digit_count++;
        }
    }
    
    printf("Input contains %d digits\n", digit_count);
    return STATE_DONE;
}

State handle_done(const char* input) {
    return STATE_START;
}

State handle_error(const char* input) {
    printf("Error: Invalid input detected\n");
    return STATE_START;
}

int main() {
    StateHandler handlers[] = {
        handle_start,
        handle_reading,
        handle_processing,
        handle_done,
        handle_error
    };
    
    State current_state = STATE_START;
    char input_buffer[MAX_INPUT_LEN + 1];
    
    printf("String Digit Counter (type 'quit' to exit)\n");
    
    while (1) {
        printf("Enter string: ");
        
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
        
        if (current_state < 0 || current_state >= sizeof(handlers)/sizeof(handlers[0])) {
            current_state = STATE_ERROR;
        }
        
        StateHandler handler = handlers[current_state];
        current_state = handler(input_buffer);
        
        if (current_state < 0 || current_state >= sizeof(handlers)/sizeof(handlers[0])) {
            current_state = STATE_ERROR;
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}