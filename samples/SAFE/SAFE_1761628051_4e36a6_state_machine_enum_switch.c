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

int get_token(struct Token *token) {
    static char buffer[1024];
    static size_t pos = 0;
    static int initialized = 0;
    
    if (!initialized) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            token->type = TOKEN_EOF;
            strcpy(token->value, "");
            return 1;
        }
        initialized = 1;
    }
    
    while (pos < sizeof(buffer) && buffer[pos] != '\0') {
        char c = buffer[pos];
        
        if (c == ' ' || c == '\t' || c == '\n') {
            pos++;
            continue;
        }
        
        if (isdigit(c)) {
            size_t i = 0;
            while (pos < sizeof(buffer) && i < sizeof(token->value) - 1 && 
                   isdigit(buffer[pos])) {
                token->value[i++] = buffer[pos++];
            }
            token->value[i] = '\0';
            token->type = TOKEN_NUMBER;
            return 1;
        }
        
        if (isalpha(c)) {
            size_t i = 0;
            while (pos < sizeof(buffer) && i < sizeof(token->value) - 1 && 
                   isalpha(buffer[pos])) {
                token->value[i++] = buffer[pos++];
            }
            token->value[i] = '\0';
            token->type = TOKEN_WORD;
            return 1;
        }
        
        if (c == EOF || c == '\0') {
            token->type = TOKEN_EOF;
            strcpy(token->value, "");
            return 1;
        }
        
        pos++;
    }
    
    token->type = TOKEN_EOF;
    strcpy(token->value, "");
    return 1;
}

int main(void) {
    enum State current_state = STATE_START;
    struct Token token;
    int number_count = 0;
    int word_count = 0;
    
    printf("Enter text (alphanumeric characters only): ");
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        if (!get_token(&token)) {
            current_state = STATE_ERROR;
            break;
        }
        
        switch (current_state) {
            case STATE_START:
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
                if (token.type == TOKEN_WORD) {
                    word_count++;
                    printf("Found word: %s\n", token.value);
                } else if (token.type == TOKEN_NUMBER) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                    printf("Found number: %s\n", token.value);
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
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("\nSummary:\n");
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}