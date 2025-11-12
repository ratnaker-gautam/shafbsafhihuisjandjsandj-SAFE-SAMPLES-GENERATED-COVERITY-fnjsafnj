//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { IDLE, READY, PROCESSING, COMPLETE, ERROR };

typedef void (*StateHandler)(enum State *current_state, int *data);

void handle_idle(enum State *current_state, int *data) {
    char input[16];
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
    char input[16];
    printf("READY: Enter number (1-100) to process or 'back': ");
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "back") == 0) {
            *current_state = IDLE;
        } else {
            int num = atoi(input);
            if (num >= 1 && num <= 100) {
                *data = num;
                *current_state = PROCESSING;
            } else {
                printf("Invalid number. Must be 1-100.\n");
            }
        }
    }
}

void handle_processing(enum State *current_state, int *data) {
    if (*data < 1 || *data > 100) {
        *current_state = ERROR;
        return;
    }
    
    int result = (*data) * (*data);
    if (result / (*data) != (*data)) {
        *current_state = ERROR;
        return;
    }
    
    printf("PROCESSING: %d squared is %d\n", *data, result);
    *data = result;
    *current_state = COMPLETE;
}

void handle_complete(enum State *current_state, int *data) {
    printf("COMPLETE: Final result is %d\n", *data);
    printf("Enter 'restart' to go to IDLE or 'quit' to exit: ");
    char input[16];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "restart") == 0) {
            *current_state = IDLE;
            *data = 0;
        } else if (strcmp(input, "quit") == 0) {
            *current_state = COMPLETE;
        }
    }
}

void handle_error(enum State *current_state, int *data) {
    printf("ERROR: Invalid state detected.\n");
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
    
    printf("Program terminated.\n");
    return 0;
}