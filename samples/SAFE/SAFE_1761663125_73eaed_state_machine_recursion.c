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
                return process_input('P', depth + 1);
            }
            return ERROR;
        default:
            return ERROR;
    }
}

void execute_state(enum State current, int *counter) {
    if (counter == NULL) return;
    if (*counter < 0 || *counter > 1000) return;
    
    switch (current) {
        case START:
            if (*counter < 1000) (*counter)++;
            printf("State: START - Counter: %d\n", *counter);
            break;
        case PROCESSING:
            if (*counter > 0) (*counter)--;
            printf("State: PROCESSING - Counter: %d\n", *counter);
            break;
        case FINISHED:
            printf("State: FINISHED - Final counter: %d\n", *counter);
            break;
        case ERROR:
            printf("State: ERROR\n");
            break;
    }
}

int main() {
    char buffer[1024];
    int counter = 0;
    enum State current = START;
    
    printf("Enter state sequence (S=START, P=PROCESSING, F=FINISHED, R=RECURSE): ");
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0 || len >= sizeof(buffer)) {
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (buffer[i] != 'S' && buffer[i] != 'P' && buffer[i] != 'F' && buffer[i] != 'R') {
            current = ERROR;
            break;
        }
        
        current = process_input(buffer[i], 0);
        execute_state(current, &counter);
        
        if (current == FINISHED || current == ERROR) {
            break;
        }
    }
    
    return EXIT_SUCCESS;
}