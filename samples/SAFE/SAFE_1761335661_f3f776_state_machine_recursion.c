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

enum State simulate_state_machine(const char* sequence, int index, int depth) {
    if (depth > 100) return ERROR;
    if (sequence == NULL) return ERROR;
    if (index < 0) return ERROR;
    
    size_t len = strlen(sequence);
    if (index >= (int)len) return FINISHED;
    
    char current = sequence[index];
    if (current == '\0') return FINISHED;
    
    enum State next_state = process_input(current, depth + 1);
    
    if (next_state == ERROR) return ERROR;
    if (next_state == FINISHED) return FINISHED;
    
    return simulate_state_machine(sequence, index + 1, depth + 1);
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
    char input_buffer[1024];
    
    printf("Enter state sequence (B=START, R=READING, P=PROCESSING, F=FINISHED, E=ERROR): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("Empty input sequence\n");
        return 1;
    }
    
    if (strlen(input_buffer) > 1000) {
        printf("Input sequence too long\n");
        return 1;
    }
    
    for (size_t i = 0; i < strlen(input_buffer); i++) {
        char c = input_buffer[i];
        if (c != 'B' && c != 'R' && c != 'P' && c != 'F' && c != 'E') {
            printf("Invalid character in sequence: %c\n", c);
            return 1;
        }
    }
    
    enum State final_state = simulate_state_machine(input_buffer, 0, 0);
    print_state(final_state);
    
    return 0;
}