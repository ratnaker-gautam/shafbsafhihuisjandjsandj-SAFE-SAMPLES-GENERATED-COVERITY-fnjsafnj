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
        printf("Started reading\n");
    } else if (c != ' ' && c != '\t' && c != '\n') {
        *state = STATE_ERROR;
        printf("Invalid start character\n");
    }
}

void handle_reading(char c, State* state) {
    if (c == 'E') {
        *state = STATE_PROCESSING;
        printf("End reading, processing\n");
    } else if (c == 'X') {
        *state = STATE_ERROR;
        printf("Abort during reading\n");
    } else if (c >= 'a' && c <= 'z') {
        printf("Read: %c\n", c);
    } else if (c != ' ' && c != '\t' && c != '\n') {
        *state = STATE_ERROR;
        printf("Invalid character during reading\n");
    }
}

void handle_processing(char c, State* state) {
    if (c == 'P') {
        *state = STATE_OUTPUT;
        printf("Processing complete, ready for output\n");
    } else if (c == 'X') {
        *state = STATE_ERROR;
        printf("Abort during processing\n");
    } else if (c != ' ' && c != '\t' && c != '\n') {
        *state = STATE_ERROR;
        printf("Invalid character during processing\n");
    }
}

void handle_output(char c, State* state) {
    if (c == 'O') {
        printf("Output delivered\n");
        *state = STATE_IDLE;
    } else if (c == 'X') {
        *state = STATE_ERROR;
        printf("Abort during output\n");
    } else if (c != ' ' && c != '\t' && c != '\n') {
        *state = STATE_ERROR;
        printf("Invalid character during output\n");
    }
}

void handle_error(char c, State* state) {
    if (c == 'R') {
        *state = STATE_IDLE;
        printf("Reset to idle\n");
    } else if (c != ' ' && c != '\t' && c != '\n') {
        printf("Still in error state, press R to reset\n");
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
    
    State current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN + 1];
    
    printf("State Machine Controller\n");
    printf("Commands: S=Start, E=End reading, P=Process, O=Output, R=Reset, X=Abort\n");
    printf("Enter commands (max %d chars):\n", MAX_INPUT_LEN);
    
    while (1) {
        printf("Current state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_OUTPUT: printf("OUTPUT"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf(" > ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            continue;
        }
        
        if (strcmp(input, "QUIT") == 0) {
            break;
        }
        
        for (size_t i = 0; i < len; i++) {
            if (current_state >= 0 && current_state < MAX_STATES) {
                handlers[current_state](input[i], &current_state);
            }
        }
    }
    
    printf("Exiting state machine\n");
    return 0;
}