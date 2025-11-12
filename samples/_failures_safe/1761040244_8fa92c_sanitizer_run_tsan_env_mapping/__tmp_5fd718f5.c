//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: state_machine
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
    int valid_inputs = 0;
    
    printf("Enter state commands (S,P,F,E,R): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    for (size_t i = 0; i < len; i++) {
        char cmd = buffer[i];
        if (cmd != 'S' && cmd != 'P' && cmd != 'F' && cmd != 'E' && cmd != 'R') {
            printf("Invalid command: %c\n", cmd);
            continue;
        }
        
        valid_inputs++;
        enum State result = process_input(cmd, 0);
        print_state(result);
        
        if (result == FINISHED) {
            printf("Sequence completed successfully\n");
            break;
        }
    }
    
    if (valid_inputs == 0) {
        printf("No valid commands processed\n");
        return 1;
    }
    
    return 0;
}