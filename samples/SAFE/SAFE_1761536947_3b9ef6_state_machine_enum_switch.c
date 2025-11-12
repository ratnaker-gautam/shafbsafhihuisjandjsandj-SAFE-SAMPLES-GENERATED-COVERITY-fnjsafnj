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
    STATE_DONE
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

int get_next_token(struct Token *token) {
    static char buffer[1024];
    static size_t pos = 0;
    static int initialized = 0;
    
    if (!initialized) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        initialized = 1;
    }
    
    while (pos < sizeof(buffer) && buffer[pos] != '\0') {
        char c = buffer[pos];
        
        if (!is_valid_char(c)) {
            return -1;
        }
        
        if (c == ' ' || c == '\t' || c == '\n') {
            pos++;
            continue;
        }
        
        if (isdigit(c)) {
            size_t i = 0;
            while (pos < sizeof(buffer) && isdigit(buffer[pos]) && i < sizeof(token->value) - 1) {
                token->value[i++] = buffer[pos++];
            }
            token->value[i] = '\0';
            token->type = TOKEN_NUMBER;
            return 0;
        }
        
        if (isalpha(c)) {
            size_t i = 0;
            while (pos < sizeof(buffer) && isalpha(buffer[pos]) && i < sizeof(token->value) - 1) {
                token->value[i++] = buffer[pos++];
            }
            token->value[i] = '\0';
            token->type = TOKEN_WORD;
            return 0;
        }
        
        pos++;
    }
    
    token->type = TOKEN_EOF;
    token->value[0] = '\0';
    return 0;
}

int main(void) {
    enum State current_state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    struct Token token;
    
    printf("Enter text (alphanumeric only): ");
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_START:
                if (get_next_token(&token) != 0) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                if (token.type == TOKEN_NUMBER) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                    printf("Found number: %s\n", token.value);
                } else if (token.type == TOKEN_WORD) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                    printf("Found word: %s\n", token.value);
                } else if (token.type == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (get_next_token(&token) != 0) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                if (token.type == TOKEN_NUMBER) {
                    number_count++;
                    printf("Found number: %s\n", token.value);
                } else if (token.type == TOKEN_WORD) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                    printf("Found word: %s\n", token.value);
                } else if (token.type == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (get_next_token(&token) != 0) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                if (token.type == TOKEN_NUMBER) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                    printf("Found number: %s\n", token.value);
                } else if (token.type == TOKEN_WORD) {
                    word_count++;
                    printf("Found word: %s\n", token.value);
                } else if (token.type == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input detected\n");
                return 1;
                
            case STATE_DONE:
                break;
        }
    }
    
    printf("Summary: %d numbers, %d words\n", number_count, word_count);
    return 0;
}