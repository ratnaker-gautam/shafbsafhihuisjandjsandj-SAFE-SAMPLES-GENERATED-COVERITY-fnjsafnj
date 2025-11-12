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
    if (depth > 100) {
        return ERROR;
    }
    
    enum State next = process_input(input, depth + 1);
    
    switch (current) {
        case START:
            if (next == PROCESSING || next == FINISHED) {
                return next;
            }
            break;
        case PROCESSING:
            if (next == FINISHED || next == ERROR) {
                return next;
            }
            break;
        case FINISHED:
            if (next == START) {
                return next;
            }
            break;
        case ERROR:
            return ERROR;
    }
    
    return ERROR;
}

void print_state(enum State s) {
    switch (s) {
        case START: printf("START"); break;
        case PROCESSING: printf("PROCESSING"); break;
        case FINISHED: printf("FINISHED"); break;
        case ERROR: printf("ERROR"); break;
    }
}

enum State simulate_state_machine(const char* sequence, int index, enum State current, int depth) {
    if (depth > 100 || sequence == NULL) {
        return ERROR;
    }
    
    size_t len = strlen(sequence);
    if (index < 0 || index >= (int)len) {
        return current;
    }
    
    char input = sequence[index];
    if (input == '\0') {
        return current;
    }
    
    enum State next = state_transition(current, input, depth + 1);
    
    printf("Step %d: Input '%c' -> ", index, input);
    print_state(current);
    printf(" -> ");
    print_state(next);
    printf("\n");
    
    if (index + 1 < (int)len) {
        return simulate_state_machine(sequence, index + 1, next, depth + 1);
    }
    
    return next;
}

int main(void) {
    char input_sequence[256];
    
    printf("Enter state sequence (S=START, P=PROCESSING, F=FINISHED, E=ERROR): ");
    
    if (fgets(input_sequence, sizeof(input_sequence), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_sequence);
    if (len > 0 && input_sequence[len - 1] == '\n') {
        input_sequence[len - 1] = '\0';
    }
    
    if (strlen(input_sequence) == 0) {
        printf("Empty input sequence\n");
        return 1;
    }
    
    for (size_t i = 0; i < strlen(input_sequence); i++) {
        char c = input_sequence[i];
        if (c != 'S' && c != 'P' && c != 'F' && c != 'E') {
            printf("Invalid character in sequence: '%c'\n", c);
            return 1;
        }
    }
    
    printf("State machine simulation:\n");
    enum State final_state = simulate_state_machine(input_sequence, 0, START, 0);
    
    printf("Final state: ");
    print_state(final_state);
    printf("\n");
    
    return 0;
}