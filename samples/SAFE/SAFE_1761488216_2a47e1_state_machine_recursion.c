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
    
    printf("Enter state commands (S/P/F/E/R), one per line. Empty line to finish:\n");
    
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            break;
        }
        
        if (len != 1) {
            printf("Invalid input length. Please enter single character commands.\n");
            continue;
        }
        
        char input = buffer[0];
        if (input != 'S' && input != 'P' && input != 'F' && input != 'E' && input != 'R') {
            printf("Invalid command. Valid commands: S, P, F, E, R\n");
            continue;
        }
        
        enum State result = process_input(input, 0);
        print_state(result);
        valid_inputs++;
        
        if (result == FINISHED || result == ERROR) {
            break;
        }
    }
    
    if (valid_inputs == 0) {
        printf("No valid inputs processed.\n");
    } else {
        printf("Processed %d valid inputs.\n", valid_inputs);
    }
    
    return 0;
}