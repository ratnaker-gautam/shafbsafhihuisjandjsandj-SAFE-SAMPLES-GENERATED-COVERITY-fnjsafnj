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
    return STATE_PROCESSING;
}

State handle_processing(const char* input) {
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
    
    return valid ? STATE_FINISHED : STATE_ERROR;
}

State handle_finished(const char* input) {
    if (input == NULL) return STATE_ERROR;
    return STATE_FINISHED;
}

State handle_error(const char* input) {
    if (input == NULL) return STATE_ERROR;
    return STATE_ERROR;
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
    
    printf("Enter numeric strings (empty line to exit):\n");
    
    while (1) {
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
        
        if ((int)current_state < 0 || (int)current_state >= MAX_STATES) {
            current_state = STATE_ERROR;
        }
        
        StateHandler handler = handlers[(int)current_state];
        if (handler == NULL) {
            current_state = STATE_ERROR;
        } else {
            current_state = handler(input_buffer);
        }
        
        switch (current_state) {
            case STATE_START:
                printf("Ready for input\n");
                break;
            case STATE_READING:
                printf("Input received\n");
                break;
            case STATE_PROCESSING:
                printf("Processing input\n");
                break;
            case STATE_FINISHED:
                printf("Valid numeric string: %s\n", input_buffer);
                current_state = STATE_START;
                break;
            case STATE_ERROR:
                printf("Invalid input: %s\n", input_buffer);
                current_state = STATE_START;
                break;
        }
    }
    
    printf("Program terminated\n");
    return 0;
}