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
            if (depth < 10) {
                return process_input('P', depth + 1);
            }
            return ERROR;
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
    
    printf("Enter state commands (S,P,F,E,R): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("ERROR\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("ERROR\n");
        return 1;
    }
    
    for (size_t i = 0; i < len; i++) {
        char cmd = buffer[i];
        
        if (cmd != 'S' && cmd != 'P' && cmd != 'F' && cmd != 'E' && cmd != 'R') {
            printf("ERROR\n");
            return 1;
        }
        
        enum State result = process_input(cmd, 0);
        print_state(result);
        
        if (result != ERROR) {
            valid_inputs++;
        }
    }
    
    printf("Valid transitions: %d\n", valid_inputs);
    
    return 0;
}