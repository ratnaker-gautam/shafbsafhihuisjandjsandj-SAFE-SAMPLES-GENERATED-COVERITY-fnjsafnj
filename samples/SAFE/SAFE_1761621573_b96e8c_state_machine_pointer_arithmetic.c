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
    CHAR_ALPHA,
    CHAR_OTHER,
    CHAR_NULL
} CharType;

CharType get_char_type(char c) {
    if (c == '\0') return CHAR_NULL;
    if (isspace((unsigned char)c)) return CHAR_SPACE;
    if (isdigit((unsigned char)c)) return CHAR_DIGIT;
    if (isalpha((unsigned char)c)) return CHAR_ALPHA;
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
                if (type == CHAR_ALPHA) {
                    current_state = STATE_READING_WORD;
                    buf_ptr = buffer;
                    *(buf_ptr++) = *(ptr++);
                } else if (type == CHAR_DIGIT) {
                    current_state = STATE_READING_NUMBER;
                    buf_ptr = buffer;
                    *(buf_ptr++) = *(ptr++);
                } else if (type == CHAR_SPACE || type == CHAR_OTHER) {
                    current_state = STATE_SKIPPING;
                    ptr++;
                } else if (type == CHAR_NULL) {
                    current_state = STATE_END;
                }
                break;
                
            case STATE_READING_WORD:
                if (type == CHAR_ALPHA) {
                    if ((size_t)(buf_ptr - buffer) < MAX_INPUT - 1) {
                        *(buf_ptr++) = *(ptr++);
                    } else {
                        ptr++;
                    }
                } else {
                    *buf_ptr = '\0';
                    printf("Word: %s\n", buffer);
                    current_state = STATE_START;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (type == CHAR_DIGIT) {
                    if ((size_t)(buf_ptr - buffer) < MAX_INPUT - 1) {
                        *(buf_ptr++) = *(ptr++);
                    } else {
                        ptr++;
                    }
                } else {
                    *buf_ptr = '\0';
                    printf("Number: %s\n", buffer);
                    current_state = STATE_START;
                }
                break;
                
            case STATE_SKIPPING:
                if (type == CHAR_SPACE || type == CHAR_OTHER) {
                    ptr++;
                } else if (type == CHAR_NULL) {
                    current_state = STATE_END;
                } else {
                    current_state = STATE_START;
                }
                break;
                
            case STATE_END:
                break;
        }
    }
}

int main(void) {
    char input[MAX_INPUT];
    
    printf("Enter text to analyze: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\0')) {
        printf("No input provided\n");
        return 0;
    }
    
    process_input(input);
    return 0;
}