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

int main(void) {
    char input_buffer[1024];
    
    printf("Enter state sequence (B=Start, R=Read, P=Process, F=Finish, E=Error): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("Empty input sequence\n");
        return 1;
    }
    
    if (input_len > 1000) {
        printf("Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        char c = input_buffer[i];
        if (c != 'B' && c != 'R' && c != 'P' && c != 'F' && c != 'E') {
            printf("Invalid character in sequence: %c\n", c);
            return 1;
        }
    }
    
    enum State result = simulate_state_machine(input_buffer, 0, 0);
    
    switch (result) {
        case FINISHED:
            printf("State machine completed successfully\n");
            break;
        case ERROR:
            printf("State machine ended in error state\n");
            break;
        default:
            printf("State machine ended in unexpected state\n");
            break;
    }
    
    return 0;
}