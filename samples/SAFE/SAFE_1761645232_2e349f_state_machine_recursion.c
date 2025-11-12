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
            break;
        case ERROR:
            printf("State: ERROR\n");
            break;
    }
}

int main(void) {
    char input_buffer[32];
    int valid_inputs[] = {0, 1, 2};
    int input_count = sizeof(valid_inputs) / sizeof(valid_inputs[0]);
    int user_input;
    
    printf("Enter input (0=Finish, 1=Process, 2=Recurse): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%d", &user_input) != 1) {
        printf("Invalid input format\n");
        return 1;
    }
    
    int valid = 0;
    for (int i = 0; i < input_count; i++) {
        if (user_input == valid_inputs[i]) {
            valid = 1;
            break;
        }
    }
    
    if (!valid) {
        printf("Invalid input value\n");
        return 1;
    }
    
    int counter = 0;
    enum State initial_state = process_input(user_input, 0);
    execute_state(initial_state, &counter);
    
    return 0;
}