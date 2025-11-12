//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_DONE
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

int get_next_token(struct token *tok, FILE *input) {
    static int pos = 0;
    char c;
    int i = 0;
    
    memset(tok->value, 0, sizeof(tok->value));
    
    while (1) {
        c = fgetc(input);
        pos++;
        
        if (!is_valid_char(c)) {
            return -1;
        }
        
        if (c == EOF) {
            tok->type = TOKEN_EOF;
            return 0;
        }
        
        if (c == ' ' || c == '\t' || c == '\n') {
            continue;
        }
        
        if (isdigit(c)) {
            tok->type = TOKEN_NUMBER;
            while (isdigit(c) && i < 31) {
                tok->value[i++] = c;
                c = fgetc(input);
                pos++;
                if (!is_valid_char(c)) {
                    return -1;
                }
            }
            if (c != EOF) {
                ungetc(c, input);
                pos--;
            }
            tok->value[i] = '\0';
            return 0;
        }
        
        if (isalpha(c)) {
            tok->type = TOKEN_WORD;
            while (isalpha(c) && i < 31) {
                tok->value[i++] = c;
                c = fgetc(input);
                pos++;
                if (!is_valid_char(c)) {
                    return -1;
                }
            }
            if (c != EOF) {
                ungetc(c, input);
                pos--;
            }
            tok->value[i] = '\0';
            return 0;
        }
        
        return -1;
    }
}

int main(void) {
    FILE *input = stdin;
    struct token current_token;
    enum state current_state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    int token_result;
    
    printf("Enter text (alphanumeric only): ");
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        token_result = get_next_token(&current_token, input);
        
        if (token_result == -1) {
            current_state = STATE_ERROR;
            break;
        }
        
        switch (current_state) {
            case STATE_START:
                if (current_token.type == TOKEN_NUMBER) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (current_token.type == TOKEN_WORD) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (current_token.type == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (current_token.type == TOKEN_NUMBER) {
                    number_count++;
                } else if (current_token.type == TOKEN_WORD) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (current_token.type == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (current_token.type == TOKEN_WORD) {
                    word_count++;
                } else if (current_token.type == TOKEN_NUMBER) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (current_token.type == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            default:
                current_state = STATE_ERROR;
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input detected\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}