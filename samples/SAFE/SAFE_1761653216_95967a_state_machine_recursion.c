//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { START, READING, PROCESSING, FINISHED, ERROR };

enum State process_input(char* data, int pos, int len) {
    if (pos >= len) return FINISHED;
    
    char c = data[pos];
    
    if (c == '\0') return FINISHED;
    
    if (pos == 0) {
        if (c == 'B') return READING;
        else return ERROR;
    }
    
    enum State prev_state = process_input(data, pos - 1, len);
    
    switch (prev_state) {
        case START:
            if (c == 'B') return READING;
            else return ERROR;
            
        case READING:
            if (c == 'A' || c == 'B') return READING;
            else if (c == 'C') return PROCESSING;
            else return ERROR;
            
        case PROCESSING:
            if (c == 'D') return FINISHED;
            else return ERROR;
            
        case FINISHED:
            return FINISHED;
            
        case ERROR:
            return ERROR;
    }
    
    return ERROR;
}

int validate_string(char* str, int max_len) {
    if (str == NULL) return 0;
    
    int len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    
    if (len >= max_len) return 0;
    
    if (len < 3) return 0;
    
    for (int i = 0; i < len; i++) {
        if (str[i] < 'A' || str[i] > 'Z') return 0;
    }
    
    return 1;
}

int main(void) {
    char input[256];
    
    printf("Enter sequence (A-Z, max 255 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_string(input, 255)) {
        printf("Invalid input format\n");
        return 1;
    }
    
    enum State result = process_input(input, len - 1, len);
    
    switch (result) {
        case FINISHED:
            printf("Valid sequence: Accepted\n");
            break;
        case ERROR:
            printf("Invalid sequence: Rejected\n");
            break;
        default:
            printf("Incomplete sequence: Processing\n");
            break;
    }
    
    return 0;
}