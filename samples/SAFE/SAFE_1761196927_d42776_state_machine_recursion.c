//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, FINISHED, ERROR };

enum State process_input(char input, int depth) {
    if (depth > 100) {
        return ERROR;
    }
    
    switch (input) {
        case 'S':
            return START;
        case 'P':
            return PROCESSING;
        case 'F':
            return FINISHED;
        case 'E':
            return ERROR;
        default:
            return process_input('E', depth + 1);
    }
}

void execute_state(enum State current, int *count, int depth) {
    if (depth > 50 || count == NULL) {
        return;
    }
    
    if (*count < 0 || *count > 1000) {
        *count = 0;
        return;
    }
    
    switch (current) {
        case START:
            if (*count < 1000) {
                (*count)++;
            }
            execute_state(PROCESSING, count, depth + 1);
            break;
        case PROCESSING:
            if (*count > 0 && *count < 1000) {
                (*count) *= 2;
                if (*count > 1000) {
                    *count = 1000;
                }
            }
            execute_state(FINISHED, count, depth + 1);
            break;
        case FINISHED:
            printf("Final count: %d\n", *count);
            break;
        case ERROR:
            printf("Error state reached\n");
            *count = 0;
            break;
    }
}

int main(void) {
    char input_sequence[] = "SPF";
    size_t seq_len = strlen(input_sequence);
    
    if (seq_len == 0 || seq_len > 100) {
        printf("Invalid input sequence\n");
        return 1;
    }
    
    int count = 0;
    enum State current_state = START;
    
    for (size_t i = 0; i < seq_len; i++) {
        if (i >= 100) {
            break;
        }
        
        char current_char = input_sequence[i];
        if (current_char < 32 || current_char > 126) {
            current_state = ERROR;
            break;
        }
        
        current_state = process_input(current_char, 0);
        
        if (current_state == ERROR) {
            break;
        }
        
        execute_state(current_state, &count, 0);
        
        if (count < 0 || count > 1000) {
            count = 0;
            current_state = ERROR;
            break;
        }
    }
    
    if (current_state != FINISHED && current_state != ERROR) {
        printf("Sequence incomplete\n");
    }
    
    return 0;
}