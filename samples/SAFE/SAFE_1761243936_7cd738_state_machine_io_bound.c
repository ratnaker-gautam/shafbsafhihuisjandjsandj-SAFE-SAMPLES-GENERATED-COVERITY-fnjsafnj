//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_DONE
} parser_state_t;

typedef enum {
    TOKEN_DIGIT,
    TOKEN_LETTER,
    TOKEN_SPACE,
    TOKEN_END,
    TOKEN_INVALID
} token_type_t;

token_type_t get_token_type(char c) {
    if (c == '\0') return TOKEN_END;
    if (isspace(c)) return TOKEN_SPACE;
    if (isdigit(c)) return TOKEN_DIGIT;
    if (isalpha(c)) return TOKEN_LETTER;
    return TOKEN_INVALID;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    int number_count = 0;
    int word_count = 0;
    int i = 0;
    parser_state_t state = STATE_START;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    while (i <= len) {
        char current_char = input[i];
        token_type_t token = get_token_type(current_char);
        
        switch (state) {
            case STATE_START:
                if (token == TOKEN_DIGIT) {
                    state = STATE_READING_NUMBER;
                    number_count++;
                } else if (token == TOKEN_LETTER) {
                    state = STATE_READING_WORD;
                    word_count++;
                } else if (token == TOKEN_SPACE) {
                    i++;
                } else if (token == TOKEN_END) {
                    state = STATE_DONE;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (token == TOKEN_DIGIT) {
                    i++;
                } else if (token == TOKEN_SPACE || token == TOKEN_END) {
                    state = STATE_START;
                    i++;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (token == TOKEN_LETTER) {
                    i++;
                } else if (token == TOKEN_SPACE || token == TOKEN_END) {
                    state = STATE_START;
                    i++;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Invalid input format at position %d\n", i);
                return 1;
                
            case STATE_DONE:
                break;
        }
        
        if (state == STATE_DONE) break;
    }
    
    if (state == STATE_ERROR) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}