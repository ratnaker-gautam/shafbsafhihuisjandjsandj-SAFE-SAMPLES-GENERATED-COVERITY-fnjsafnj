//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define MAX_STATES 8

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_PUNCTUATION,
    STATE_END,
    STATE_ERROR
} State;

typedef enum {
    CHAR_LETTER,
    CHAR_DIGIT,
    CHAR_SPACE,
    CHAR_PUNCT,
    CHAR_END,
    CHAR_INVALID
} CharType;

CharType get_char_type(char c) {
    if (c == '\0') return CHAR_END;
    if (isspace((unsigned char)c)) return CHAR_SPACE;
    if (isalpha((unsigned char)c)) return CHAR_LETTER;
    if (isdigit((unsigned char)c)) return CHAR_DIGIT;
    if (ispunct((unsigned char)c)) return CHAR_PUNCT;
    return CHAR_INVALID;
}

State transition_table[MAX_STATES][6] = {
    {STATE_ERROR, STATE_READING_WORD, STATE_READING_NUMBER, STATE_PUNCTUATION, STATE_END, STATE_ERROR},
    {STATE_ERROR, STATE_READING_WORD, STATE_ERROR, STATE_PUNCTUATION, STATE_END, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_READING_NUMBER, STATE_PUNCTUATION, STATE_END, STATE_ERROR},
    {STATE_ERROR, STATE_READING_WORD, STATE_READING_NUMBER, STATE_PUNCTUATION, STATE_END, STATE_ERROR},
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
};

void process_input(const char *input) {
    State current_state = STATE_START;
    const char *ptr = input;
    char buffer[MAX_INPUT];
    char *buf_ptr = buffer;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        CharType ctype = get_char_type(*ptr);
        State next_state = transition_table[current_state][ctype];
        
        switch (current_state) {
            case STATE_START:
                if (next_state == STATE_READING_WORD || next_state == STATE_READING_NUMBER) {
                    buf_ptr = buffer;
                    *buf_ptr = '\0';
                }
                break;
                
            case STATE_READING_WORD:
                if (next_state != STATE_READING_WORD) {
                    if (buf_ptr - buffer > 0) {
                        *buf_ptr = '\0';
                        printf("Word: %s\n", buffer);
                    }
                    buf_ptr = buffer;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (next_state != STATE_READING_NUMBER) {
                    if (buf_ptr - buffer > 0) {
                        *buf_ptr = '\0';
                        printf("Number: %s\n", buffer);
                    }
                    buf_ptr = buffer;
                }
                break;
                
            case STATE_PUNCTUATION:
                printf("Punctuation: %c\n", *(ptr - 1));
                break;
                
            default:
                break;
        }
        
        if (next_state == STATE_READING_WORD || next_state == STATE_READING_NUMBER) {
            if (buf_ptr - buffer < MAX_INPUT - 1) {
                *buf_ptr = *ptr;
                buf_ptr++;
            }
        }
        
        current_state = next_state;
        
        if (*ptr != '\0') {
            ptr++;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input sequence\n");
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
        return 1;
    }
    
    process_input(input);
    return 0;
}