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

int get_next_char(void) {
    int c = getchar();
    if (c != EOF && !is_valid_char(c)) {
        return -1;
    }
    return c;
}

int parse_token(struct token *tok) {
    static int current_char = ' ';
    enum state current_state = STATE_START;
    int pos = 0;
    
    memset(tok->value, 0, sizeof(tok->value));
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        if (current_char == ' ' || current_char == '\t' || current_char == '\n') {
            if (current_state == STATE_START) {
                current_char = get_next_char();
                continue;
            } else if (current_state == STATE_READING_NUMBER || current_state == STATE_READING_WORD) {
                current_state = STATE_DONE;
                break;
            }
        }
        
        if (current_char == EOF) {
            if (current_state == STATE_START) {
                tok->type = TOKEN_EOF;
                return 1;
            } else {
                current_state = STATE_DONE;
                break;
            }
        }
        
        if (current_char == -1) {
            current_state = STATE_ERROR;
            break;
        }
        
        switch (current_state) {
            case STATE_START:
                if (isdigit(current_char)) {
                    current_state = STATE_READING_NUMBER;
                    tok->type = TOKEN_NUMBER;
                } else if (isalpha(current_char)) {
                    current_state = STATE_READING_WORD;
                    tok->type = TOKEN_WORD;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(current_char)) {
                    if (pos < 31) {
                        tok->value[pos++] = current_char;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isalpha(current_char)) {
                    current_state = STATE_ERROR;
                } else {
                    current_state = STATE_DONE;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(current_char)) {
                    if (pos < 31) {
                        tok->value[pos++] = current_char;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isdigit(current_char)) {
                    current_state = STATE_ERROR;
                } else {
                    current_state = STATE_DONE;
                }
                break;
                
            default:
                current_state = STATE_ERROR;
                break;
        }
        
        if (current_state == STATE_READING_NUMBER || current_state == STATE_READING_WORD) {
            current_char = get_next_char();
        }
    }
    
    if (current_state == STATE_ERROR) {
        return -1;
    }
    
    if (pos == 0 && tok->type != TOKEN_EOF) {
        return -1;
    }
    
    tok->value[pos] = '\0';
    return 0;
}

int main(void) {
    struct token tok;
    int number_count = 0;
    int word_count = 0;
    int result;
    
    printf("Enter text (alphanumeric only, separate with spaces):\n");
    
    while (1) {
        result = parse_token(&tok);
        
        if (result == -1) {
            printf("Error: Invalid input format\n");
            return 1;
        }
        
        if (tok.type == TOKEN_EOF) {
            break;
        }
        
        if (tok.type == TOKEN_NUMBER) {
            printf("Number: %s\n", tok.value);
            number_count++;
        } else if (tok.type == TOKEN_WORD) {
            printf("Word: %s\n", tok.value);
            word_count++;
        }
    }
    
    printf("\nSummary: %d numbers, %d words\n", number_count, word_count);
    return 0;
}