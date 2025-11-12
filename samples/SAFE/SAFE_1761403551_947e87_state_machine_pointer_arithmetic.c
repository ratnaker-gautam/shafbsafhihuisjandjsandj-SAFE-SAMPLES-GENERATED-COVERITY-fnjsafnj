//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    START,
    READING_NUM,
    READING_WORD,
    SKIPPING,
    END
} State;

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '\n' || c == '\t';
}

void process_input(const char *input) {
    State current_state = START;
    const char *ptr = input;
    char buffer[MAX_INPUT];
    char *buf_ptr = buffer;
    
    while (current_state != END && *ptr != '\0') {
        if (!is_valid_char(*ptr)) {
            current_state = END;
            break;
        }
        
        switch (current_state) {
            case START:
                if (isdigit(*ptr)) {
                    current_state = READING_NUM;
                    *buf_ptr = *ptr;
                    buf_ptr++;
                } else if (isalpha(*ptr)) {
                    current_state = READING_WORD;
                    *buf_ptr = *ptr;
                    buf_ptr++;
                } else {
                    current_state = SKIPPING;
                }
                break;
                
            case READING_NUM:
                if (isdigit(*ptr)) {
                    *buf_ptr = *ptr;
                    buf_ptr++;
                } else if (isalpha(*ptr) || *ptr == ' ') {
                    *buf_ptr = '\0';
                    if (buf_ptr - buffer > 0) {
                        printf("Found number: %s\n", buffer);
                    }
                    buf_ptr = buffer;
                    current_state = isalpha(*ptr) ? READING_WORD : SKIPPING;
                    if (isalpha(*ptr)) {
                        *buf_ptr = *ptr;
                        buf_ptr++;
                    }
                } else {
                    current_state = END;
                }
                break;
                
            case READING_WORD:
                if (isalpha(*ptr)) {
                    *buf_ptr = *ptr;
                    buf_ptr++;
                } else if (isdigit(*ptr) || *ptr == ' ') {
                    *buf_ptr = '\0';
                    if (buf_ptr - buffer > 0) {
                        printf("Found word: %s\n", buffer);
                    }
                    buf_ptr = buffer;
                    current_state = isdigit(*ptr) ? READING_NUM : SKIPPING;
                    if (isdigit(*ptr)) {
                        *buf_ptr = *ptr;
                        buf_ptr++;
                    }
                } else {
                    current_state = END;
                }
                break;
                
            case SKIPPING:
                if (isdigit(*ptr)) {
                    current_state = READING_NUM;
                    *buf_ptr = *ptr;
                    buf_ptr++;
                } else if (isalpha(*ptr)) {
                    current_state = READING_WORD;
                    *buf_ptr = *ptr;
                    buf_ptr++;
                }
                break;
                
            case END:
                break;
        }
        
        if (buf_ptr - buffer >= MAX_INPUT - 1) {
            *buf_ptr = '\0';
            if (current_state == READING_NUM) {
                printf("Found number: %s\n", buffer);
            } else if (current_state == READING_WORD) {
                printf("Found word: %s\n", buffer);
            }
            buf_ptr = buffer;
            current_state = SKIPPING;
        }
        
        ptr++;
    }
    
    if (current_state == READING_NUM || current_state == READING_WORD) {
        *buf_ptr = '\0';
        if (buf_ptr - buffer > 0) {
            if (current_state == READING_NUM) {
                printf("Found number: %s\n", buffer);
            } else {
                printf("Found word: %s\n", buffer);
            }
        }
    }
}

int main() {
    char input[MAX_INPUT + 1];
    
    printf("Enter text (max %d characters): ", MAX_INPUT);
    
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
    
    process_input(input);
    
    return 0;
}