//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING,
    STATE_NUMBER,
    STATE_WORD,
    STATE_END,
    STATE_ERROR
};

enum token_type {
    TOKEN_NUMBER,
    TOKEN_WORD,
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

int parse_token(struct token *tok) {
    static int current_char = ' ';
    static int initialized = 0;
    
    if (!initialized) {
        current_char = get_next_char();
        initialized = 1;
    }
    
    memset(tok->value, 0, sizeof(tok->value));
    int pos = 0;
    enum state current_state = STATE_START;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_START:
                if (current_char == EOF) {
                    tok->type = TOKEN_EOF;
                    current_state = STATE_END;
                } else if (isspace(current_char)) {
                    current_char = get_next_char();
                } else if (isdigit(current_char)) {
                    current_state = STATE_NUMBER;
                } else if (isalpha(current_char)) {
                    current_state = STATE_WORD;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_NUMBER:
                if (isdigit(current_char) && pos < 31) {
                    tok->value[pos++] = current_char;
                    current_char = get_next_char();
                } else {
                    tok->type = TOKEN_NUMBER;
                    current_state = STATE_END;
                }
                break;
                
            case STATE_WORD:
                if (isalpha(current_char) && pos < 31) {
                    tok->value[pos++] = current_char;
                    current_char = get_next_char();
                } else {
                    tok->type = TOKEN_WORD;
                    current_state = STATE_END;
                }
                break;
                
            default:
                current_state = STATE_ERROR;
                break;
        }
        
        if (current_char == -1) {
            current_state = STATE_ERROR;
        }
    }
    
    if (current_state == STATE_ERROR) {
        return -1;
    }
    
    if (pos > 0) {
        tok->value[pos] = '\0';
    }
    
    return 0;
}

int main(void) {
    struct token tok;
    int number_count = 0;
    int word_count = 0;
    
    printf("Enter text (alphanumeric only, EOF to end):\n");
    
    while (1) {
        if (parse_token(&tok) != 0) {
            fprintf(stderr, "Error: Invalid input character\n");
            return 1;
        }
        
        if (tok.type == TOKEN_EOF) {
            break;
        }
        
        switch (tok.type) {
            case TOKEN_NUMBER:
                printf("Number: %s\n", tok.value);
                number_count++;
                break;
            case TOKEN_WORD:
                printf("Word: %s\n", tok.value);
                word_count++;
                break;
            default:
                break;
        }
    }
    
    printf("\nSummary: %d numbers, %d words\n", number_count, word_count);
    return 0;
}