//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: state_machine
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

void execute_state(enum State current, int *counter) {
    if (*counter > 100) {
        printf("State machine terminated: recursion limit\n");
        return;
    }
    
    (*counter)++;
    
    switch (current) {
        case START:
            printf("State: START\n");
            execute_state(PROCESSING, counter);
            break;
        case PROCESSING:
            printf("State: PROCESSING\n");
            execute_state(FINISHED, counter);
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
    int valid_input = 0;
    
    printf("Enter a number (0-3): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
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
    
    valid_input = 1;
    
    if (valid_input) {
        int counter = 0;
        enum State result = process_input(user_input, 0);
        execute_state(result, &counter);
    }
    
    return 0;
}