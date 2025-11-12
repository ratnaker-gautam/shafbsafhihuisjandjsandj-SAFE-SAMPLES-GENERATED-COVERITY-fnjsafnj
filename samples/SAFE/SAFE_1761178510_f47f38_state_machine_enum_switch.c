//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_ERROR,
    STATE_DONE
};

enum token_type {
    TOKEN_WORD,
    TOKEN_NUMBER,
    TOKEN_EOF
};

struct token {
    enum token_type type;
    char value[32];
};

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n' || c == EOF;
}

int get_next_char(void) {
    int c = getchar();
    if (c != EOF && !is_valid_char(c)) {
        return -1;
    }
    return c;
}

int parse_input(struct token *tokens, size_t max_tokens) {
    enum state current_state = STATE_START;
    size_t token_count = 0;
    int c;
    size_t pos = 0;
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        c = get_next_char();
        if (c == -1) {
            current_state = STATE_ERROR;
            break;
        }
        
        switch (current_state) {
            case STATE_START:
                if (c == EOF) {
                    current_state = STATE_DONE;
                } else if (isalpha(c)) {
                    current_state = STATE_READING_WORD;
                    tokens[token_count].type = TOKEN_WORD;
                    tokens[token_count].value[pos++] = c;
                } else if (isdigit(c)) {
                    current_state = STATE_READING_NUMBER;
                    tokens[token_count].type = TOKEN_NUMBER;
                    tokens[token_count].value[pos++] = c;
                } else if (c == ' ' || c == '\t' || c == '\n') {
                    continue;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(c)) {
                    if (pos < 31) {
                        tokens[token_count].value[pos++] = c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\n' || c == EOF) {
                    tokens[token_count].value[pos] = '\0';
                    token_count++;
                    pos = 0;
                    if (c == EOF) {
                        current_state = STATE_DONE;
                    } else {
                        current_state = STATE_START;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(c)) {
                    if (pos < 31) {
                        tokens[token_count].value[pos++] = c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\n' || c == EOF) {
                    tokens[token_count].value[pos] = '\0';
                    token_count++;
                    pos = 0;
                    if (c == EOF) {
                        current_state = STATE_DONE;
                    } else {
                        current_state = STATE_START;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            default:
                current_state = STATE_ERROR;
                break;
        }
        
        if (token_count >= max_tokens && current_state != STATE_DONE) {
            current_state = STATE_ERROR;
        }
    }
    
    if (current_state == STATE_ERROR) {
        return -1;
    }
    
    if (token_count < max_tokens) {
        tokens[token_count].type = TOKEN_EOF;
        tokens[token_count].value[0] = '\0';
    }
    
    return (int)token_count;
}

void print_tokens(const struct token *tokens, int count) {
    for (int i = 0; i < count; i++) {
        switch (tokens[i].type) {
            case TOKEN_WORD:
                printf("WORD: %s\n", tokens[i].value);
                break;
            case TOKEN_NUMBER:
                printf("NUMBER: %s\n", tokens[i].value);
                break;
            case TOKEN_EOF:
                printf("EOF\n");
                break;
        }
    }
}

int main(void) {
    struct token tokens[100];
    
    printf("Enter text (words and numbers, EOF to end): ");
    
    int result = parse_input(tokens, 100);
    
    if (result == -1) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("\nParsed tokens:\n");
    print_tokens(tokens, result);
    
    return 0;
}