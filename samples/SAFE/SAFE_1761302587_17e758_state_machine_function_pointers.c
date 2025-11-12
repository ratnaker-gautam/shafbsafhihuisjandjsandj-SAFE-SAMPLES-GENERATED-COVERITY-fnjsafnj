//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_DONE,
    STATE_ERROR
} state_t;

typedef state_t (*state_handler_t)(char);

state_t handle_start(char c);
state_t handle_reading(char c);
state_t handle_processing(char c);
state_t handle_done(char c);
state_t handle_error(char c);

state_t handle_start(char c) {
    if (c == 'B' || c == 'b') {
        printf("Beginning data read\n");
        return STATE_READING;
    }
    printf("Invalid start character: %c\n", c);
    return STATE_ERROR;
}

state_t handle_reading(char c) {
    if (c >= '0' && c <= '9') {
        printf("Read digit: %c\n", c);
        return STATE_READING;
    } else if (c == 'E' || c == 'e') {
        printf("End of data reached\n");
        return STATE_PROCESSING;
    }
    printf("Invalid data character: %c\n", c);
    return STATE_ERROR;
}

state_t handle_processing(char c) {
    if (c == 'C' || c == 'c') {
        printf("Processing complete\n");
        return STATE_DONE;
    }
    printf("Invalid processing command: %c\n", c);
    return STATE_ERROR;
}

state_t handle_done(char c) {
    printf("Machine finished\n");
    return STATE_DONE;
}

state_t handle_error(char c) {
    printf("Error state encountered\n");
    return STATE_ERROR;
}

int main() {
    state_handler_t handlers[] = {
        handle_start,
        handle_reading,
        handle_processing,
        handle_done,
        handle_error
    };
    
    char input[MAX_INPUT_LEN + 1];
    state_t current_state = STATE_START;
    size_t input_len;
    
    printf("Enter state machine input: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        if (current_state >= sizeof(handlers)/sizeof(handlers[0])) {
            fprintf(stderr, "Invalid state\n");
            return 1;
        }
        
        state_handler_t handler = handlers[current_state];
        if (handler == NULL) {
            fprintf(stderr, "No handler for state\n");
            return 1;
        }
        
        current_state = handler(input[i]);
        
        if (current_state == STATE_DONE || current_state == STATE_ERROR) {
            break;
        }
    }
    
    if (current_state != STATE_DONE && current_state != STATE_ERROR) {
        printf("Input ended before completion\n");
    }
    
    return 0;
}