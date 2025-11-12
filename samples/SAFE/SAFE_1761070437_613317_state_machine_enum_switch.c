//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum state {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_ERROR,
    STATE_DONE
};

enum token_type {
    TOKEN_WORD,
    TOKEN_NUMBER,
    TOKEN_EOF
};

struct token {
    enum token_type type;
    char value[32];
};

int is_valid_input_char(char c) {
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n' || c == EOF;
}

int get_next_char(void) {
    int c = getchar();
    if (c != EOF && !is_valid_input_char(c)) {
        return -1;
    }
    return c;
}

int classify_token(const char *str) {
    if (str[0] == '\0') return -1;
    
    int has_digit = 0;
    int has_alpha = 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (isdigit(str[i])) {
            has_digit = 1;
        } else if (isalpha(str[i])) {
            has_alpha = 1;
        } else {
            return -1;
        }
    }
    
    if (has_alpha && !has_digit) return TOKEN_WORD;
    if (has_digit && !has_alpha) return TOKEN_NUMBER;
    return -1;
}

int main(void) {
    enum state current_state = STATE_START;
    struct token tokens[100];
    int token_count = 0;
    char buffer[32];
    int buf_pos = 0;
    int c;
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_START:
                c = get_next_char();
                if (c == -1) {
                    current_state = STATE_ERROR;
                } else if (c == EOF) {
                    current_state = STATE_DONE;
                } else if (isalpha(c)) {
                    buffer[0] = c;
                    buf_pos = 1;
                    current_state = STATE_READING_WORD;
                } else if (isdigit(c)) {
                    buffer[0] = c;
                    buf_pos = 1;
                    current_state = STATE_READING_NUMBER;
                } else if (isspace(c)) {
                    current_state = STATE_START;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                c = get_next_char();
                if (c == -1) {
                    current_state = STATE_ERROR;
                } else if (isalpha(c)) {
                    if (buf_pos < 31) {
                        buffer[buf_pos++] = c;
                        current_state = STATE_READING_WORD;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isspace(c) || c == EOF) {
                    buffer[buf_pos] = '\0';
                    int token_type = classify_token(buffer);
                    if (token_type == TOKEN_WORD && token_count < 100) {
                        tokens[token_count].type = TOKEN_WORD;
                        strncpy(tokens[token_count].value, buffer, 31);
                        tokens[token_count].value[31] = '\0';
                        token_count++;
                        current_state = (c == EOF) ? STATE_DONE : STATE_START;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                c = get_next_char();
                if (c == -1) {
                    current_state = STATE_ERROR;
                } else if (isdigit(c)) {
                    if (buf_pos < 31) {
                        buffer[buf_pos++] = c;
                        current_state = STATE_READING_NUMBER;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isspace(c) || c == EOF) {
                    buffer[buf_pos] = '\0';
                    int token_type = classify_token(buffer);
                    if (token_type == TOKEN_NUMBER && token_count < 100) {
                        tokens[token_count].type = TOKEN_NUMBER;
                        strncpy(tokens[token_count].value, buffer, 31);
                        tokens[token_count].value[31] = '\0';
                        token_count++;
                        current_state = (c == EOF) ? STATE_DONE : STATE_START;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input format\n");
                return 1;
                
            case STATE_DONE:
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        return 1;
    }
    
    printf("Parsed tokens:\n");
    for (int i = 0; i < token_count; i++) {
        switch (tokens[i].type) {
            case TOKEN_WORD:
                printf("WORD: %s\n", tokens[i].value);
                break;
            case TOKEN_NUMBER:
                printf("NUMBER: %s\n", tokens[i].value);
                break;
            case TOKEN_EOF:
                break;
        }
    }
    
    return 0;
}