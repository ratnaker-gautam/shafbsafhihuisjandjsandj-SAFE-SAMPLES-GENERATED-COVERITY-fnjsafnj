//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { IDLE, READY, PROCESSING, COMPLETE, ERROR };

typedef void (*StateHandler)(enum State *current_state, int *data);

void handle_idle(enum State *current_state, int *data) {
    if (*current_state != IDLE) return;
    
    printf("IDLE: Enter command (start/exit): ");
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        *current_state = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "start") == 0) {
        *current_state = READY;
        *data = 0;
    } else if (strcmp(input, "exit") == 0) {
        *current_state = COMPLETE;
    } else {
        printf("Invalid command\n");
    }
}

void handle_ready(enum State *current_state, int *data) {
    if (*current_state != READY) return;
    
    printf("READY: Enter number to process (1-100): ");
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        *current_state = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    int num;
    if (sscanf(input, "%d", &num) != 1) {
        printf("Invalid number\n");
        return;
    }
    
    if (num < 1 || num > 100) {
        printf("Number must be between 1 and 100\n");
        return;
    }
    
    *data = num;
    *current_state = PROCESSING;
}

void handle_processing(enum State *current_state, int *data) {
    if (*current_state != PROCESSING) return;
    
    if (*data < 1 || *data > 100) {
        *current_state = ERROR;
        return;
    }
    
    int result = *data * 2;
    printf("PROCESSING: %d * 2 = %d\n", *data, result);
    *data = result;
    *current_state = COMPLETE;
}

void handle_complete(enum State *current_state, int *data) {
    if (*current_state != COMPLETE) return;
    
    printf("COMPLETE: Result is %d\n", *data);
    printf("Continue? (yes/no): ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        *current_state = ERROR;
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "yes") == 0) {
        *current_state = IDLE;
        *data = 0;
    } else if (strcmp(input, "no") == 0) {
        *current_state = COMPLETE;
    } else {
        printf("Invalid response\n");
    }
}

void handle_error(enum State *current_state, int *data) {
    if (*current_state != ERROR) return;
    
    printf("ERROR: An error occurred\n");
    printf("Reset? (yes/no): ");
    
    char input[16];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "yes") == 0) {
        *current_state = IDLE;
        *data = 0;
    } else {
        *current_state = COMPLETE;
    }
}

int main() {
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
        if (current_state < IDLE || current_state > ERROR) {
            current_state = ERROR;
        }
        
        handlers[current_state](&current_state, &data);
    }
    
    printf("Program terminated\n");
    return 0;
}