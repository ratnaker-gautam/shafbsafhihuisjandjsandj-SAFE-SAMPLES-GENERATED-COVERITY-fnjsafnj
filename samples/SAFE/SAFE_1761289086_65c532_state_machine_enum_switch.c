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
    enum State state = STATE_START;
    
    memset(token->value, 0, sizeof(token->value));
    
    while (state != STATE_END && state != STATE_ERROR) {
        c = fgetc(stream);
        
        if (!is_valid_char(c) && c != EOF) {
            state = STATE_ERROR;
            break;
        }
        
        switch (state) {
            case STATE_START:
                if (c == EOF) {
                    token->type = TOKEN_EOF;
                    state = STATE_END;
                } else if (isdigit(c)) {
                    if (pos < 31) {
                        token->value[pos++] = c;
                    }
                    state = STATE_READING_NUMBER;
                } else if (isalpha(c)) {
                    if (pos < 31) {
                        token->value[pos++] = c;
                    }
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
                        token->value[pos++] = c;
                    }
                } else if (c == ' ' || c == '\t' || c == '\n' || c == EOF) {
                    token->type = TOKEN_NUMBER;
                    if (c != EOF) {
                        ungetc(c, stream);
                    }
                    state = STATE_END;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(c)) {
                    if (pos < 31) {
                        token->value[pos++] = c;
                    }
                } else if (c == ' ' || c == '\t' || c == '\n' || c == EOF) {
                    token->type = TOKEN_WORD;
                    if (c != EOF) {
                        ungetc(c, stream);
                    }
                    state = STATE_END;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            default:
                state = STATE_ERROR;
                break;
        }
    }
    
    if (state == STATE_ERROR) {
        return -1;
    }
    
    if (pos > 0 && pos < 32) {
        token->value[pos] = '\0';
    }
    
    return 0;
}

int main(void) {
    FILE *input = stdin;
    struct Token token;
    int number_count = 0;
    int word_count = 0;
    int error_count = 0;
    
    printf("Enter text (alphanumeric characters only):\n");
    
    while (1) {
        if (get_next_token(input, &token) != 0) {
            error_count++;
            printf("Error: Invalid character sequence\n");
            break;
        }
        
        switch (token.type) {
            case TOKEN_NUMBER:
                number_count++;
                printf("Number: %s\n", token.value);
                break;
            case TOKEN_WORD:
                word_count++;
                printf("Word: %s\n", token.value);
                break;
            case TOKEN_EOF:
                printf("\nSummary: %d numbers, %d words, %d errors\n", 
                       number_count, word_count, error_count);
                return 0;
            default:
                error_count++;
                printf("Error: Unknown token type\n");
                break;
        }
    }
    
    return 1;
}