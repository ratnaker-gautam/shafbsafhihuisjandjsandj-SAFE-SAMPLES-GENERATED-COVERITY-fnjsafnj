//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
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
    if (c == '\0' || c == '\n') return TOKEN_EOF;
    if (isspace((unsigned char)c)) return TOKEN_SPACE;
    if (isdigit((unsigned char)c)) return TOKEN_DIGIT;
    if (isalpha((unsigned char)c)) return TOKEN_LETTER;
    return TOKEN_INVALID;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    int number_count = 0;
    int word_count = 0;
    int pos = 0;
    parser_state_t state = STATE_START;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    while (state != STATE_DONE && state != STATE_ERROR) {
        if (pos >= MAX_INPUT_LEN || input[pos] == '\0') {
            break;
        }
        
        token_type_t token = get_next_token(input[pos]);
        
        switch (state) {
            case STATE_START:
                if (token == TOKEN_DIGIT) {
                    state = STATE_READING_NUMBER;
                    number_count++;
                } else if (token == TOKEN_LETTER) {
                    state = STATE_READING_WORD;
                    word_count++;
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
                fprintf(stderr, "Invalid input at position %d\n", pos);
                return 1;
                
            case STATE_DONE:
                break;
        }
    }
    
    if (state == STATE_ERROR) {
        fprintf(stderr, "Invalid input at position %d\n", pos);
        return 1;
    }
    
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}