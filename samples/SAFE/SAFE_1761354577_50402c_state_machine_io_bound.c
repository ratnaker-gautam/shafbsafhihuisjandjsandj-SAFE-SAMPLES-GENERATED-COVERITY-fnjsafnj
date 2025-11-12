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
    TOKEN_EOF,
    TOKEN_INVALID
} token_type_t;

token_type_t get_token_type(char c) {
    if (isdigit(c)) return TOKEN_DIGIT;
    if (isalpha(c)) return TOKEN_LETTER;
    if (isspace(c)) return TOKEN_SPACE;
    if (c == '\0') return TOKEN_EOF;
    return TOKEN_INVALID;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    int numbers_found = 0;
    int words_found = 0;
    int pos = 0;
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
    
    while (state != STATE_DONE && state != STATE_ERROR && pos <= (int)len) {
        char current_char = (pos < (int)len) ? input[pos] : '\0';
        token_type_t token = get_token_type(current_char);
        
        switch (state) {
            case STATE_START:
                if (token == TOKEN_DIGIT) {
                    state = STATE_READING_NUMBER;
                    numbers_found++;
                } else if (token == TOKEN_LETTER) {
                    state = STATE_READING_WORD;
                    words_found++;
                } else if (token == TOKEN_SPACE) {
                    pos++;
                } else if (token == TOKEN_EOF) {
                    state = STATE_DONE;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (token == TOKEN_DIGIT) {
                    pos++;
                } else if (token == TOKEN_SPACE || token == TOKEN_EOF) {
                    state = STATE_START;
                    pos++;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (token == TOKEN_LETTER) {
                    pos++;
                } else if (token == TOKEN_SPACE || token == TOKEN_EOF) {
                    state = STATE_START;
                    pos++;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                printf("Invalid character at position %d: '%c'\n", pos, current_char);
                return 1;
                
            case STATE_DONE:
                break;
        }
    }
    
    if (state == STATE_ERROR) {
        printf("Parsing error occurred\n");
        return 1;
    }
    
    printf("Parsing completed successfully\n");
    printf("Numbers found: %d\n", numbers_found);
    printf("Words found: %d\n", words_found);
    
    return 0;
}