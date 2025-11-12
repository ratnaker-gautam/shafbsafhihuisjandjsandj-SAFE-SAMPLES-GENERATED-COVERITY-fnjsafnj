//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_OUTPUT,
    STATE_ERROR
} State;

typedef void (*StateHandler)(char, State*);

void handle_idle(char c, State* state) {
    if (c == 'S') {
        *state = STATE_READING;
    } else if (c != ' ' && c != '\t' && c != '\n') {
        *state = STATE_ERROR;
    }
}

void handle_reading(char c, State* state) {
    if (c == 'P') {
        *state = STATE_PROCESSING;
    } else if (c == ' ' || c == '\t' || c == '\n') {
        return;
    } else {
        *state = STATE_ERROR;
    }
}

void handle_processing(char c, State* state) {
    if (c == 'O') {
        *state = STATE_OUTPUT;
    } else if (c == ' ' || c == '\t' || c == '\n') {
        return;
    } else {
        *state = STATE_ERROR;
    }
}

void handle_output(char c, State* state) {
    if (c == 'E') {
        *state = STATE_IDLE;
    } else if (c == ' ' || c == '\t' || c == '\n') {
        return;
    } else {
        *state = STATE_ERROR;
    }
}

void handle_error(char c, State* state) {
    if (c == 'R') {
        *state = STATE_IDLE;
    }
}

int main() {
    StateHandler handlers[MAX_STATES] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_output,
        handle_error
    };
    
    char input[MAX_INPUT_LEN + 1];
    State current_state = STATE_IDLE;
    
    printf("Enter state sequence (S P O E or R to reset from error): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    printf("Processing sequence: %s\n", input);
    printf("State transitions:\n");
    
    for (size_t i = 0; i < len; i++) {
        State previous_state = current_state;
        
        if (current_state < 0 || current_state >= MAX_STATES) {
            fprintf(stderr, "Invalid state\n");
            return 1;
        }
        
        handlers[current_state](input[i], &current_state);
        
        printf("Char '%c': %d -> %d\n", input[i], previous_state, current_state);
        
        if (current_state == STATE_ERROR) {
            printf("Error state reached. Send 'R' to reset.\n");
        }
    }
    
    printf("Final state: %d\n", current_state);
    
    if (current_state == STATE_IDLE) {
        printf("Sequence completed successfully\n");
    } else if (current_state != STATE_ERROR) {
        printf("Sequence incomplete\n");
    }
    
    return 0;
}