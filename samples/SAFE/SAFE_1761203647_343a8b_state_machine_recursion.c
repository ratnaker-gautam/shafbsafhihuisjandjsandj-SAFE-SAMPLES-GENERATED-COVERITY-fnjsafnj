//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PROCESSING, FINISHED, ERROR };

enum State process_char(char c, int *count) {
    if (c == '\0') return FINISHED;
    if (c < 32 || c > 126) return ERROR;
    
    if (c >= 'a' && c <= 'z') {
        (*count)++;
        return PROCESSING;
    }
    
    return READING;
}

enum State state_machine(enum State current, const char *input, int *pos, int *count) {
    if (current == FINISHED || current == ERROR) return current;
    if (input == NULL || pos == NULL || count == NULL) return ERROR;
    if (*pos < 0) return ERROR;
    
    char c = input[*pos];
    enum State next = process_char(c, count);
    
    if (next == FINISHED || next == ERROR) return next;
    
    (*pos)++;
    if (input[*pos] == '\0') return FINISHED;
    
    return state_machine(next, input, pos, count);
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 1000) return 0;
    return 1;
}

int main(void) {
    char input[1001];
    printf("Enter a string (max 1000 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        printf("Invalid input\n");
        return 1;
    }
    
    int pos = 0;
    int lowercase_count = 0;
    enum State result = state_machine(START, input, &pos, &lowercase_count);
    
    switch (result) {
        case FINISHED:
            printf("Processing completed. Found %d lowercase letters.\n", lowercase_count);
            break;
        case ERROR:
            printf("Error: Invalid character encountered.\n");
            break;
        default:
            printf("Unexpected state: %d\n", result);
            return 1;
    }
    
    return 0;
}