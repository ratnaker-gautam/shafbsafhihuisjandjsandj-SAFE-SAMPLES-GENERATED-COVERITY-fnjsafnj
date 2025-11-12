//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, FINISHED, ERROR };

enum State process_input(int input, int depth) {
    if (depth > 100) {
        return ERROR;
    }
    
    switch (input) {
        case 0:
            return FINISHED;
        case 1:
            return PROCESSING;
        case 2:
            return process_input(1, depth + 1);
        case 3:
            return process_input(0, depth + 1);
        default:
            return ERROR;
    }
}

void print_state(enum State s) {
    switch (s) {
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
    char input_buffer[32];
    int input_value;
    
    printf("Enter state machine inputs (0-3, negative to quit):\n");
    printf("0: Finish, 1: Process, 2: Recursive Process, 3: Recursive Finish\n");
    
    while (1) {
        printf("Input: ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        if (strlen(input_buffer) == 0 || input_buffer[0] == '\n') {
            continue;
        }
        
        char *endptr;
        input_value = strtol(input_buffer, &endptr, 10);
        
        if (endptr == input_buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (input_value < 0) {
            printf("Exiting state machine.\n");
            break;
        }
        
        if (input_value > 3 || input_value < 0) {
            printf("Input must be between 0 and 3.\n");
            continue;
        }
        
        enum State current_state = START;
        enum State result = process_input(input_value, 0);
        print_state(result);
    }
    
    return 0;
}