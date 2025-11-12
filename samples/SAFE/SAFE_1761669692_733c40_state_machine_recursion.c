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
    
    switch (current) {
        case START:
            printf("State: START\n");
            *counter += 1;
            execute_state(PROCESSING, counter);
            break;
        case PROCESSING:
            printf("State: PROCESSING\n");
            *counter += 1;
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
    int counter = 0;
    
    printf("Enter state input (0=finish, 1=process, 2=recurse): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return EXIT_FAILURE;
    }
    
    if (sscanf(input_buffer, "%d", &user_input) != 1) {
        printf("Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    if (user_input < 0 || user_input > 2) {
        printf("Input out of range\n");
        return EXIT_FAILURE;
    }
    
    enum State result = process_input(user_input, 0);
    execute_state(result, &counter);
    
    return EXIT_SUCCESS;
}