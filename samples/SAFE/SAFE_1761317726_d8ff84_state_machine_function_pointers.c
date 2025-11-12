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
    STATE_WRITING,
    STATE_EXIT
} State;

typedef void (*StateHandler)(State*, char*);

void handle_idle(State* state, char* data) {
    if (data[0] == 's') {
        *state = STATE_READING;
        printf("Transition to READING state\n");
    } else if (data[0] == 'q') {
        *state = STATE_EXIT;
        printf("Transition to EXIT state\n");
    } else {
        printf("Invalid command in IDLE state\n");
    }
}

void handle_reading(State* state, char* data) {
    if (data[0] == 'p') {
        *state = STATE_PROCESSING;
        printf("Transition to PROCESSING state\n");
    } else if (data[0] == 'r') {
        *state = STATE_IDLE;
        printf("Transition to IDLE state\n");
    } else {
        printf("Invalid command in READING state\n");
    }
}

void handle_processing(State* state, char* data) {
    if (data[0] == 'w') {
        *state = STATE_WRITING;
        printf("Transition to WRITING state\n");
    } else if (data[0] == 'r') {
        *state = STATE_READING;
        printf("Transition to READING state\n");
    } else {
        printf("Invalid command in PROCESSING state\n");
    }
}

void handle_writing(State* state, char* data) {
    if (data[0] == 'd') {
        *state = STATE_IDLE;
        printf("Transition to IDLE state\n");
    } else if (data[0] == 'r') {
        *state = STATE_PROCESSING;
        printf("Transition to PROCESSING state\n");
    } else {
        printf("Invalid command in WRITING state\n");
    }
}

void handle_exit(State* state, char* data) {
    printf("In EXIT state\n");
}

StateHandler get_handler(State state) {
    static StateHandler handlers[] = {
        handle_idle,
        handle_reading,
        handle_processing,
        handle_writing,
        handle_exit
    };
    
    if (state >= 0 && state < MAX_STATES) {
        return handlers[state];
    }
    return NULL;
}

int main() {
    State current_state = STATE_IDLE;
    char input[MAX_INPUT_LEN];
    
    printf("State Machine Demo\n");
    printf("Commands: s(start), p(process), w(write), d(done), r(restart), q(quit)\n");
    
    while (current_state != STATE_EXIT) {
        printf("Current state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_WRITING: printf("WRITING"); break;
            default: break;
        }
        printf("\nEnter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) == 0) {
            printf("Empty input\n");
            continue;
        }
        
        StateHandler handler = get_handler(current_state);
        if (handler != NULL) {
            handler(&current_state, input);
        } else {
            printf("Invalid state\n");
            break;
        }
    }
    
    printf("Program terminated\n");
    return 0;
}