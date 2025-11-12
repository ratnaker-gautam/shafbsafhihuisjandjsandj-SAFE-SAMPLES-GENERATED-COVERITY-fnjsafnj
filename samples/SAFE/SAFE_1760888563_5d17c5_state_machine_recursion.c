//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    START,
    PROCESSING,
    VALIDATING,
    FINISHED,
    ERROR
};

enum State process_state(enum State current, int input, int depth) {
    if (depth > 100) {
        return ERROR;
    }
    
    switch (current) {
        case START:
            if (input >= 0 && input <= 100) {
                return process_state(PROCESSING, input, depth + 1);
            } else {
                return ERROR;
            }
            
        case PROCESSING:
            if (input % 2 == 0) {
                return process_state(VALIDATING, input * 2, depth + 1);
            } else {
                return process_state(VALIDATING, input * 3, depth + 1);
            }
            
        case VALIDATING:
            if (input > 0 && input < 500) {
                return process_state(FINISHED, input, depth + 1);
            } else {
                return ERROR;
            }
            
        case FINISHED:
            return FINISHED;
            
        case ERROR:
            return ERROR;
    }
    
    return ERROR;
}

void print_state(enum State s) {
    switch (s) {
        case START: printf("START"); break;
        case PROCESSING: printf("PROCESSING"); break;
        case VALIDATING: printf("VALIDATING"); break;
        case FINISHED: printf("FINISHED"); break;
        case ERROR: printf("ERROR"); break;
    }
}

int main() {
    char input_buffer[32];
    int value;
    
    printf("Enter a number between 0 and 100: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("ERROR: Failed to read input\n");
        return 1;
    }
    
    if (strlen(input_buffer) == 0 || input_buffer[0] == '\n') {
        printf("ERROR: Empty input\n");
        return 1;
    }
    
    char *endptr;
    value = (int)strtol(input_buffer, &endptr, 10);
    
    if (endptr == input_buffer || *endptr != '\n') {
        printf("ERROR: Invalid number format\n");
        return 1;
    }
    
    if (value < 0 || value > 100) {
        printf("ERROR: Number out of range\n");
        return 1;
    }
    
    enum State result = process_state(START, value, 0);
    
    printf("Input: %d\n", value);
    printf("Final state: ");
    print_state(result);
    printf("\n");
    
    if (result == FINISHED) {
        printf("Processing completed successfully\n");
    } else {
        printf("Processing failed\n");
    }
    
    return 0;
}