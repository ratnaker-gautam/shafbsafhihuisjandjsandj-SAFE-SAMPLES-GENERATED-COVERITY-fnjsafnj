//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PROCESSING, FINISHED, ERROR };

enum State process_input(char input, int depth) {
    if (depth > 100) return ERROR;
    
    switch (input) {
        case 'B':
            return START;
        case 'R':
            return READING;
        case 'P':
            return PROCESSING;
        case 'F':
            return FINISHED;
        case 'E':
            return ERROR;
        default:
            return ERROR;
    }
}

enum State state_transition(enum State current, char input, int depth) {
    if (depth >= 100) return ERROR;
    
    enum State next = process_input(input, depth + 1);
    
    switch (current) {
        case START:
            if (next == READING || next == FINISHED) return next;
            break;
        case READING:
            if (next == PROCESSING || next == ERROR) return next;
            break;
        case PROCESSING:
            if (next == FINISHED || next == READING) return next;
            break;
        case FINISHED:
            if (next == START || next == ERROR) return next;
            break;
        case ERROR:
            return ERROR;
    }
    
    return ERROR;
}

void execute_state_action(enum State state, int *counter) {
    if (counter == NULL) return;
    
    switch (state) {
        case START:
            printf("State: START - Initializing\n");
            *counter = 0;
            break;
        case READING:
            printf("State: READING - Collecting data\n");
            (*counter)++;
            break;
        case PROCESSING:
            printf("State: PROCESSING - Analyzing data\n");
            (*counter)++;
            break;
        case FINISHED:
            printf("State: FINISHED - Completed %d operations\n", *counter);
            break;
        case ERROR:
            printf("State: ERROR - Invalid transition\n");
            break;
    }
}

enum State run_state_machine(enum State current, const char *inputs, int index, int depth, int *counter) {
    if (depth > 100 || inputs == NULL || counter == NULL) return ERROR;
    if (index < 0) return ERROR;
    
    size_t len = strlen(inputs);
    if (index >= (int)len) return current;
    
    char input = inputs[index];
    if (input == '\0') return current;
    
    enum State next = state_transition(current, input, depth + 1);
    execute_state_action(next, counter);
    
    if (next == ERROR) return ERROR;
    
    return run_state_machine(next, inputs, index + 1, depth + 1, counter);
}

int main(void) {
    char input_buffer[1024];
    int counter = 0;
    
    printf("Enter state sequence (B=Start, R=Read, P=Process, F=Finish, E=Error): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (len == 1) {
        printf("Empty input sequence\n");
        return EXIT_FAILURE;
    }
    
    enum State final_state = run_state_machine(START, input_buffer, 0, 0, &counter);
    
    if (final_state == FINISHED) {
        printf("State machine completed successfully\n");
    } else {
        printf("State machine terminated in state: %d\n", final_state);
    }
    
    return EXIT_SUCCESS;
}