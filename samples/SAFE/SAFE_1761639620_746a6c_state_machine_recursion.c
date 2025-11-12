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
    
    if (depth == 0) {
        for (size_t i = 0; i < len; i++) {
            if (input[i] < 32 || input[i] > 126) return ERROR;
        }
    }
    
    if (len == 1) {
        if (input[0] == 'D') return FINISHED;
        return PROCESSING;
    }
    
    size_t mid = len / 2;
    char left[1001];
    char right[1001];
    
    if (mid >= sizeof(left) || (len - mid) >= sizeof(right)) return ERROR;
    
    strncpy(left, input, mid);
    left[mid] = '\0';
    strncpy(right, input + mid, len - mid);
    right[len - mid] = '\0';
    
    enum State left_state = process_input(left, depth + 1);
    enum State right_state = process_input(right, depth + 1);
    
    if (left_state == ERROR || right_state == ERROR) return ERROR;
    if (left_state == FINISHED && right_state == FINISHED) return FINISHED;
    if (left_state == FINISHED || right_state == FINISHED) return PROCESSING;
    return START;
}

const char* state_to_string(enum State s) {
    switch (s) {
        case START: return "START";
        case PROCESSING: return "PROCESSING";
        case FINISHED: return "FINISHED";
        case ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[1001];
    
    printf("Enter input string (max 1000 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("ERROR: Failed to read input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("STATE: START\n");
        return 0;
    }
    
    if (len > 1000) {
        printf("STATE: ERROR\n");
        return 1;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) {
            printf("STATE: ERROR\n");
            return 1;
        }
    }
    
    enum State result = process_input(input, 0);
    printf("STATE: %s\n", state_to_string(result));
    
    return result == ERROR ? 1 : 0;
}