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
    END
} State;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

void process_input(const char *input) {
    State current_state = START;
    const char *ptr = input;
    char buffer[MAX_INPUT];
    char *buf_ptr = buffer;
    
    while (current_state != END && *ptr != '\0') {
        if ((size_t)(ptr - input) >= MAX_INPUT - 1) {
            break;
        }
        
        switch (current_state) {
            case START:
                if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    buf_ptr = buffer;
                    *buf_ptr = *ptr;
                    buf_ptr++;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    buf_ptr = buffer;
                    *buf_ptr = *ptr;
                    buf_ptr++;
                } else if (isspace((unsigned char)*ptr)) {
                    current_state = SKIPPING;
                } else {
                    current_state = END;
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
                    current_state = SKIPPING;
                    ptr++;
                } else {
                    current_state = END;
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
                    current_state = SKIPPING;
                    ptr++;
                } else {
                    current_state = END;
                }
                break;
                
            case SKIPPING:
                if (isspace((unsigned char)*ptr)) {
                    ptr++;
                } else if (isdigit((unsigned char)*ptr)) {
                    current_state = READING_NUMBER;
                    buf_ptr = buffer;
                    *buf_ptr = *ptr;
                    buf_ptr++;
                    ptr++;
                } else if (isalpha((unsigned char)*ptr)) {
                    current_state = READING_WORD;
                    buf_ptr = buffer;
                    *buf_ptr = *ptr;
                    buf_ptr++;
                    ptr++;
                } else {
                    current_state = END;
                }
                break;
                
            case END:
                break;
        }
        
        if (buf_ptr - buffer >= MAX_INPUT - 1) {
            current_state = END;
        }
    }
    
    if (current_state == READING_NUMBER || current_state == READING_WORD) {
        *buf_ptr = '\0';
        if (current_state == READING_NUMBER) {
            printf("Found number: %s\n", buffer);
        } else {
            printf("Found word: %s\n", buffer);
        }
    }
}

int main() {
    char input[MAX_INPUT];
    
    printf("Enter text (max %d characters): ", MAX_INPUT - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_char(input[i]) && input[i] != '\0') {
            printf("Invalid input character detected\n");
            return 1;
        }
    }
    
    process_input(input);
    
    return 0;
}