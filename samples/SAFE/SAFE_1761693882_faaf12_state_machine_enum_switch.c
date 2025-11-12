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
    STATE_END
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
    static char buffer[32];
    int pos = 0;
    int c;
    enum state current_state = STATE_START;
    
    memset(buffer, 0, sizeof(buffer));
    memset(tok->value, 0, sizeof(tok->value));
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        c = fgetc(input);
        if (c == EOF && current_state == STATE_START) {
            tok->type = TOKEN_EOF;
            current_state = STATE_END;
            break;
        }
        
        if (!is_valid_char(c) && c != EOF) {
            current_state = STATE_ERROR;
            break;
        }
        
        switch (current_state) {
            case STATE_START:
                if (isdigit(c)) {
                    buffer[pos++] = (char)c;
                    current_state = STATE_READING_NUMBER;
                } else if (isalpha(c)) {
                    buffer[pos++] = (char)c;
                    current_state = STATE_READING_WORD;
                } else if (c == ' ' || c == '\t' || c == '\n') {
                    continue;
                } else if (c == EOF) {
                    tok->type = TOKEN_EOF;
                    current_state = STATE_END;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(c)) {
                    if (pos < 31) {
                        buffer[pos++] = (char)c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else {
                    if (c != EOF) ungetc(c, input);
                    strncpy(tok->value, buffer, 31);
                    tok->type = TOKEN_NUMBER;
                    current_state = STATE_END;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(c)) {
                    if (pos < 31) {
                        buffer[pos++] = (char)c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else {
                    if (c != EOF) ungetc(c, input);
                    strncpy(tok->value, buffer, 31);
                    tok->type = TOKEN_WORD;
                    current_state = STATE_END;
                }
                break;
                
            default:
                current_state = STATE_ERROR;
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        return -1;
    }
    
    return 0;
}

int main(void) {
    struct token tok;
    int token_count = 0;
    int number_count = 0;
    int word_count = 0;
    
    printf("Enter text (alphanumeric only, Ctrl+D to end):\n");
    
    while (1) {
        int result = get_next_token(&tok, stdin);
        if (result == -1) {
            printf("Error: Invalid input character encountered\n");
            return 1;
        }
        
        if (tok.type == TOKEN_EOF) {
            break;
        }
        
        token_count++;
        
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
    
    printf("\nSummary:\n");
    printf("Total tokens: %d\n", token_count);
    printf("Numbers: %d\n", number_count);
    printf("Words: %d\n", word_count);
    
    return 0;
}