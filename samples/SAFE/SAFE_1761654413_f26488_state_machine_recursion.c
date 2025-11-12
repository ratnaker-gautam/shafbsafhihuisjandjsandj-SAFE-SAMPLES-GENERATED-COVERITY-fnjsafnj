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

int get_valid_input(void) {
    char buffer[16];
    int value;
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return -1;
    }
    
    if (sscanf(buffer, "%d", &value) != 1) {
        return -1;
    }
    
    if (value < 0 || value > 9) {
        return -1;
    }
    
    return value;
}

int main(void) {
    enum State current_state = START;
    int input;
    
    printf("State Machine Demo\n");
    printf("Valid inputs: 0-9 (0=finish, 1=process, 2=recursive process, 3=recursive finish)\n");
    
    while (current_state != FINISHED && current_state != ERROR) {
        printf("Enter input: ");
        input = get_valid_input();
        
        if (input == -1) {
            printf("Invalid input. Please enter a number 0-9.\n");
            continue;
        }
        
        current_state = process_input(input, 0);
        print_state(current_state);
        
        if (current_state == ERROR) {
            printf("State machine entered error state.\n");
            break;
        }
    }
    
    if (current_state == FINISHED) {
        printf("State machine completed successfully.\n");
    }
    
    return 0;
}