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
        case 'N':
            return process_input('R', depth + 1);
        case 'X':
            return process_input('P', depth + 1);
        default:
            return ERROR;
    }
}

void print_state(enum State s) {
    switch (s) {
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
    char buffer[1024];
    enum State current = START;
    
    printf("Enter state transitions (B=Start, R=Read, P=Process, F=Finish, N=Next, X=ProcessNext): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    printf("Initial state: ");
    print_state(current);
    
    for (size_t i = 0; i < len; i++) {
        char input = buffer[i];
        if (input < 32 || input > 126) {
            fprintf(stderr, "Invalid character\n");
            return 1;
        }
        
        current = process_input(input, 0);
        printf("Input '%c': ", input);
        print_state(current);
        
        if (current == ERROR) {
            fprintf(stderr, "Invalid transition\n");
            return 1;
        }
        
        if (current == FINISHED) {
            printf("Reached final state\n");
            break;
        }
    }
    
    return 0;
}