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

void print_state(enum State s) {
    switch (s) {
        case START:
            printf("State: START\n");
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
    char buffer[256];
    int valid_inputs = 0;
    
    printf("Enter state transitions (S=START, P=PROCESSING, F=FINISHED, R=RECURSE):\n");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    enum State current_state = START;
    
    for (size_t i = 0; i < len; i++) {
        char input_char = buffer[i];
        
        if (input_char != 'S' && input_char != 'P' && input_char != 'F' && input_char != 'R') {
            fprintf(stderr, "Invalid input character: %c\n", input_char);
            return EXIT_FAILURE;
        }
        
        current_state = process_input(input_char, 1);
        print_state(current_state);
        
        if (current_state == ERROR) {
            fprintf(stderr, "Invalid state transition\n");
            return EXIT_FAILURE;
        }
        
        valid_inputs++;
    }
    
    if (valid_inputs > 0) {
        printf("Processed %d valid state transitions\n", valid_inputs);
    } else {
        fprintf(stderr, "No valid transitions processed\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}