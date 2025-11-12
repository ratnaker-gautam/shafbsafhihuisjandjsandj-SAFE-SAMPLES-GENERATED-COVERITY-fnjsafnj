//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
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
    TOKEN_INVALID
};

struct Token {
    enum TokenType type;
    char value[32];
};

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '\n' || c == '\t';
}

enum TokenType classify_token(const char* str) {
    if (str[0] == '\0') return TOKEN_INVALID;
    
    int has_digit = 0;
    int has_alpha = 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (isdigit(str[i])) has_digit = 1;
        if (isalpha(str[i])) has_alpha = 1;
    }
    
    if (has_digit && !has_alpha) return TOKEN_NUMBER;
    if (has_alpha && !has_digit) return TOKEN_WORD;
    return TOKEN_INVALID;
}

int main(void) {
    char input[1024];
    printf("Enter text (max 1023 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
        len--;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_char(input[i])) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    enum State current_state = STATE_START;
    char buffer[32];
    int buffer_index = 0;
    int token_count = 0;
    struct Token tokens[100];
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        
        switch (current_state) {
            case STATE_START:
                if (c == '\0') {
                    current_state = STATE_END;
                } else if (isdigit(c)) {
                    if (buffer_index < 31) {
                        buffer[buffer_index++] = c;
                        current_state = STATE_READING_NUMBER;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isalpha(c)) {
                    if (buffer_index < 31) {
                        buffer[buffer_index++] = c;
                        current_state = STATE_READING_WORD;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (c != ' ' && c != '\t') {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (c == '\0' || c == ' ' || c == '\t') {
                    buffer[buffer_index] = '\0';
                    enum TokenType type = classify_token(buffer);
                    if (type == TOKEN_NUMBER && token_count < 100) {
                        tokens[token_count].type = type;
                        strncpy(tokens[token_count].value, buffer, 31);
                        tokens[token_count].value[31] = '\0';
                        token_count++;
                    }
                    buffer_index = 0;
                    current_state = (c == '\0') ? STATE_END : STATE_START;
                } else if (isdigit(c)) {
                    if (buffer_index < 31) {
                        buffer[buffer_index++] = c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (c == '\0' || c == ' ' || c == '\t') {
                    buffer[buffer_index] = '\0';
                    enum TokenType type = classify_token(buffer);
                    if (type == TOKEN_WORD && token_count < 100) {
                        tokens[token_count].type = type;
                        strncpy(tokens[token_count].value, buffer, 31);
                        tokens[token_count].value[31] = '\0';
                        token_count++;
                    }
                    buffer_index = 0;
                    current_state = (c == '\0') ? STATE_END : STATE_START;
                } else if (isalnum(c)) {
                    if (buffer_index < 31) {
                        buffer[buffer_index++] = c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Error: Invalid input format\n");
                return 1;
                
            case STATE_END:
                break;
        }
        
        if (current_state == STATE_ERROR) {
            fprintf(stderr, "Error: Invalid input format\n");
            return 1;
        }
    }
    
    printf("Found %d tokens:\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: ", i + 1);
        switch (tokens[i].type) {
            case TOKEN_NUMBER:
                printf("NUMBER '%s'\n", tokens[i].value);
                break;
            case TOKEN