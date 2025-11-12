//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, FINISHED, ERROR };

enum State process_input(int input, int depth) {
    if (depth > 100) return ERROR;
    
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

void execute_state_machine(void) {
    enum State current_state = START;
    int input_value;
    int step_count = 0;
    
    printf("Enter state machine inputs (0-3, -1 to exit):\n");
    
    while (step_count < 1000) {
        printf("Current state: %d | Enter input: ", current_state);
        
        if (scanf("%d", &input_value) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            current_state = ERROR;
            break;
        }
        
        if (input_value == -1) {
            printf("Exiting state machine\n");
            break;
        }
        
        if (input_value < 0 || input_value > 3) {
            printf("Invalid input range\n");
            current_state = ERROR;
            break;
        }
        
        current_state = process_input(input_value, 0);
        step_count++;
        
        if (current_state == FINISHED) {
            printf("State machine completed successfully\n");
            break;
        } else if (current_state == ERROR) {
            printf("State machine encountered error\n");
            break;
        }
    }
    
    if (step_count >= 1000) {
        printf("State machine step limit exceeded\n");
    }
}

int main(void) {
    execute_state_machine();
    return 0;
}