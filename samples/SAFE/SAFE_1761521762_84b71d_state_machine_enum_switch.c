//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
};

enum TokenType {
    TOKEN_NUMBER,
    TOKEN_WORD,
    TOKEN_EOF
};

struct Token {
    enum TokenType type;
    char value[32];
};

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n' || c == EOF;
}

int get_next_char(FILE *stream) {
    int c = fgetc(stream);
    if (c == EOF) return EOF;
    if (!is_valid_char(c)) return -1;
    return c;
}

int process_input(FILE *input, struct Token *tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return -1;
    
    enum State current_state = STATE_START;
    int token_count = 0;
    int c;
    int value_index = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR && token_count < max_tokens) {
        c = get_next_char(input);
        
        switch (current_state) {
            case STATE_START:
                if (c == EOF) {
                    current_state = STATE_END;
                } else if (isdigit(c)) {
                    tokens[token_count].type = TOKEN_NUMBER;
                    tokens[token_count].value[0] = c;
                    value_index = 1;
                    current_state = STATE_READING_NUMBER;
                } else if (isalpha(c)) {
                    tokens[token_count].type = TOKEN_WORD;
                    tokens[token_count].value[0] = c;
                    value_index = 1;
                    current_state = STATE_READING_WORD;
                } else if (c == ' ' || c == '\t' || c == '\n') {
                    continue;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(c)) {
                    if (value_index < 31) {
                        tokens[token_count].value[value_index++] = c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\n' || c == EOF) {
                    tokens[token_count].value[value_index] = '\0';
                    token_count++;
                    current_state = (c == EOF) ? STATE_END : STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(c)) {
                    if (value_index < 31) {
                        tokens[token_count].value[value_index++] = c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\n' || c == EOF) {
                    tokens[token_count].value[value_index] = '\0';
                    token_count++;
                    current_state = (c == EOF) ? STATE_END : STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                return -1;
                
            case STATE_END:
                break;
        }
    }
    
    if (current_state == STATE_ERROR) return -1;
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
        token_count++;
    }
    
    return token_count;
}

int main(void) {
    struct Token tokens[100];
    int token_count;
    
    printf("Enter text (numbers and words, max 100 tokens):\n");
    
    token_count = process_input(stdin, tokens, 100);
    
    if (token_count == -1) {
        printf("Error: Invalid input detected\n");
        return 1;
    }
    
    printf("\nTokens found:\n");
    for (int i = 0; i < token_count; i++) {
        if (tokens[i].type == TOKEN_NUMBER) {
            printf("Number: %s\n", tokens[i].value);
        } else if (tokens[i].type == TOKEN_WORD) {
            printf("Word: %s\n", tokens[i].value);
        } else if (tokens[i].type == TOKEN_EOF) {
            printf("End of input\n");
        }
    }
    
    return 0;
}