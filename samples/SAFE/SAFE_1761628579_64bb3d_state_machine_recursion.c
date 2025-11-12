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
    char buffer[32];
    int input;
    
    printf("Enter state machine inputs (0-3, negative to quit):\n");
    printf("0: Finish, 1: Process, 2: Recursive Process, 3: Recursive Finish\n");
    
    enum State current_state = START;
    print_state(current_state);
    
    while (1) {
        printf("Input: ");
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        if (buffer[0] == '\n') {
            continue;
        }
        
        char *endptr;
        long val = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (val < -1 || val > 3) {
            printf("Input out of range. Use -1 to quit, 0-3 for states.\n");
            continue;
        }
        
        if (val == -1) {
            break;
        }
        
        input = (int)val;
        
        if (input >= 0 && input <= 3) {
            current_state = process_input(input, 0);
            print_state(current_state);
            
            if (current_state == FINISHED || current_state == ERROR) {
                break;
            }
        }
    }
    
    printf("State machine terminated.\n");
    return 0;
}