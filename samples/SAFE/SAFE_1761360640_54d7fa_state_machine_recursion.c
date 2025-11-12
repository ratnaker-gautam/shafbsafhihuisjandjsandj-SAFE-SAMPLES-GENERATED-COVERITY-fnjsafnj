//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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

void execute_state_machine(int initial_input) {
    enum State current_state = START;
    int input = initial_input;
    int step_count = 0;
    
    while (step_count < 1000) {
        switch (current_state) {
            case START:
                if (input >= 0 && input <= 3) {
                    current_state = process_input(input, 0);
                } else {
                    current_state = ERROR;
                }
                break;
                
            case PROCESSING:
                printf("Processing step %d\n", step_count);
                input = (input + 1) % 4;
                current_state = process_input(input, 0);
                break;
                
            case FINISHED:
                printf("State machine completed successfully\n");
                return;
                
            case ERROR:
                printf("State machine encountered an error\n");
                return;
        }
        
        step_count++;
        if (step_count >= 1000) {
            printf("State machine exceeded maximum steps\n");
            return;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <initial_input>\n", argv[0]);
        return 1;
    }
    
    char *endptr;
    long input_value = strtol(argv[1], &endptr, 10);
    
    if (*endptr != '\0') {
        printf("Error: Input must be a valid integer\n");
        return 1;
    }
    
    if (input_value < INT_MIN || input_value > INT_MAX) {
        printf("Error: Input value out of range\n");
        return 1;
    }
    
    int initial_input = (int)input_value;
    
    if (initial_input < 0 || initial_input > 3) {
        printf("Warning: Input should be between 0-3 for best results\n");
    }
    
    execute_state_machine(initial_input);
    
    return 0;
}