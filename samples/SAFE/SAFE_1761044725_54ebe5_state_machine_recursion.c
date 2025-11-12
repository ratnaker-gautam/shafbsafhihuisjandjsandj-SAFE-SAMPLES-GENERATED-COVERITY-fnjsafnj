//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: state_machine
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

enum State state_machine(enum State current, const char* sequence, int pos, int depth) {
    if (depth > 50) return ERROR;
    if (sequence == NULL) return ERROR;
    if (pos < 0) return ERROR;
    
    size_t len = strlen(sequence);
    if (pos >= (int)len) return current;
    
    char input = sequence[pos];
    if (input == '\0') return current;
    
    enum State next = process_input(input, depth + 1);
    
    if (next == ERROR) return ERROR;
    
    printf("State: %d, Input: %c, Next: %d\n", current, input, next);
    
    return state_machine(next, sequence, pos + 1, depth + 1);
}

int main(void) {
    char buffer[256];
    printf("Enter state sequence (B=Start, R=Reading, P=Processing, F=Finished, E=Error): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    if (strlen(buffer) == 0) {
        fprintf(stderr, "Empty input sequence\n");
        return EXIT_FAILURE;
    }
    
    enum State result = state_machine(START, buffer, 0, 0);
    
    printf("Final state: %d\n", result);
    
    if (result == FINISHED) {
        printf("State machine completed successfully\n");
        return EXIT_SUCCESS;
    } else {
        printf("State machine ended in non-finished state\n");
        return EXIT_FAILURE;
    }
}