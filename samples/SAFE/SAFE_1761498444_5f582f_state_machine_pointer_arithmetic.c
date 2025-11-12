//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_SKIPPING,
    STATE_END
} State;

typedef enum {
    CHAR_SPACE,
    CHAR_DIGIT,
    CHAR_LETTER,
    CHAR_OTHER,
    CHAR_END
} CharType;

CharType get_char_type(char c) {
    if (isspace(c)) return CHAR_SPACE;
    if (isdigit(c)) return CHAR_DIGIT;
    if (isalpha(c)) return CHAR_LETTER;
    if (c == '\0') return CHAR_END;
    return CHAR_OTHER;
}

void process_input(const char *input) {
    State current_state = STATE_START;
    const char *ptr = input;
    char buffer[MAX_INPUT];
    char *buf_ptr = buffer;
    
    while (current_state != STATE_END) {
        CharType type = get_char_type(*ptr);
        
        switch (current_state) {
            case STATE_START:
                if (type == CHAR_LETTER) {
                    current_state = STATE_READING_WORD;
                    *buf_ptr = *ptr;
                    buf_ptr++;
                } else if (type == CHAR_DIGIT) {
                    current_state = STATE_READING_NUMBER;
                    *buf_ptr = *ptr;
                    buf_ptr++;
                } else if (type == CHAR_SPACE || type == CHAR_OTHER) {
                    current_state = STATE_SKIPPING;
                } else if (type == CHAR_END) {
                    current_state = STATE_END;
                }
                ptr++;
                break;
                
            case STATE_READING_WORD:
                if (type == CHAR_LETTER) {
                    *buf_ptr = *ptr;
                    buf_ptr++;
                    ptr++;
                } else {
                    *buf_ptr = '\0';
                    printf("Found word: %s\n", buffer);
                    buf_ptr = buffer;
                    current_state = STATE_START;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (type == CHAR_DIGIT) {
                    *buf_ptr = *ptr;
                    buf_ptr++;
                    ptr++;
                } else {
                    *buf_ptr = '\0';
                    printf("Found number: %s\n", buffer);
                    buf_ptr = buffer;
                    current_state = STATE_START;
                }
                break;
                
            case STATE_SKIPPING:
                if (type == CHAR_LETTER || type == CHAR_DIGIT) {
                    current_state = STATE_START;
                } else if (type == CHAR_END) {
                    current_state = STATE_END;
                } else {
                    ptr++;
                }
                break;
                
            case STATE_END:
                break;
        }
        
        if (buf_ptr - buffer >= MAX_INPUT - 1) {
            *buf_ptr = '\0';
            printf("Buffer overflow protection triggered\n");
            break;
        }
    }
}

int main() {
    char input[MAX_INPUT];
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    process_input(input);
    return 0;
}