//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, FINISHED, ERROR };

enum State process_input(const char* input, int depth) {
    if (depth > 100) return ERROR;
    
    if (input == NULL) return ERROR;
    
    size_t len = strlen(input);
    if (len == 0) return START;
    if (len > 1000) return ERROR;
    
    if (strcmp(input, "start") == 0) {
        return PROCESSING;
    } else if (strcmp(input, "process") == 0) {
        return PROCESSING;
    } else if (strcmp(input, "finish") == 0) {
        return FINISHED;
    } else if (strcmp(input, "reset") == 0) {
        return START;
    } else if (strcmp(input, "recursive") == 0) {
        char new_input[32];
        if (snprintf(new_input, sizeof(new_input), "process%d", depth) >= sizeof(new_input)) {
            return ERROR;
        }
        return process_input(new_input, depth + 1);
    } else {
        return ERROR;
    }
}

void print_state(enum State state) {
    switch (state) {
        case START:
            printf("State: START\n");
            break;
        case PROCESSING:
            printf("State: PROCESSING\n");
            break;
        case FINISHED:
            printf("State: FINISHED\n");
            break;
        case ERROR:
            printf("State: ERROR\n");
            break;
    }
}

int main(void) {
    char input[1024];
    enum State current_state = START;
    
    printf("State Machine Demo\n");
    printf("Available commands: start, process, finish, reset, recursive, quit\n");
    
    while (1) {
        printf("\nCurrent state: ");
        print_state(current_state);
        printf("Enter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input\n");
            continue;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) == 0) {
            continue;
        }
        
        if (strcmp(input, "quit") == 0) {
            printf("Exiting state machine\n");
            break;
        }
        
        current_state = process_input(input, 0);
        
        if (current_state == FINISHED) {
            printf("Process completed successfully!\n");
            break;
        }
    }
    
    return 0;
}