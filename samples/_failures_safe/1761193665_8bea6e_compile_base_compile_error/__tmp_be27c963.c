//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_END,
    STATE_ERROR
};

enum TokenType {
    TOKEN_WORD,
    TOKEN_NUMBER,
    TOKEN_END,
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
};

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n' || c == '\0';
}

enum TokenType classify_token(const char* str) {
    if (str[0] == '\0') return TOKEN_END;
    
    int has_alpha = 0;
    int has_digit = 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (isalpha(str[i])) has_alpha = 1;
        if (isdigit(str[i])) has_digit = 1;
    }
    
    if (has_alpha && !has_digit) return TOKEN_WORD;
    if (has_digit && !has_alpha) return TOKEN_NUMBER;
    return TOKEN_INVALID;
}

int get_next_token(struct Token* token, const char* input, int* pos) {
    int len = strlen(input);
    if (*pos >= len) {
        token->type = TOKEN_END;
        token->value[0] = '\0';
        return 1;
    }
    
    while (*pos < len && (input[*pos] == ' ' || input[*pos] == '\t')) {
        (*pos)++;
    }
    
    if (*pos >= len) {
        token->type = TOKEN_END;
        token->value[0] = '\0';
        return 1;
    }
    
    int start = *pos;
    while (*pos < len && input[*pos] != ' ' && input[*pos] != '\t') {
        (*pos)++;
    }
    
    int token_len = *pos - start;
    if (token_len >= 32) {
        token_len = 31;
    }
    
    strncpy(token->value, input + start, token_len);
    token->value[token_len] = '\0';
    
    for (int i = 0; i < token_len; i++) {
        if (!is_valid_char(token->value[i])) {
            token->type = TOKEN_INVALID;
            return 1;
        }
    }
    
    token->type = classify_token(token->value);
    return 1;
}

int main(void) {
    char input[256];
    printf("Enter text (words and numbers): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    int len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    for (int i = 0; input[i] != '\0'; i++) {
        if (!is_valid_char(input[i])) {
            printf("Invalid character in input\n");
            return 1;
        }
    }
    
    enum State current_state = STATE_START;
    int pos = 0;
    int word_count = 0;
    int number_count = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        struct Token token;
        
        switch (current_state) {
            case STATE_START:
                if (!get_next_token(&token, input, &pos)) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                switch (token.type) {
                    case TOKEN_WORD:
                        current_state = STATE_READING_WORD;
                        word_count++;
                        break;
                    case TOKEN_NUMBER:
                        current_state = STATE_READING_NUMBER;
                        number_count++;
                        break;
                    case TOKEN_END:
                        current_state = STATE_END;
                        break;
                    case TOKEN_INVALID:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_WORD:
                if (!get_next_token(&token, input, &pos)) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                switch (token.type) {
                    case TOKEN_WORD:
                        word_count++;
                        break;
                    case TOKEN_NUMBER:
                        current_state = STATE_READING_NUMBER;
                        number_count++;
                        break;
                    case TOKEN_END:
                        current_state = STATE_END;
                        break;
                    case TOKEN_INVALID:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (!get_next_token(&token, input, &pos)) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                switch (token.type) {
                    case TOKEN_WORD:
                        current_state = STATE_READING_WORD;
                        word_count++;
                        break;
                    case TOKEN_NUMBER:
                        number_count++;
                        break;
                    case TOKEN_END:
                        current_state = STATE_END;
                        break;
                    case TOKEN_INVALID:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_END:
            case STATE_ERROR:
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format