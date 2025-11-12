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

token_type_t get_next_token(char c) {
    if (c == '\0') return TOKEN_EOF;
    if (isspace(c)) return TOKEN_SPACE;
    if (isdigit(c)) return TOKEN_DIGIT;
    if (isalpha(c)) return TOKEN_LETTER;
    return TOKEN_INVALID;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    parser_state_t current_state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    int pos = 0;
    
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
    
    while (pos <= (int)len && current_state != STATE_ERROR && current_state != STATE_DONE) {
        char current_char = input[pos];
        token_type_t token = get_next_token(current_char);
        
        switch (current_state) {
            case STATE_START:
                if (token == TOKEN_DIGIT) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (token == TOKEN_LETTER) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (token == TOKEN_SPACE) {
                    current_state = STATE_START;
                } else if (token == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (token == TOKEN_DIGIT) {
                    current_state = STATE_READING_NUMBER;
                } else if (token == TOKEN_SPACE) {
                    current_state = STATE_START;
                } else if (token == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (token == TOKEN_LETTER) {
                    current_state = STATE_READING_WORD;
                } else if (token == TOKEN_SPACE) {
                    current_state = STATE_START;
                } else if (token == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                break;
                
            case STATE_DONE:
                break;
        }
        
        pos++;
    }
    
    if (current_state == STATE_ERROR) {
        printf("Invalid input format at position %d\n", pos - 1);
        return 1;
    }
    
    printf("Parsing completed successfully\n");
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}