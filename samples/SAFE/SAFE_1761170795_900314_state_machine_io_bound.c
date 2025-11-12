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

void process_input(const char* input) {
    parser_state_t state = STATE_START;
    int pos = 0;
    char current_token[MAX_INPUT_LEN] = {0};
    int token_len = 0;
    
    printf("Processing: '%s'\n", input);
    
    while (state != STATE_DONE && state != STATE_ERROR) {
        char c = input[pos];
        token_type_t token = get_token_type(c);
        
        switch (state) {
            case STATE_START:
                if (token == TOKEN_DIGIT) {
                    state = STATE_READING_NUMBER;
                    current_token[token_len++] = c;
                } else if (token == TOKEN_LETTER) {
                    state = STATE_READING_WORD;
                    current_token[token_len++] = c;
                } else if (token == TOKEN_SPACE) {
                } else if (token == TOKEN_END) {
                    state = STATE_DONE;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (token == TOKEN_DIGIT) {
                    if (token_len < MAX_INPUT_LEN - 1) {
                        current_token[token_len++] = c;
                    } else {
                        state = STATE_ERROR;
                    }
                } else if (token == TOKEN_SPACE || token == TOKEN_END) {
                    current_token[token_len] = '\0';
                    printf("Found number: %s\n", current_token);
                    token_len = 0;
                    state = (token == TOKEN_END) ? STATE_DONE : STATE_START;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (token == TOKEN_LETTER) {
                    if (token_len < MAX_INPUT_LEN - 1) {
                        current_token[token_len++] = c;
                    } else {
                        state = STATE_ERROR;
                    }
                } else if (token == TOKEN_SPACE || token == TOKEN_END) {
                    current_token[token_len] = '\0';
                    printf("Found word: %s\n", current_token);
                    token_len = 0;
                    state = (token == TOKEN_END) ? STATE_DONE : STATE_START;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input at position %d\n", pos);
                break;
                
            case STATE_DONE:
                break;
        }
        
        pos++;
        if (pos > MAX_INPUT_LEN * 2) {
            state = STATE_ERROR;
            break;
        }
    }
    
    if (state == STATE_DONE) {
        printf("Processing completed successfully.\n");
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    
    printf("Enter text to parse (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Error: Empty input\n");
        return 1;
    }
    
    process_input(input);
    return 0;
}