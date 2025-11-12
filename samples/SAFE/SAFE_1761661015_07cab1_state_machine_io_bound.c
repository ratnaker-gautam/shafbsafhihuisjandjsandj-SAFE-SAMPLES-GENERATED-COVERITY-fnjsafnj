//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

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
    if (c == '\0' || c == '\n') return TOKEN_EOF;
    if (isspace(c)) return TOKEN_SPACE;
    if (isdigit(c)) return TOKEN_DIGIT;
    if (isalpha(c)) return TOKEN_LETTER;
    return TOKEN_INVALID;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    int numbers_found = 0;
    int words_found = 0;
    int valid_input = 1;
    
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
    
    parser_state_t state = STATE_START;
    size_t pos = 0;
    
    while (state != STATE_DONE && state != STATE_ERROR && pos <= len) {
        char current_char = input[pos];
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
                } else if (token == TOKEN_SPACE) {
                    state = STATE_START;
                    pos++;
                } else if (token == TOKEN_EOF) {
                    state = STATE_DONE;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (token == TOKEN_LETTER) {
                    pos++;
                } else if (token == TOKEN_SPACE) {
                    state = STATE_START;
                    pos++;
                } else if (token == TOKEN_EOF) {
                    state = STATE_DONE;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                valid_input = 0;
                state = STATE_DONE;
                break;
                
            case STATE_DONE:
                break;
        }
    }
    
    if (state == STATE_ERROR) {
        valid_input = 0;
    }
    
    if (valid_input) {
        printf("Parsing successful\n");
        printf("Numbers found: %d\n", numbers_found);
        printf("Words found: %d\n", words_found);
    } else {
        printf("Invalid input format\n");
        return 1;
    }
    
    return 0;
}