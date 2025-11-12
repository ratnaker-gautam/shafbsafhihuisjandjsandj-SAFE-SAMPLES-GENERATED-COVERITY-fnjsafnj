//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, FINISHED, ERROR };

enum State process_input(const char* input, int depth) {
    if (depth > 100) {
        return ERROR;
    }
    
    if (input == NULL) {
        return ERROR;
    }
    
    size_t len = strlen(input);
    if (len == 0 || len > 100) {
        return ERROR;
    }
    
    if (depth == 0) {
        return START;
    }
    
    if (strcmp(input, "start") == 0) {
        return PROCESSING;
    }
    
    if (strcmp(input, "process") == 0) {
        char next_input[101];
        if (snprintf(next_input, sizeof(next_input), "step%d", depth) >= (int)sizeof(next_input)) {
            return ERROR;
        }
        return process_input(next_input, depth - 1);
    }
    
    if (strcmp(input, "finish") == 0) {
        return FINISHED;
    }
    
    if (strncmp(input, "step", 4) == 0) {
        if (depth > 1) {
            return process_input("process", depth - 1);
        } else {
            return process_input("finish", depth - 1);
        }
    }
    
    return ERROR;
}

void print_state(enum State state) {
    switch (state) {
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

int main() {
    char input[101];
    
    printf("Enter command (start/process/finish): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        print_state(ERROR);
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0 || strlen(input) >= sizeof(input)) {
        print_state(ERROR);
        return 1;
    }
    
    enum State result = process_input(input, 5);
    print_state(result);
    
    return 0;
}