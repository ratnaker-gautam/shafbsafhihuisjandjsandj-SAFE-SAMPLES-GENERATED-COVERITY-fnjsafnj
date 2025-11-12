//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PROCESSING, FINISHED, ERROR };

enum State process_input(char input, int depth) {
    if (depth > 100) {
        return ERROR;
    }
    
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
            return process_input('E', depth + 1);
    }
}

enum State state_machine_engine(const char* sequence, size_t index, enum State current, int depth) {
    if (depth > 100 || sequence == NULL || index >= strlen(sequence)) {
        return ERROR;
    }
    
    char input_char = sequence[index];
    if (input_char == '\0') {
        return current;
    }
    
    enum State next_state = process_input(input_char, depth + 1);
    if (next_state == ERROR) {
        return ERROR;
    }
    
    return state_machine_engine(sequence, index + 1, next_state, depth + 1);
}

enum State run_state_machine(const char* input_sequence) {
    if (input_sequence == NULL || strlen(input_sequence) > 1000) {
        return ERROR;
    }
    
    return state_machine_engine(input_sequence, 0, START, 0);
}

void print_state(enum State state) {
    switch (state) {
        case START:
            printf("State: START\n");
            break;
        case READING:
            printf("State: READING\n");
            break;
        case PROCESSING:
            printf("State: PROCESSING\n");
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
    char input_buffer[1002];
    
    printf("Enter state sequence (B=START, R=READING, P=PROCESSING, F=FINISHED, E=ERROR): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("Empty input sequence\n");
        return EXIT_FAILURE;
    }
    
    enum State final_state = run_state_machine(input_buffer);
    print_state(final_state);
    
    return (final_state == FINISHED) ? EXIT_SUCCESS : EXIT_FAILURE;
}