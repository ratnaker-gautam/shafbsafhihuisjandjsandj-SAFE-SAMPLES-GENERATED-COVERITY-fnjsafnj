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
    if (counter == NULL) {
        return;
    }
    
    if (*counter > 1000) {
        return;
    }
    
    (*counter)++;
    
    switch (current) {
        case START:
            printf("State: START\n");
            break;
        case PROCESSING:
            printf("State: PROCESSING\n");
            execute_state(PROCESSING, counter);
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
    int user_input;
    int counter = 0;
    
    printf("Enter input (0=Finish, 1=Process, 2=Recursive): ");
    
    if (scanf("%d", &user_input) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (user_input < 0 || user_input > 2) {
        printf("Input out of range\n");
        return 1;
    }
    
    enum State result = process_input(user_input, 0);
    execute_state(result, &counter);
    
    return 0;
}