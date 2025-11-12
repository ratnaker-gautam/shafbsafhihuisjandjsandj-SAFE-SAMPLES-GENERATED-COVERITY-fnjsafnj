//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    START,
    READING_NUMBER,
    READING_WORD,
    SKIPPING,
    FINISHED
} State;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

void process_input(const char *input) {
    State current_state = START;
    const char *ptr = input;
    char buffer[MAX_INPUT];
    char *buf_ptr = buffer;
    
    while (current_state != FINISHED && *ptr != '\0') {
        if (!is_valid_char(*ptr)) {
            current_state = FINISHED;
            break;
        }
        
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    *buf_ptr = *ptr;
                    buf_ptr++;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    *buf_ptr = *ptr;
                    buf_ptr++;
                } else {
                    current_state = SKIPPING;
                }
                ptr++;
                break;
                
            case READING_NUMBER:
                if (isdigit((unsigned char)*ptr)) {
                    *buf_ptr = *ptr;
                    buf_ptr++;
                    ptr++;
                } else if (isspace((unsigned char)*ptr)) {
                    *buf_ptr = '\0';
                    printf("Found number: %s\n", buffer);
                    buf_ptr = buffer;
                    current_state = START;
                    ptr++;
                } else {
                    current_state = SKIPPING;
                    buf_ptr = buffer;
                }
                break;
                
            case READING_WORD:
                if (isalpha((unsigned char)*ptr)) {
                    *buf_ptr = *ptr;
                    buf_ptr++;
                    ptr++;
                } else if (isspace((unsigned char)*ptr)) {
                    *buf_ptr = '\0';
                    printf("Found word: %s\n", buffer);
                    buf_ptr = buffer;
                    current_state = START;
                    ptr++;
                } else {
                    current_state = SKIPPING;
                    buf_ptr = buffer;
                }
                break;
                
            case SKIPPING:
                if (isspace((unsigned char)*ptr)) {
                    current_state = START;
                }
                ptr++;
                break;
                
            case FINISHED:
                break;
        }
        
        if (buf_ptr - buffer >= MAX_INPUT - 1) {
            *buf_ptr = '\0';
            if (current_state == READING_NUMBER) {
                printf("Found number: %s\n", buffer);
            } else if (current_state == READING_WORD) {
                printf("Found word: %s\n", buffer);
            }
            buf_ptr = buffer;
            current_state = START;
        }
    }
    
    if (buf_ptr != buffer && current_state != FINISHED) {
        *buf_ptr = '\0';
        if (current_state == READING_NUMBER) {
            printf("Found number: %s\n", buffer);
        } else if (current_state == READING_WORD) {
            printf("Found word: %s\n", buffer);
        }
    }
}

int main(void) {
    char input[MAX_INPUT];
    
    printf("Enter text (alphanumeric and spaces only): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_char(input[i])) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    process_input(input);
    
    return 0;
}