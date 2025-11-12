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
    if (index >= len) return FINISHED;
    
    char current = sequence[index];
    if (current == '\0') return FINISHED;
    
    enum State next_state = process_input(current, depth + 1);
    
    if (next_state == ERROR) return ERROR;
    if (next_state == FINISHED) return FINISHED;
    
    return simulate_state_machine(sequence, index + 1, depth + 1);
}

int main(void) {
    char input_sequence[256];
    
    printf("Enter state sequence (B=Start, R=Read, P=Process, F=Finish, E=Error): ");
    
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
    
    enum State result = simulate_state_machine(input_sequence, 0, 0);
    
    switch (result) {
        case START:
            printf("Result: Machine in start state\n");
            break;
        case READING:
            printf("Result: Machine in reading state\n");
            break;
        case PROCESSING:
            printf("Result: Machine in processing state\n");
            break;
        case FINISHED:
            printf("Result: Machine finished successfully\n");
            break;
        case ERROR:
            printf("Result: Machine encountered error\n");
            break;
    }
    
    return 0;
}