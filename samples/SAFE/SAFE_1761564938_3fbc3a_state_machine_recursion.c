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
        default:
            return ERROR;
    }
}

void execute_state_machine(enum State current_state, int depth) {
    if (depth > 100) {
        printf("Error: Recursion depth exceeded\n");
        return;
    }
    
    switch (current_state) {
        case START:
            printf("State: START\n");
            execute_state_machine(PROCESSING, depth + 1);
            break;
        case PROCESSING:
            printf("State: PROCESSING\n");
            printf("Enter input (0=finish, 1=continue, 2=recurse): ");
            
            int user_input;
            if (scanf("%d", &user_input) != 1) {
                printf("Error: Invalid input\n");
                execute_state_machine(ERROR, depth + 1);
                return;
            }
            
            if (user_input < 0 || user_input > 2) {
                printf("Error: Input out of range\n");
                execute_state_machine(ERROR, depth + 1);
                return;
            }
            
            enum State next_state = process_input(user_input, 0);
            execute_state_machine(next_state, depth + 1);
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
    printf("Starting state machine...\n");
    execute_state_machine(START, 0);
    return 0;
}