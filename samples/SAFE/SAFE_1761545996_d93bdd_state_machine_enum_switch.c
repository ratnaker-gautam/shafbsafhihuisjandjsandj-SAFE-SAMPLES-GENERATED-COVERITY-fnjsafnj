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

int get_next_token(FILE *stream, struct Token *token) {
    int c;
    int pos = 0;
    
    while ((c = fgetc(stream)) != EOF) {
        if (!is_valid_char(c)) {
            return -1;
        }
        
        if (c == ' ' || c == '\t' || c == '\n') {
            if (pos > 0) {
                ungetc(c, stream);
                token->value[pos] = '\0';
                return 0;
            }
            continue;
        }
        
        if (isdigit(c)) {
            token->type = TOKEN_NUMBER;
        } else if (isalpha(c)) {
            token->type = TOKEN_WORD;
        } else {
            return -1;
        }
        
        token->value[pos++] = c;
        if (pos >= 31) {
            return -1;
        }
        
        while ((c = fgetc(stream)) != EOF) {
            if (!is_valid_char(c)) {
                return -1;
            }
            
            if (c == ' ' || c == '\t' || c == '\n') {
                ungetc(c, stream);
                break;
            }
            
            if ((token->type == TOKEN_NUMBER && !isdigit(c)) ||
                (token->type == TOKEN_WORD && !isalpha(c))) {
                return -1;
            }
            
            token->value[pos++] = c;
            if (pos >= 31) {
                return -1;
            }
        }
        
        token->value[pos] = '\0';
        return 0;
    }
    
    token->type = TOKEN_EOF;
    token->value[0] = '\0';
    return 0;
}

int main(void) {
    FILE *input = stdin;
    struct Token token;
    enum State current_state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    
    printf("Enter text (alphanumeric words and numbers separated by spaces):\n");
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_START:
                if (get_next_token(input, &token) != 0) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                if (token.type == TOKEN_EOF) {
                    current_state = STATE_END;
                } else if (token.type == TOKEN_NUMBER) {
                    current_state = STATE_READING_NUMBER;
                } else if (token.type == TOKEN_WORD) {
                    current_state = STATE_READING_WORD;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                number_count++;
                printf("Found number: %s\n", token.value);
                current_state = STATE_START;
                break;
                
            case STATE_READING_WORD:
                word_count++;
                printf("Found word: %s\n", token.value);
                current_state = STATE_START;
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Error: Invalid input format\n");
                return 1;
                
            case STATE_END:
                break;
        }
    }
    
    printf("\nSummary: %d numbers, %d words\n", number_count, word_count);
    
    if (current_state == STATE_ERROR) {
        return 1;
    }
    
    return 0;
}