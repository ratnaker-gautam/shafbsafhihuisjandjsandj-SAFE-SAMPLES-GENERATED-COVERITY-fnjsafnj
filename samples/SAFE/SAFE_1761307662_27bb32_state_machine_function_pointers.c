//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINISHED,
    STATE_ERROR
} State;

typedef State (*StateHandler)(char);

State handle_start(char c) {
    if (c == '\0') {
        return STATE_ERROR;
    }
    if (c == 'B' || c == 'b') {
        printf("Beginning sequence\n");
        return STATE_READING;
    }
    return STATE_ERROR;
}

State handle_reading(char c) {
    if (c == '\0') {
        return STATE_ERROR;
    }
    if (c == 'E' || c == 'e') {
        printf("End of input detected\n");
        return STATE_PROCESSING;
    }
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
        printf("Reading character: %c\n", c);
        return STATE_READING;
    }
    return STATE_ERROR;
}

State handle_processing(char c) {
    if (c == '\0') {
        printf("Processing complete\n");
        return STATE_FINISHED;
    }
    return STATE_ERROR;
}

State handle_finished(char c) {
    return STATE_FINISHED;
}

State handle_error(char c) {
    printf("Invalid state transition\n");
    return STATE_ERROR;
}

int main() {
    StateHandler handlers[MAX_STATES] = {
        handle_start,
        handle_reading,
        handle_processing,
        handle_finished,
        handle_error
    };
    
    char input[MAX_INPUT_LEN + 1];
    printf("Enter a sequence (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    State current_state = STATE_START;
    
    for (size_t i = 0; i <= len; i++) {
        char current_char = (i < len) ? input[i] : '\0';
        
        if (current_state < 0 || current_state >= MAX_STATES) {
            fprintf(stderr, "Invalid state\n");
            return 1;
        }
        
        StateHandler handler = handlers[current_state];
        if (handler == NULL) {
            fprintf(stderr, "No handler for state\n");
            return 1;
        }
        
        State next_state = handler(current_char);
        
        if (next_state < 0 || next_state >= MAX_STATES) {
            fprintf(stderr, "Invalid next state\n");
            return 1;
        }
        
        current_state = next_state;
        
        if (current_state == STATE_FINISHED) {
            break;
        }
    }
    
    if (current_state != STATE_FINISHED) {
        fprintf(stderr, "Sequence did not complete successfully\n");
        return 1;
    }
    
    printf("State machine completed successfully\n");
    return 0;
}