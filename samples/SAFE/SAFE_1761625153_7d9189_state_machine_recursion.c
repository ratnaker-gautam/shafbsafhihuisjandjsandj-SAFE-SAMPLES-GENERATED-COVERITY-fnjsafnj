//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
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
            return process_input('S', depth + 1);
        case 'F':
            return FINISHED;
        default:
            return ERROR;
    }
}

void execute_state(enum State current, int step) {
    if (step > 50) {
        printf("Maximum recursion depth reached\n");
        return;
    }
    
    switch (current) {
        case START:
            printf("State: START - Step %d\n", step);
            execute_state(PROCESSING, step + 1);
            break;
        case PROCESSING:
            printf("State: PROCESSING - Step %d\n", step);
            execute_state(FINISHED, step + 1);
            break;
        case FINISHED:
            printf("State: FINISHED - Step %d\n", step);
            break;
        case ERROR:
            printf("State: ERROR - Step %d\n", step);
            break;
    }
}

int main(void) {
    char input_sequence[] = "SPF";
    size_t seq_len = strlen(input_sequence);
    
    if (seq_len == 0 || seq_len > 100) {
        printf("Invalid input sequence length\n");
        return 1;
    }
    
    printf("Processing input sequence: %s\n", input_sequence);
    
    enum State current_state = START;
    
    for (size_t i = 0; i < seq_len; i++) {
        if (input_sequence[i] == '\0') {
            break;
        }
        
        current_state = process_input(input_sequence[i], 0);
        
        if (current_state == ERROR) {
            printf("Invalid input character at position %zu\n", i);
            break;
        }
    }
    
    printf("Final state transition:\n");
    execute_state(current_state, 1);
    
    return 0;
}