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
    
    switch (current) {
        case START:
            if (next == READING || next == FINISHED) {
                return state_machine(next, sequence, pos + 1, depth + 1);
            }
            return ERROR;
        case READING:
            if (next == PROCESSING || next == FINISHED) {
                return state_machine(next, sequence, pos + 1, depth + 1);
            }
            return ERROR;
        case PROCESSING:
            if (next == READING || next == FINISHED) {
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
            printf("START");
            break;
        case READING:
            printf("READING");
            break;
        case PROCESSING:
            printf("PROCESSING");
            break;
        case FINISHED:
            printf("FINISHED");
            break;
        case ERROR:
            printf("ERROR");
            break;
    }
}

int main(void) {
    char input_sequence[256];
    
    printf("Enter state sequence (B=START, R=READING, P=PROCESSING, F=FINISHED, E=ERROR): ");
    
    if (fgets(input_sequence, sizeof(input_sequence), stdin) == NULL) {
        printf("ERROR\n");
        return 1;
    }
    
    size_t len = strlen(input_sequence);
    if (len > 0 && input_sequence[len - 1] == '\n') {
        input_sequence[len - 1] = '\0';
    }
    
    if (strlen(input_sequence) == 0) {
        printf("ERROR\n");
        return 1;
    }
    
    for (size_t i = 0; i < strlen(input_sequence); i++) {
        char c = input_sequence[i];
        if (c != 'B' && c != 'R' && c != 'P' && c != 'F' && c != 'E') {
            printf("ERROR\n");
            return 1;
        }
    }
    
    enum State result = state_machine(START, input_sequence, 0, 0);
    
    printf("Final state: ");
    print_state(result);
    printf("\n");
    
    return 0;
}