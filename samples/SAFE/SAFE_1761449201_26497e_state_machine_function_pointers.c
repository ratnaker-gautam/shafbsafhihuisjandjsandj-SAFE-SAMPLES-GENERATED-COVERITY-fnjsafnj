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

typedef void (*StateHandler)(State*, char);

void handle_idle(State* state, char input) {
    if (input == 'r') {
        *state = STATE_READING;
        printf("Transition to READING\n");
    } else if (input == 'q') {
        *state = STATE_EXIT;
        printf("Transition to EXIT\n");
    } else {
        printf("Invalid input in IDLE state\n");
    }
}

void handle_reading(State* state, char input) {
    if (input == 'p') {
        *state = STATE_PROCESSING;
        printf("Transition to PROCESSING\n");
    } else if (input == 'i') {
        *state = STATE_IDLE;
        printf("Transition to IDLE\n");
    } else {
        printf("Invalid input in READING state\n");
    }
}

void handle_processing(State* state, char input) {
    if (input == 'w') {
        *state = STATE_WRITING;
        printf("Transition to WRITING\n");
    } else if (input == 'r') {
        *state = STATE_READING;
        printf("Transition to READING\n");
    } else {
        printf("Invalid input in PROCESSING state\n");
    }
}

void handle_writing(State* state, char input) {
    if (input == 'i') {
        *state = STATE_IDLE;
        printf("Transition to IDLE\n");
    } else if (input == 'p') {
        *state = STATE_PROCESSING;
        printf("Transition to PROCESSING\n");
    } else {
        printf("Invalid input in WRITING state\n");
    }
}

void handle_exit(State* state, char input) {
    printf("In EXIT state, ignoring input\n");
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
    char input_buffer[MAX_INPUT_LEN + 1];
    
    printf("State Machine Demo\n");
    printf("Available commands: r (read), p (process), w (write), i (idle), q (quit)\n");
    
    while (current_state != STATE_EXIT) {
        printf("Current state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_WRITING: printf("WRITING"); break;
            case STATE_EXIT: printf("EXIT"); break;
        }
        printf("\nEnter command: ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            printf("Error reading input\n");
            continue;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
            len--;
        }
        
        if (len != 1) {
            printf("Invalid input: must be single character\n");
            continue;
        }
        
        char command = input_buffer[0];
        StateHandler handler = get_handler(current_state);
        
        if (handler != NULL) {
            handler(&current_state, command);
        } else {
            printf("Invalid state encountered\n");
            break;
        }
    }
    
    printf("State machine terminated\n");
    return 0;
}