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
    int index = 0;
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
                    if (index < 31) {
                        token->value[index++] = (char)c;
                    }
                    token->type = TOKEN_NUMBER;
                    state = STATE_READING_NUMBER;
                } else if (isalpha(c)) {
                    if (index < 31) {
                        token->value[index++] = (char)c;
                    }
                    token->type = TOKEN_WORD;
                    state = STATE_READING_WORD;
                } else if (c == ' ' || c == '\t' || c == '\n') {
                    continue;
                } else {
                    state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(c)) {
                    if (index < 31) {
                        token->value[index++] = (char)c;
                    }
                } else {
                    if (c != EOF) {
                        ungetc(c, stream);
                    }
                    state = STATE_END;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(c)) {
                    if (index < 31) {
                        token->value[index++] = (char)c;
                    }
                } else {
                    if (c != EOF) {
                        ungetc(c, stream);
                    }
                    state = STATE_END;
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
    
    if (index > 0 && index < 32) {
        token->value[index] = '\0';
    }
    
    return 0;
}

int main(void) {
    FILE *input = stdin;
    struct Token token;
    int number_count = 0;
    int word_count = 0;
    int line_count = 1;
    
    printf("Enter text (Ctrl+D to end):\n");
    
    while (1) {
        int result = get_next_token(input, &token);
        
        if (result == -1) {
            fprintf(stderr, "Error: Invalid character encountered at line %d\n", line_count);
            return 1;
        }
        
        if (token.type == TOKEN_EOF) {
            break;
        }
        
        switch (token.type) {
            case TOKEN_NUMBER:
                printf("Number: %s\n", token.value);
                number_count++;
                break;
            case TOKEN_WORD:
                printf("Word: %s\n", token.value);
                word_count++;
                break;
            case TOKEN_EOF:
                break;
        }
        
        if (strchr(token.value, '\n')) {
            line_count++;
        }
    }
    
    printf("\nSummary:\n");
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    printf("Lines processed: %d\n", line_count);
    
    return 0;
}