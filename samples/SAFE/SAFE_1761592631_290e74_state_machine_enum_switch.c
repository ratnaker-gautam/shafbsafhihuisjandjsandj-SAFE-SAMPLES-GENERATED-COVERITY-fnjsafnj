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
    return isalnum(c) || c == ' ' || c == '\n' || c == '\t' || c == EOF;
}

int get_next_token(struct token *tok, FILE *input) {
    static int pos = 0;
    int c;
    int idx = 0;
    
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
        
        if (c == ' ' || c == '\n' || c == '\t') {
            continue;
        }
        
        if (isdigit(c)) {
            tok->type = TOKEN_NUMBER;
            while (isdigit(c) && idx < 31) {
                tok->value[idx++] = c;
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
            tok->value[idx] = '\0';
            return 0;
        }
        
        if (isalpha(c)) {
            tok->type = TOKEN_WORD;
            while (isalpha(c) && idx < 31) {
                tok->value[idx++] = tolower(c);
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
            tok->value[idx] = '\0';
            return 0;
        }
    }
    
    return -1;
}

int main(void) {
    FILE *input = stdin;
    struct token current_token;
    enum state current_state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    int valid = 1;
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_START:
                if (get_next_token(&current_token, input) == -1) {
                    current_state = STATE_ERROR;
                    break;
                }
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
                if (get_next_token(&current_token, input) == -1) {
                    current_state = STATE_ERROR;
                    break;
                }
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
                if (get_next_token(&current_token, input) == -1) {
                    current_state = STATE_ERROR;
                    break;
                }
                if (current_token.type == TOKEN_NUMBER) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (current_token.type == TOKEN_WORD) {
                    word_count++;
                } else if (current_token.type == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                valid = 0;
                current_state = STATE_DONE;
                break;
                
            case STATE_DONE:
                break;
        }
    }
    
    if (!valid) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Numbers: %d\n", number_count);
    printf("Words: %d\n", word_count);
    
    return 0;
}