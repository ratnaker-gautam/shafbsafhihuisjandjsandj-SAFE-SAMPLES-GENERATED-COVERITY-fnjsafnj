//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PROCESSING, FINISHED, ERROR };

enum State process_input(char* input, int pos, int len) {
    if (pos >= len) return FINISHED;
    
    char c = input[pos];
    
    switch (c) {
        case 'B':
            return START;
        case 'R':
            return READING;
        case 'P':
            return PROCESSING;
        case 'E':
            return FINISHED;
        default:
            return ERROR;
    }
}

void execute_state(enum State current, char* input, int pos, int len) {
    if (pos >= len) return;
    
    switch (current) {
        case START:
            printf("State: START - Beginning processing\n");
            break;
        case READING:
            printf("State: READING - Reading input character '%c'\n", input[pos]);
            break;
        case PROCESSING:
            printf("State: PROCESSING - Processing character '%c'\n", input[pos]);
            break;
        case FINISHED:
            printf("State: FINISHED - Processing complete\n");
            return;
        case ERROR:
            printf("State: ERROR - Invalid input character '%c'\n", input[pos]);
            return;
    }
    
    enum State next = process_input(input, pos, len);
    execute_state(next, input, pos + 1, len);
}

int validate_input(char* input) {
    if (input == NULL) return 0;
    
    int len = strlen(input);
    if (len <= 0 || len > 100) return 0;
    
    for (int i = 0; i < len; i++) {
        char c = input[i];
        if (c != 'B' && c != 'R' && c != 'P' && c != 'E') {
            return 0;
        }
    }
    
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_string>\n", argv[0]);
        printf("Valid characters: B (Start), R (Reading), P (Processing), E (End)\n");
        return 1;
    }
    
    char* input = argv[1];
    
    if (!validate_input(input)) {
        printf("Error: Invalid input string\n");
        printf("Input must contain only characters: B, R, P, E\n");
        printf("Maximum length: 100 characters\n");
        return 1;
    }
    
    int len = strlen(input);
    printf("Processing input: %s\n", input);
    printf("Input length: %d\n", len);
    
    execute_state(START, input, 0, len);
    
    return 0;
}