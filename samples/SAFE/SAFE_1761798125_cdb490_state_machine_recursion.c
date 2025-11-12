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
    char buffer[1024];
    enum State current_state = START;
    
    printf("Enter state commands (S=START, P=PROCESSING, F=FINISHED, R=RECURSE): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    for (size_t i = 0; i < len; i++) {
        current_state = process_input(buffer[i], 0);
        print_state(current_state);
        
        if (current_state == ERROR) {
            fprintf(stderr, "Invalid state transition\n");
            return 1;
        }
        
        if (current_state == FINISHED) {
            printf("State machine completed successfully\n");
            return 0;
        }
    }
    
    if (current_state != FINISHED) {
        printf("State machine did not reach FINISHED state\n");
    }
    
    return 0;
}