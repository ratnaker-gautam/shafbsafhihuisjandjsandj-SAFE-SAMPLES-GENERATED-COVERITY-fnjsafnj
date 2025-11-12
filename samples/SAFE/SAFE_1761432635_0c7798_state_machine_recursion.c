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

void execute_state(enum State current, int *counter) {
    if (*counter > 1000) {
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
            return;
        case ERROR:
            printf("State: ERROR\n");
            return;
    }
}

int main(void) {
    char input_buffer[32];
    int user_input;
    int valid_input = 0;
    
    printf("Enter state input (0-3): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    if (strlen(input_buffer) == 0 || input_buffer[0] == '\n') {
        printf("Empty input\n");
        return 1;
    }
    
    char *endptr;
    user_input = strtol(input_buffer, &endptr, 10);
    
    if (endptr == input_buffer || *endptr != '\n') {
        printf("Invalid input format\n");
        return 1;
    }
    
    if (user_input < 0 || user_input > 3) {
        printf("Input out of range\n");
        return 1;
    }
    
    enum State result = process_input(user_input, 0);
    
    int counter = 0;
    execute_state(result, &counter);
    
    return 0;
}