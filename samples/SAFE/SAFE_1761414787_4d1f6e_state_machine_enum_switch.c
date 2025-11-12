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
    return isalnum(c) || c == ' ' || c == '\n' || c == '\t' || c == '\0';
}

int parse_token(const char *input, int *pos, struct Token *token) {
    int start = *pos;
    int len = strlen(input);
    
    if (start >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        return 1;
    }
    
    while (start < len && isspace(input[start])) {
        start++;
    }
    
    if (start >= len) {
        token->type = TOKEN_EOF;
        token->value[0] = '\0';
        *pos = start;
        return 1;
    }
    
    int i = 0;
    if (isdigit(input[start])) {
        token->type = TOKEN_NUMBER;
        while (start < len && isdigit(input[start]) && i < 31) {
            token->value[i++] = input[start++];
        }
    } else if (isalpha(input[start])) {
        token->type = TOKEN_WORD;
        while (start < len && isalpha(input[start]) && i < 31) {
            token->value[i++] = input[start++];
        }
    } else {
        return 0;
    }
    
    token->value[i] = '\0';
    *pos = start;
    return 1;
}

void process_tokens(const char *input) {
    enum State current_state = STATE_START;
    int pos = 0;
    struct Token token;
    int number_count = 0;
    int word_count = 0;
    
    printf("Processing input: %s\n", input);
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_START:
                if (!parse_token(input, &pos, &token)) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                switch (token.type) {
                    case TOKEN_NUMBER:
                        current_state = STATE_READING_NUMBER;
                        number_count++;
                        printf("Found number: %s\n", token.value);
                        break;
                    case TOKEN_WORD:
                        current_state = STATE_READING_WORD;
                        word_count++;
                        printf("Found word: %s\n", token.value);
                        break;
                    case TOKEN_EOF:
                        current_state = STATE_END;
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_NUMBER:
            case STATE_READING_WORD:
                if (!parse_token(input, &pos, &token)) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                switch (token.type) {
                    case TOKEN_NUMBER:
                        current_state = STATE_READING_NUMBER;
                        number_count++;
                        printf("Found number: %s\n", token.value);
                        break;
                    case TOKEN_WORD:
                        current_state = STATE_READING_WORD;
                        word_count++;
                        printf("Found word: %s\n", token.value);
                        break;
                    case TOKEN_EOF:
                        current_state = STATE_END;
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input format\n");
                break;
                
            case STATE_END:
                break;
        }
    }
    
    if (current_state == STATE_END) {
        printf("Processing complete. Numbers: %d, Words: %d\n", number_count, word_count);
    }
}

int main() {
    char input[256];
    
    printf("Enter text to analyze (max 255 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("Empty input\n");
        return 1;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_char(input[i])) {
            printf("Error: Invalid character in input\n");
            return 1;
        }
    }
    
    process_tokens(input);
    return 0;
}