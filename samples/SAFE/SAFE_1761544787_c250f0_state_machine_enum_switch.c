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

int is_valid_input_char(char c) {
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n' || c == EOF;
}

int read_token(struct Token *token, FILE *input) {
    if (token == NULL || input == NULL) return -1;
    
    int c;
    int index = 0;
    memset(token->value, 0, sizeof(token->value));
    
    while ((c = fgetc(input)) != EOF) {
        if (!is_valid_input_char(c)) return -1;
        if (!isspace(c)) break;
    }
    
    if (c == EOF) {
        token->type = TOKEN_EOF;
        return 0;
    }
    
    if (isdigit(c)) {
        token->type = TOKEN_NUMBER;
        do {
            if (index >= 31) return -1;
            token->value[index++] = (char)c;
            c = fgetc(input);
        } while (isdigit(c) && c != EOF);
        if (c != EOF) ungetc(c, input);
    } else if (isalpha(c)) {
        token->type = TOKEN_WORD;
        do {
            if (index >= 31) return -1;
            token->value[index++] = (char)c;
            c = fgetc(input);
        } while (isalpha(c) && c != EOF);
        if (c != EOF) ungetc(c, input);
    } else {
        return -1;
    }
    
    token->value[index] = '\0';
    return 0;
}

int main(void) {
    enum State current_state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    struct Token token;
    
    printf("Enter text (alphanumeric characters and spaces only):\n");
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_START:
                if (read_token(&token, stdin) != 0) {
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
                if (read_token(&token, stdin) != 0) {
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
                if (read_token(&token, stdin) != 0) {
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
                printf("Error: Invalid input encountered\n");
                return 1;
                
            case STATE_DONE:
                break;
        }
    }
    
    printf("\nSummary:\n");
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}