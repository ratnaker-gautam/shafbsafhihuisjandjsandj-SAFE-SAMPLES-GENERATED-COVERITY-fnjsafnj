//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    IDLE,
    READING,
    PROCESSING,
    WRITING,
    ERROR,
    EXIT
};

typedef void (*StateHandler)(enum State *current_state, int *data);

void idle_handler(enum State *current_state, int *data) {
    char input[32];
    printf("Enter command (read, exit): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        *current_state = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "read") == 0) {
        *current_state = READING;
    } else if (strcmp(input, "exit") == 0) {
        *current_state = EXIT;
    } else {
        printf("Invalid command\n");
    }
}

void reading_handler(enum State *current_state, int *data) {
    char buffer[32];
    printf("Enter a number: ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        *current_state = ERROR;
        return;
    }
    
    char *endptr;
    long value = strtol(buffer, &endptr, 10);
    
    if (endptr == buffer || *endptr != '\n') {
        printf("Invalid number\n");
        *current_state = IDLE;
        return;
    }
    
    if (value < -1000 || value > 1000) {
        printf("Number out of range\n");
        *current_state = IDLE;
        return;
    }
    
    *data = (int)value;
    *current_state = PROCESSING;
}

void processing_handler(enum State *current_state, int *data) {
    if (*data < 0) {
        *data = -*data;
    }
    
    *data = (*data) * 2;
    
    if (*data > 10000) {
        *current_state = ERROR;
        return;
    }
    
    *current_state = WRITING;
}

void writing_handler(enum State *current_state, int *data) {
    printf("Processed result: %d\n", *data);
    *current_state = IDLE;
}

void error_handler(enum State *current_state, int *data) {
    printf("Error occurred in state machine\n");
    *current_state = EXIT;
}

int main() {
    enum State current_state = IDLE;
    int data = 0;
    
    StateHandler handlers[] = {
        idle_handler,
        reading_handler,
        processing_handler,
        writing_handler,
        error_handler,
        NULL
    };
    
    while (current_state != EXIT) {
        if (current_state < 0 || current_state >= EXIT) {
            current_state = ERROR;
        }
        
        StateHandler handler = handlers[current_state];
        if (handler != NULL) {
            handler(&current_state, &data);
        } else {
            current_state = ERROR;
        }
    }
    
    printf("State machine terminated\n");
    return 0;
}