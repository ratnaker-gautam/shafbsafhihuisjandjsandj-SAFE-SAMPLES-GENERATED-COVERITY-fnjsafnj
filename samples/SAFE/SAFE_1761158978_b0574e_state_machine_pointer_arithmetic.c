//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    START,
    READING_DIGITS,
    READING_LETTERS,
    ERROR,
    DONE
} State;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\n' || c == '\0';
}

void process_input(const char *input) {
    State current_state = START;
    const char *ptr = input;
    char buffer[MAX_INPUT];
    char *buf_ptr = buffer;
    
    while (current_state != DONE && current_state != ERROR) {
        if (ptr - input >= MAX_INPUT - 1 || *ptr == '\0') {
            current_state = DONE;
            break;
        }
        
        if (!is_valid_char(*ptr)) {
            current_state = ERROR;
            break;
        }
        
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_DIGITS;
                    *buf_ptr = *ptr;
                    buf_ptr++;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_LETTERS;
                    *buf_ptr = *ptr;
                    buf_ptr++;
                } else if (*ptr == ' ') {
                    ptr++;
                    continue;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_DIGITS:
                if (isdigit((unsigned char)*ptr)) {
                    *buf_ptr = *ptr;
                    buf_ptr++;
                } else if (*ptr == ' ') {
                    current_state = START;
                } else if (*ptr == '\n' || *ptr == '\0') {
                    current_state = DONE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            case READING_LETTERS:
                if (isalpha((unsigned char)*ptr)) {
                    *buf_ptr = *ptr;
                    buf_ptr++;
                } else if (*ptr == ' ') {
                    current_state = START;
                } else if (*ptr == '\n' || *ptr == '\0') {
                    current_state = DONE;
                } else {
                    current_state = ERROR;
                }
                break;
                
            default:
                current_state = ERROR;
                break;
        }
        
        ptr++;
        
        if (buf_ptr - buffer >= MAX_INPUT - 1) {
            current_state = ERROR;
            break;
        }
    }
    
    *buf_ptr = '\0';
    
    if (current_state == ERROR) {
        printf("Error: Invalid input format\n");
    } else if (strlen(buffer) > 0) {
        printf("Processed: %s\n", buffer);
    } else {
        printf("No valid tokens found\n");
    }
}

int main(void) {
    char input[MAX_INPUT];
    
    printf("Enter alphanumeric text (max %d chars): ", MAX_INPUT - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    process_input(input);
    
    return 0;
}