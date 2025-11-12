//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { IDLE, READY, PROCESSING, COMPLETE, ERROR };

typedef void (*StateHandler)(enum State *current_state, int *data);

void handle_idle(enum State *current_state, int *data) {
    char input[32];
    printf("IDLE: Enter 'start' to begin or 'quit' to exit: ");
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "start") == 0) {
            *current_state = READY;
            *data = 0;
        } else if (strcmp(input, "quit") == 0) {
            *current_state = COMPLETE;
        }
    }
}

void handle_ready(enum State *current_state, int *data) {
    char input[32];
    printf("READY: Enter a number (1-100) to process or 'back' to return: ");
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "back") == 0) {
            *current_state = IDLE;
        } else {
            char *endptr;
            long value = strtol(input, &endptr, 10);
            if (endptr != input && *endptr == '\0' && value >= 1 && value <= 100) {
                *data = (int)value;
                *current_state = PROCESSING;
            } else {
                printf("Invalid input. Please enter a number between 1 and 100.\n");
            }
        }
    }
}

void handle_processing(enum State *current_state, int *data) {
    if (*data < 1 || *data > 100) {
        *current_state = ERROR;
        return;
    }
    
    int result = *data * *data;
    if (result / *data != *data) {
        *current_state = ERROR;
        return;
    }
    
    printf("PROCESSING: %d squared is %d\n", *data, result);
    *current_state = COMPLETE;
}

void handle_complete(enum State *current_state, int *data) {
    printf("COMPLETE: Operation finished successfully.\n");
    *current_state = IDLE;
    *data = 0;
}

void handle_error(enum State *current_state, int *data) {
    printf("ERROR: An error occurred during processing.\n");
    *current_state = IDLE;
    *data = 0;
}

int main(void) {
    enum State current_state = IDLE;
    int data = 0;
    
    StateHandler handlers[] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_complete,
        handle_error
    };
    
    while (current_state != COMPLETE) {
        if (current_state >= IDLE && current_state <= ERROR) {
            handlers[current_state](&current_state, &data);
        } else {
            current_state = ERROR;
        }
    }
    
    return 0;
}