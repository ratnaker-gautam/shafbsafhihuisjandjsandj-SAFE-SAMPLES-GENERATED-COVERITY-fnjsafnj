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
    int user_input;
    
    printf("Enter state machine inputs (0-3):\n");
    printf("0: Finish, 1: Process, 2: Recursive Process, 3: Recursive Finish\n");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%d", &user_input) != 1) {
        printf("Invalid input format\n");
        return 1;
    }
    
    if (user_input < 0 || user_input > 3) {
        printf("Input out of range (0-3)\n");
        return 1;
    }
    
    enum State current_state = START;
    printf("Initial ");
    print_state(current_state);
    
    current_state = process_input(user_input, 0);
    
    printf("Final ");
    print_state(current_state);
    
    return 0;
}