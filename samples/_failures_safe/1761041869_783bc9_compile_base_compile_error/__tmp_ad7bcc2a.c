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

int process_input(const char* input, struct Token* tokens, int max_tokens) {
    if (!input || !tokens || max_tokens <= 0) return -1;
    
    enum State current_state = STATE_START;
    int token_count = 0;
    int value_index = 0;
    char current_value[32] = {0};
    
    for (int i = 0; input[i] != '\0' && token_count < max_tokens; i++) {
        char c = input[i];
        
        if (!is_valid_char(c)) {
            current_state = STATE_ERROR;
            break;
        }
        
        switch (current_state) {
            case STATE_START:
                if (isdigit(c)) {
                    current_state = STATE_READING_NUMBER;
                    value_index = 0;
                    if (value_index < 31) current_value[value_index++] = c;
                } else if (isalpha(c)) {
                    current_state = STATE_READING_WORD;
                    value_index = 0;
                    if (value_index < 31) current_value[value_index++] = c;
                } else if (c == ' ' || c == '\t' || c == '\n') {
                    continue;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(c)) {
                    if (value_index < 31) current_value[value_index++] = c;
                } else if (isalpha(c)) {
                    current_state = STATE_ERROR;
                } else if (c == ' ' || c == '\t' || c == '\n') {
                    current_value[value_index] = '\0';
                    tokens[token_count].type = TOKEN_NUMBER;
                    strncpy(tokens[token_count].value, current_value, 31);
                    tokens[token_count].value[31] = '\0';
                    token_count++;
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalnum(c)) {
                    if (value_index < 31) current_value[value_index++] = c;
                } else if (c == ' ' || c == '\t' || c == '\n') {
                    current_value[value_index] = '\0';
                    tokens[token_count].type = TOKEN_WORD;
                    strncpy(tokens[token_count].value, current_value, 31);
                    tokens[token_count].value[31] = '\0';
                    token_count++;
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                break;
                
            case STATE_END:
                break;
        }
        
        if (current_state == STATE_ERROR) break;
    }
    
    if (current_state == STATE_READING_NUMBER || current_state == STATE_READING_WORD) {
        if (value_index > 0 && token_count < max_tokens) {
            current_value[value_index] = '\0';
            tokens[token_count].type = (current_state == STATE_READING_NUMBER) ? TOKEN_NUMBER : TOKEN_WORD;
            strncpy(tokens[token_count].value, current_value, 31);
            tokens[token_count].value[31] = '\0';
            token_count++;
        }
    }
    
    return (current_state == STATE_ERROR) ? -1 : token_count;
}

void print_tokens(const struct Token* tokens, int count) {
    if (!tokens || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        switch (tokens[i].type) {
            case TOKEN_NUMBER:
                printf("NUMBER: %s\n", tokens[i].value);
                break;
            case TOKEN_WORD:
                printf("WORD: %s\n", tokens[i].value);
                break;
            case TOKEN_INVALID:
                printf("INVALID\n");
                break;
        }
    }
}

int main() {
    char input[256];
    struct Token tokens[50];
    
    printf("Enter text (alphanumeric only): ");
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (len == 0 || len >= sizeof(input)) {
        fprintf(stderr, "Invalid input length\n");
        return 1;
    }
    
    int token_count = process_input(input, tokens, 50);
    
    if (token_count < 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    printf("Tokens found: %d\n", token_count);