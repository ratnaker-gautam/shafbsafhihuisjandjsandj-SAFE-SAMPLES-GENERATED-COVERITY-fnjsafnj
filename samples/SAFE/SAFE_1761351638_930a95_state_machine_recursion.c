//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, PROCESSING, FINISHED, ERROR };

enum State process_input(const char* input, int depth) {
    if (depth > 100) return ERROR;
    
    if (input == NULL) return ERROR;
    
    size_t len = strlen(input);
    if (len == 0) return START;
    if (len > 1000) return ERROR;
    
    if (len == 1) {
        if (input[0] == 'D') return FINISHED;
        if (input[0] == 'E') return ERROR;
    }
    
    if (input[0] == 'P') {
        if (len > 1) {
            return process_input(input + 1, depth + 1);
        }
        return PROCESSING;
    }
    
    if (input[0] == 'R') {
        char* new_input = malloc(len);
        if (new_input == NULL) return ERROR;
        
        for (size_t i = 0; i < len - 1; i++) {
            new_input[i] = input[i + 1];
        }
        new_input[len - 1] = '\0';
        
        enum State result = process_input(new_input, depth + 1);
        free(new_input);
        return result;
    }
    
    return ERROR;
}

const char* state_to_string(enum State state) {
    switch (state) {
        case START: return "START";
        case PROCESSING: return "PROCESSING";
        case FINISHED: return "FINISHED";
        case ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input_buffer[1002];
    
    printf("Enter input sequence (max 1000 chars): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("ERROR: Failed to read input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("ERROR: Empty input\n");
        return 1;
    }
    
    enum State result = process_input(input_buffer, 0);
    
    printf("Final state: %s\n", state_to_string(result));
    
    return 0;
}