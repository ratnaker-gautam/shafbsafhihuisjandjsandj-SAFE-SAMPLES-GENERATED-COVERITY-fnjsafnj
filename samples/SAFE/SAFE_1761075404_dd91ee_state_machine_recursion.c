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
        default:
            return ERROR;
    }
}

void handle_state(enum State current, int *count) {
    if (count == NULL) {
        return;
    }
    
    if (*count < 0 || *count > 1000) {
        return;
    }
    
    switch (current) {
        case START:
            printf("State: START\n");
            break;
        case PROCESSING:
            printf("State: PROCESSING (step %d)\n", *count);
            (*count)++;
            break;
        case FINISHED:
            printf("State: FINISHED\n");
            break;
        case ERROR:
            printf("State: ERROR\n");
            break;
    }
}

int main() {
    enum State current_state = START;
    int step_count = 0;
    char input_buffer[32];
    int user_input;
    
    printf("State Machine Demo\n");
    printf("Enter numbers (0=finish, 1=process, 2=recurse, other=error):\n");
    
    while (current_state != FINISHED && current_state != ERROR) {
        handle_state(current_state, &step_count);
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            current_state = ERROR;
            continue;
        }
        
        if (strlen(input_buffer) == 0 || input_buffer[0] == '\n') {
            continue;
        }
        
        if (sscanf(input_buffer, "%d", &user_input) != 1) {
            current_state = ERROR;
            continue;
        }
        
        current_state = process_input(user_input, 0);
        
        if (step_count > 50) {
            current_state = FINISHED;
            printf("Maximum steps reached.\n");
        }
    }
    
    handle_state(current_state, &step_count);
    return 0;
}