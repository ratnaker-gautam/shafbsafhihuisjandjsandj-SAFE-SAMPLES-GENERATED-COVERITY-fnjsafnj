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
    enum State current_state = START;
    
    printf("Enter state commands (S=START, P=PROCESSING, F=FINISHED, R=RECURSE): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    if (len == 1) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    for (size_t i = 0; i < len - 1; i++) {
        if (buffer[i] != 'S' && buffer[i] != 'P' && buffer[i] != 'F' && buffer[i] != 'R') {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    printf("Initial ");
    print_state(current_state);
    
    for (size_t i = 0; i < len - 1; i++) {
        current_state = process_input(buffer[i], 0);
        printf("After '%c': ", buffer[i]);
        print_state(current_state);
        
        if (current_state == ERROR) {
            break;
        }
    }
    
    return 0;
}