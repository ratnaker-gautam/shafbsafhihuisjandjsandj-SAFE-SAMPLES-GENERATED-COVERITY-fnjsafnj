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
    
    if (next == FINISHED) return FINISHED;
    if (next == ERROR) return ERROR;
    
    (*pos)++;
    return state_machine(next, input, pos, count);
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len > 1000) return 0;
    return 1;
}

int main(void) {
    char buffer[1001];
    
    printf("Enter text (max 1000 chars): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
    }
    
    if (!validate_input(buffer)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int position = 0;
    int lowercase_count = 0;
    
    enum State result = state_machine(START, buffer, &position, &lowercase_count);
    
    if (result == ERROR) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    printf("Processed %d characters\n", position);
    printf("Found %d lowercase letters\n", lowercase_count);
    
    return 0;
}