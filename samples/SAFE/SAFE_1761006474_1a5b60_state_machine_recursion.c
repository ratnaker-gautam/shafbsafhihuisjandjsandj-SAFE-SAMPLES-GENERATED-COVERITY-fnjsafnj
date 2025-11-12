//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, FINISHED, ERROR };

enum State process_input(char input, int depth) {
    if (depth > 100) {
        return ERROR;
    }
    
    switch (input) {
        case 'S':
            return START;
        case 'P':
            return PROCESSING;
        case 'F':
            return FINISHED;
        case 'E':
            return ERROR;
        default:
            return process_input('E', depth + 1);
    }
}

void execute_state(enum State current, int *counter, int depth) {
    if (depth > 50 || counter == NULL) {
        return;
    }
    
    if (*counter < 0 || *counter > 1000) {
        *counter = 0;
    }
    
    switch (current) {
        case START:
            if (*counter < 1000) {
                (*counter)++;
            }
            printf("State: START - Counter: %d\n", *counter);
            execute_state(PROCESSING, counter, depth + 1);
            break;
        case PROCESSING:
            if (*counter < 1000) {
                (*counter)++;
            }
            printf("State: PROCESSING - Counter: %d\n", *counter);
            execute_state(FINISHED, counter, depth + 1);
            break;
        case FINISHED:
            if (*counter < 1000) {
                (*counter)++;
            }
            printf("State: FINISHED - Counter: %d\n", *counter);
            break;
        case ERROR:
            printf("State: ERROR\n");
            break;
    }
}

int main(void) {
    char input_sequence[] = "SPFE";
    size_t input_len = strlen(input_sequence);
    int counter = 0;
    
    if (input_len > 100) {
        printf("Input sequence too long\n");
        return 1;
    }
    
    printf("Processing state sequence: %s\n", input_sequence);
    
    for (size_t i = 0; i < input_len; i++) {
        enum State next_state = process_input(input_sequence[i], 0);
        execute_state(next_state, &counter, 0);
        
        if (next_state == ERROR) {
            printf("Invalid state transition detected\n");
            break;
        }
    }
    
    printf("Final counter value: %d\n", counter);
    return 0;
}