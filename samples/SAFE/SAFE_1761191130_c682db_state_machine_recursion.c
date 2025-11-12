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
            if (depth > 0) {
                return process_input('P', depth + 1);
            }
            return ERROR;
        default:
            return ERROR;
    }
}

void execute_state(enum State current, int *counter) {
    if (counter == NULL) return;
    if (*counter < 0 || *counter > 1000) return;
    
    switch (current) {
        case START:
            if (*counter < 1000) (*counter)++;
            printf("State: START - Counter: %d\n", *counter);
            break;
        case PROCESSING:
            if (*counter > 0) (*counter)--;
            printf("State: PROCESSING - Counter: %d\n", *counter);
            break;
        case FINISHED:
            printf("State: FINISHED - Final counter: %d\n", *counter);
            break;
        case ERROR:
            printf("State: ERROR\n");
            break;
    }
}

enum State handle_transition(enum State current, char input, int depth, int *counter) {
    if (depth > 100) return ERROR;
    if (counter == NULL) return ERROR;
    
    enum State next = process_input(input, depth + 1);
    
    if (next == ERROR) {
        execute_state(ERROR, counter);
        return ERROR;
    }
    
    if (current == FINISHED && next != START) {
        return ERROR;
    }
    
    if (current == ERROR) {
        return ERROR;
    }
    
    execute_state(next, counter);
    
    if (next == PROCESSING && depth < 5) {
        return handle_transition(next, 'R', depth + 1, counter);
    }
    
    return next;
}

int main(void) {
    char input_sequence[] = "SPRPRF";
    size_t seq_len = strlen(input_sequence);
    enum State current = START;
    int counter = 0;
    
    if (seq_len > 100) {
        printf("Input sequence too long\n");
        return 1;
    }
    
    printf("Starting state machine\n");
    
    for (size_t i = 0; i < seq_len; i++) {
        if (input_sequence[i] == '\0') break;
        
        current = handle_transition(current, input_sequence[i], 0, &counter);
        
        if (current == ERROR) {
            printf("State machine entered error state\n");
            break;
        }
        
        if (counter < 0 || counter > 1000) {
            printf("Counter out of bounds\n");
            return 1;
        }
    }
    
    if (current != FINISHED && current != ERROR) {
        printf("State machine did not reach terminal state\n");
    }
    
    return 0;
}