//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PROCESSING, FINISHED, ERROR };

enum State process_char(char c, int *count) {
    if (c == '\0') return FINISHED;
    if (c < 32 || c > 126) return ERROR;
    
    if (c >= '0' && c <= '9') {
        if (*count < 100) (*count)++;
        return PROCESSING;
    }
    
    return READING;
}

enum State state_machine(enum State current, const char *input, int pos, int *count) {
    if (input == NULL || pos < 0 || count == NULL) return ERROR;
    if (pos >= 1000) return ERROR;
    
    switch (current) {
        case START:
            if (input[pos] == '\0') return FINISHED;
            return state_machine(READING, input, pos, count);
            
        case READING:
            return state_machine(process_char(input[pos], count), input, pos + 1, count);
            
        case PROCESSING:
            return state_machine(READING, input, pos + 1, count);
            
        case FINISHED:
            return FINISHED;
            
        case ERROR:
            return ERROR;
    }
    
    return ERROR;
}

void print_result(enum State final_state, int count) {
    switch (final_state) {
        case FINISHED:
            printf("Processing completed. Found %d digits.\n", count);
            break;
        case ERROR:
            printf("Error: Invalid input encountered.\n");
            break;
        default:
            printf("Unexpected state: %d\n", final_state);
    }
}

int main() {
    char input[1001];
    int count = 0;
    
    printf("Enter a string (max 1000 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= sizeof(input)) {
        printf("Error: Input too long.\n");
        return 1;
    }
    
    enum State result = state_machine(START, input, 0, &count);
    print_result(result, count);
    
    return 0;
}