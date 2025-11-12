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
    CHAR_ALPHA,
    CHAR_DIGIT,
    CHAR_SPACE,
    CHAR_OTHER,
    CHAR_END
} CharType;

CharType get_char_type(char c) {
    if (c == '\0') return CHAR_END;
    if (isspace((unsigned char)c)) return CHAR_SPACE;
    if (isalpha((unsigned char)c)) return CHAR_ALPHA;
    if (isdigit((unsigned char)c)) return CHAR_DIGIT;
    return CHAR_OTHER;
}

void process_input(const char *input) {
    State current_state = STATE_START;
    const char *ptr = input;
    int word_count = 0;
    int number_count = 0;
    
    while (current_state != STATE_END) {
        CharType type = get_char_type(*ptr);
        
        switch (current_state) {
            case STATE_START:
                if (type == CHAR_ALPHA) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (type == CHAR_DIGIT) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (type == CHAR_SPACE || type == CHAR_OTHER) {
                    current_state = STATE_SKIPPING;
                } else if (type == CHAR_END) {
                    current_state = STATE_END;
                }
                break;
                
            case STATE_READING_WORD:
                if (type == CHAR_SPACE || type == CHAR_OTHER) {
                    current_state = STATE_SKIPPING;
                } else if (type == CHAR_DIGIT) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (type == CHAR_END) {
                    current_state = STATE_END;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (type == CHAR_SPACE || type == CHAR_OTHER) {
                    current_state = STATE_SKIPPING;
                } else if (type == CHAR_ALPHA) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (type == CHAR_END) {
                    current_state = STATE_END;
                }
                break;
                
            case STATE_SKIPPING:
                if (type == CHAR_ALPHA) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (type == CHAR_DIGIT) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (type == CHAR_END) {
                    current_state = STATE_END;
                }
                break;
                
            case STATE_END:
                break;
        }
        
        if (current_state != STATE_END) {
            ptr++;
            if (ptr - input >= MAX_INPUT) {
                break;
            }
        }
    }
    
    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
}

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    
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