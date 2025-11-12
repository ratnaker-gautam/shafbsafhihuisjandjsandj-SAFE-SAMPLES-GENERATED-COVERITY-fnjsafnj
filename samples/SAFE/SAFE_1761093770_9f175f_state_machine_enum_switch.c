//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_END,
    STATE_ERROR
};

enum token_type {
    TOKEN_WORD,
    TOKEN_NUMBER,
    TOKEN_END
};

struct token {
    enum token_type type;
    char value[32];
};

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '\n' || c == '\t' || c == '\0';
}

int get_next_token(FILE *stream, struct token *tok) {
    int c;
    int idx = 0;
    
    memset(tok->value, 0, sizeof(tok->value));
    
    while ((c = fgetc(stream)) != EOF) {
        if (!is_valid_char(c)) {
            return -1;
        }
        
        if (c == ' ' || c == '\n' || c == '\t') {
            if (idx > 0) {
                ungetc(c, stream);
                tok->value[idx] = '\0';
                return 0;
            }
            continue;
        }
        
        if (idx >= 31) {
            return -1;
        }
        
        tok->value[idx++] = c;
    }
    
    if (idx > 0) {
        tok->value[idx] = '\0';
        return 0;
    }
    
    tok->type = TOKEN_END;
    return 0;
}

void process_token(struct token *tok) {
    if (tok->type == TOKEN_WORD) {
        printf("Word: %s\n", tok->value);
    } else if (tok->type == TOKEN_NUMBER) {
        printf("Number: %s\n", tok->value);
    }
}

int classify_token(struct token *tok) {
    if (strlen(tok->value) == 0) {
        return -1;
    }
    
    int has_digit = 0;
    int has_alpha = 0;
    
    for (size_t i = 0; i < strlen(tok->value); i++) {
        if (isdigit(tok->value[i])) {
            has_digit = 1;
        } else if (isalpha(tok->value[i])) {
            has_alpha = 1;
        } else {
            return -1;
        }
    }
    
    if (has_digit && !has_alpha) {
        tok->type = TOKEN_NUMBER;
    } else if (has_alpha && !has_digit) {
        tok->type = TOKEN_WORD;
    } else {
        return -1;
    }
    
    return 0;
}

int main(void) {
    enum state current_state = STATE_START;
    struct token current_token;
    int token_result;
    
    printf("Enter text (alphanumeric words and numbers separated by spaces):\n");
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_START:
                token_result = get_next_token(stdin, &current_token);
                if (token_result == -1) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                if (current_token.type == TOKEN_END) {
                    current_state = STATE_END;
                } else if (classify_token(&current_token) == 0) {
                    current_state = STATE_READING_WORD;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
            case STATE_READING_NUMBER:
                process_token(&current_token);
                token_result = get_next_token(stdin, &current_token);
                if (token_result == -1) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                if (current_token.type == TOKEN_END) {
                    current_state = STATE_END;
                } else if (classify_token(&current_token) == 0) {
                    if (current_token.type == TOKEN_WORD) {
                        current_state = STATE_READING_WORD;
                    } else {
                        current_state = STATE_READING_NUMBER;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_END:
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Error: Invalid input detected\n");
                return 1;
        }
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Error: Invalid input detected\n");
        return 1;
    }
    
    printf("Processing completed successfully\n");
    return 0;
}