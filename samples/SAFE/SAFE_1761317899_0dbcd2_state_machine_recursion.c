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
            if (depth > 0) {
                char next;
                if (scanf(" %c", &next) != 1) return ERROR;
                return process_input(next, depth + 1);
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

int main() {
    char buffer[1024];
    printf("Enter state sequence (S/P/F/R): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("ERROR\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
        len--;
    }
    
    if (len == 0 || len > 100) {
        printf("ERROR\n");
        return 1;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (buffer[i] != 'S' && buffer[i] != 'P' && buffer[i] != 'F' && buffer[i] != 'R') {
            printf("ERROR\n");
            return 1;
        }
    }
    
    enum State current = START;
    size_t index = 0;
    
    while (index < len) {
        current = process_input(buffer[index], 0);
        if (current == ERROR) {
            printf("ERROR\n");
            return 1;
        }
        index++;
    }
    
    print_state(current);
    return 0;
}