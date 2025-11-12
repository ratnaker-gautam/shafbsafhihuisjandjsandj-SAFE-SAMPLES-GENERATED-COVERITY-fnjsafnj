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

int get_next_token(FILE *stream, struct token *token) {
    int c;
    int pos = 0;
    
    memset(token->value, 0, sizeof(token->value));
    
    while ((c = fgetc(stream)) != EOF) {
        if (!is_valid_char(c)) {
            token->type = TOKEN_EOF;
            return 0;
        }
        
        if (c == ' ' || c == '\n' || c == '\t') {
            if (pos > 0) break;
            continue;
        }
        
        if (pos >= 31) {
            token->type = TOKEN_EOF;
            return 0;
        }
        
        token->value[pos++] = (char)c;
        
        if (!isdigit(c) && !isalpha(c)) {
            token->type = TOKEN_EOF;
            return 0;
        }
    }
    
    if (pos == 0) {
        token->type = TOKEN_EOF;
        return 1;
    }
    
    token->value[pos] = '\0';
    
    int all_digits = 1;
    int all_alpha = 1;
    
    for (int i = 0; i < pos; i++) {
        if (!isdigit(token->value[i])) all_digits = 0;
        if (!isalpha(token->value[i])) all_alpha = 0;
    }
    
    if (all_digits) {
        token->type = TOKEN_NUMBER;
    } else if (all_alpha) {
        token->type = TOKEN_WORD;
    } else {
        token->type = TOKEN_EOF;
        return 0;
    }
    
    return 1;
}

int main(void) {
    FILE *input = stdin;
    struct token current_token;
    enum state current_state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    
    printf("Enter text with numbers and words (alphanumeric only):\n");
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_START:
                if (!get_next_token(input, &current_token)) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                if (current_token.type == TOKEN_NUMBER) {
                    current_state = STATE_READING_NUMBER;
                } else if (current_token.type == TOKEN_WORD) {
                    current_state = STATE_READING_WORD;
                } else if (current_token.type == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                number_count++;
                printf("Found number: %s\n", current_token.value);
                
                if (!get_next_token(input, &current_token)) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                if (current_token.type == TOKEN_NUMBER) {
                    current_state = STATE_READING_NUMBER;
                } else if (current_token.type == TOKEN_WORD) {
                    current_state = STATE_READING_WORD;
                } else if (current_token.type == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                word_count++;
                printf("Found word: %s\n", current_token.value);
                
                if (!get_next_token(input, &current_token)) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                if (current_token.type == TOKEN_NUMBER) {
                    current_state = STATE_READING_NUMBER;
                } else if (current_token.type == TOKEN_WORD) {
                    current_state = STATE_READING_WORD;
                } else if (current_token.type == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input format\n");
                return 1;
                
            case STATE_DONE:
                break;
        }
    }
    
    printf("\nSummary: %d numbers, %d words\n", number_count, word_count);
    
    if (current_state == STATE_ERROR) {
        return 1;
    }
    
    return 0;
}