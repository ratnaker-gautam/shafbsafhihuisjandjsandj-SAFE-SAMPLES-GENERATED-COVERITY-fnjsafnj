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
        case 'E':
            return ERROR;
        case 'R':
            return process_input('P', depth + 1);
        default:
            return ERROR;
    }
}

void print_state(enum State s) {
    switch (s) {
        case START: printf("START\n"); break;
        case PROCESSING: printf("PROCESSING\n"); break;
        case FINISHED: printf("FINISHED\n"); break;
        case ERROR: printf("ERROR\n"); break;
    }
}

int main(void) {
    char buffer[1024];
    int valid_inputs = 0;
    
    printf("Enter state transitions (S=START, P=PROCESSING, F=FINISHED, E=ERROR, R=RECURSE):\n");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("ERROR\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("ERROR\n");
        return 1;
    }
    
    enum State current_state = START;
    
    for (size_t i = 0; i < len; i++) {
        if (buffer[i] != 'S' && buffer[i] != 'P' && buffer[i] != 'F' && 
            buffer[i] != 'E' && buffer[i] != 'R') {
            printf("ERROR\n");
            return 1;
        }
        valid_inputs++;
    }
    
    if (valid_inputs > 100) {
        printf("ERROR\n");
        return 1;
    }
    
    printf("State sequence:\n");
    print_state(current_state);
    
    for (size_t i = 0; i < len; i++) {
        current_state = process_input(buffer[i], 0);
        print_state(current_state);
        
        if (current_state == ERROR) {
            break;
        }
    }
    
    return 0;
}