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
    
    switch (current) {
        case START:
            if (next == READING || next == PROCESSING) {
                return state_machine(next, sequence, pos + 1, depth + 1);
            }
            return ERROR;
        case READING:
            if (next == PROCESSING || next == FINISHED) {
                return state_machine(next, sequence, pos + 1, depth + 1);
            }
            return ERROR;
        case PROCESSING:
            if (next == FINISHED || next == READING) {
                return state_machine(next, sequence, pos + 1, depth + 1);
            }
            return ERROR;
        case FINISHED:
            return FINISHED;
        case ERROR:
            return ERROR;
    }
    
    return ERROR;
}

void print_state(enum State state) {
    switch (state) {
        case START:
            printf("START\n");
            break;
        case READING:
            printf("READING\n");
            break;
        case PROCESSING:
            printf("PROCESSING\n");
            break;
        case FINISHED:
            printf("FINISHED\n");
            break;
        case ERROR:
            printf("ERROR\n");
            break;
    }
}

int main(void) {
    char input_sequence[256];
    
    printf("Enter state sequence (B=Start, R=Reading, P=Processing, F=Finished, E=Error): ");
    
    if (fgets(input_sequence, sizeof(input_sequence), stdin) == NULL) {
        printf("ERROR\n");
        return 1;
    }
    
    size_t len = strlen(input_sequence);
    if (len > 0 && input_sequence[len - 1] == '\n') {
        input_sequence[len - 1] = '\0';
    }
    
    if (len == 1 && input_sequence[0] == '\n') {
        printf("ERROR\n");
        return 1;
    }
    
    enum State result = state_machine(START, input_sequence, 0, 0);
    printf("Final state: ");
    print_state(result);
    
    return 0;
}