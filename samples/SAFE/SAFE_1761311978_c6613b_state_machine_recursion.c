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

enum State state_machine(char* sequence, int pos, int depth) {
    if (depth > 100) return ERROR;
    if (sequence == NULL) return ERROR;
    if (pos < 0) return ERROR;
    
    size_t len = strlen(sequence);
    if (pos >= len) return FINISHED;
    
    char current = sequence[pos];
    if (current == '\0') return FINISHED;
    
    enum State next_state = process_input(current, depth + 1);
    
    if (next_state == ERROR) return ERROR;
    if (next_state == FINISHED) return FINISHED;
    
    return state_machine(sequence, pos + 1, depth + 1);
}

int main(void) {
    char input_buffer[1024];
    
    printf("Enter state sequence (B=Start, R=Read, P=Process, F=Finish, E=Error): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (len == 1 && input_buffer[0] == '\0') {
        printf("Empty input sequence\n");
        return 1;
    }
    
    for (size_t i = 0; i < strlen(input_buffer); i++) {
        char c = input_buffer[i];
        if (c != 'B' && c != 'R' && c != 'P' && c != 'F' && c != 'E') {
            printf("Invalid character in sequence: %c\n", c);
            return 1;
        }
    }
    
    enum State result = state_machine(input_buffer, 0, 0);
    
    switch (result) {
        case START:
            printf("Result: Start state\n");
            break;
        case READING:
            printf("Result: Reading state\n");
            break;
        case PROCESSING:
            printf("Result: Processing state\n");
            break;
        case FINISHED:
            printf("Result: Finished successfully\n");
            break;
        case ERROR:
            printf("Result: Error state\n");
            break;
    }
    
    return 0;
}