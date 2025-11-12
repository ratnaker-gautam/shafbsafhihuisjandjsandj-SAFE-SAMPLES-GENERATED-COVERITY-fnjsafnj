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
    STATE_DONE
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
    
    enum State state = STATE_START;
    int token_count = 0;
    int pos = 0;
    int c;
    
    while (state != STATE_DONE && state != STATE_ERROR && token_count < max_tokens) {
        c = get_next_char(input);
        if (c == -1) {
            state = STATE_ERROR;
            break;
        }
        
        switch (state) {
            case STATE_START:
                if (c == EOF) {
                    state = STATE_DONE;
                } else if (isdigit(c)) {
                    tokens[token_count].type = TOKEN_NUMBER;
                    tokens[token_count].value[0] = (char)c;
                    pos = 1;
                    state = STATE_READING_NUMBER;
                } else if (isalpha(c)) {
                    tokens[token_count].type = TOKEN_WORD;
                    tokens[token_count].value[0] = (char)c;
                    pos = 1;
                    state = STATE_READING_WORD;
                } else if (c == ' ' || c == '\t' || c == '\n') {
                    continue;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(c)) {
                    if (pos < 31) {
                        tokens[token_count].value[pos++] = (char)c;
                    } else {
                        state = STATE_ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\n' || c == EOF) {
                    tokens[token_count].value[pos] = '\0';
                    token_count++;
                    state = (c == EOF) ? STATE_DONE : STATE_START;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(c)) {
                    if (pos < 31) {
                        tokens[token_count].value[pos++] = (char)c;
                    } else {
                        state = STATE_ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\n' || c == EOF) {
                    tokens[token_count].value[pos] = '\0';
                    token_count++;
                    state = (c == EOF) ? STATE_DONE : STATE_START;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            default:
                state = STATE_ERROR;
                break;
        }
    }
    
    if (state == STATE_ERROR) return -1;
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
    }
    
    return token_count;
}

void print_tokens(const struct Token *tokens, int count) {
    if (!tokens || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        switch (tokens[i].type) {
            case TOKEN_NUMBER:
                printf("NUMBER: %s\n", tokens[i].value);
                break;
            case TOKEN_WORD:
                printf("WORD: %s\n", tokens[i].value);
                break;
            case TOKEN_EOF:
                printf("EOF\n");
                break;
        }
    }
}

int main(void) {
    printf("Enter text (numbers and words separated by spaces):\n");
    
    struct Token tokens[100];
    int token_count = process_input(stdin, tokens, 100);
    
    if (token_count < 0) {
        printf("Error: Invalid input encountered\n");
        return 1;
    }
    
    printf("\nTokens found:\n");
    print_tokens(tokens, token_count);
    
    return 0;
}