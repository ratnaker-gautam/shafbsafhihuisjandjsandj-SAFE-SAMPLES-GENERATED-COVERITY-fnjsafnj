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
    
    printf("Enter state commands (S=Start, P=Processing, F=Finished, R=Recursive):\n");
    
    while (valid_inputs < 10) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
            len--;
        }
        
        if (len != 1) {
            printf("Invalid input length. Enter single character.\n");
            continue;
        }
        
        char input = buffer[0];
        if (input != 'S' && input != 'P' && input != 'F' && input != 'R') {
            printf("Invalid command. Use S, P, F, or R.\n");
            continue;
        }
        
        enum State result = process_input(input, 0);
        print_state(result);
        
        valid_inputs++;
        
        if (result == FINISHED) {
            printf("Terminating on FINISHED state.\n");
            break;
        }
        
        if (result == ERROR) {
            printf("Terminating on ERROR state.\n");
            break;
        }
    }
    
    printf("Program completed after %d valid inputs.\n", valid_inputs);
    return 0;
}