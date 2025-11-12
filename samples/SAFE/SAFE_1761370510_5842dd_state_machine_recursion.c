//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, FINISHED, ERROR };

enum State process_input(char input, int depth) {
    if (depth > 100) return ERROR;
    
    switch (input) {
        case 'S':
            return START;
        case 'P':
            return PROCESSING;
        case 'F':
            return FINISHED;
        case 'R':
            if (depth < 10) {
                return process_input('P', depth + 1);
            }
            return ERROR;
        default:
            return ERROR;
    }
}

void execute_state(enum State current, int *counter) {
    if (*counter > 1000) return;
    
    switch (current) {
        case START:
            printf("State: START\n");
            (*counter)++;
            break;
        case PROCESSING:
            printf("State: PROCESSING\n");
            (*counter)++;
            break;
        case FINISHED:
            printf("State: FINISHED\n");
            (*counter)++;
            return;
        case ERROR:
            printf("State: ERROR\n");
            (*counter)++;
            return;
    }
    
    if (current == PROCESSING && *counter % 3 == 0) {
        execute_state(FINISHED, counter);
    }
}

int main() {
    char input_sequence[] = "SPRPRF";
    size_t input_len = strlen(input_sequence);
    int counter = 0;
    
    if (input_len > 100) {
        printf("Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        enum State result = process_input(input_sequence[i], 0);
        execute_state(result, &counter);
        
        if (result == ERROR || counter > 1000) {
            printf("Terminating due to error or recursion limit\n");
            break;
        }
        
        if (result == FINISHED) {
            break;
        }
    }
    
    return 0;
}