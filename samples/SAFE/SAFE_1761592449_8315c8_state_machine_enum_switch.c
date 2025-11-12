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
    STATE_DONE
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
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n' || c == EOF;
}

int get_next_token(struct token *tok) {
    static char input[1024];
    static size_t pos = 0;
    static int initialized = 0;
    
    if (!initialized) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            return -1;
        }
        initialized = 1;
    }
    
    while (input[pos] != '\0' && isspace(input[pos])) {
        pos++;
    }
    
    if (input[pos] == '\0') {
        tok->type = TOKEN_EOF;
        return 0;
    }
    
    if (!is_valid_char(input[pos])) {
        return -1;
    }
    
    if (isdigit(input[pos])) {
        tok->type = TOKEN_NUMBER;
        size_t i = 0;
        while (isdigit(input[pos]) && i < sizeof(tok->value) - 1) {
            tok->value[i++] = input[pos++];
        }
        tok->value[i] = '\0';
    } else if (isalpha(input[pos])) {
        tok->type = TOKEN_WORD;
        size_t i = 0;
        while (isalpha(input[pos]) && i < sizeof(tok->value) - 1) {
            tok->value[i++] = input[pos++];
        }
        tok->value[i] = '\0';
    } else {
        return -1;
    }
    
    return 0;
}

int main(void) {
    enum state current_state = STATE_START;
    struct token tok;
    int number_count = 0;
    int word_count = 0;
    
    printf("Enter text (alphanumeric only): ");
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_START:
                if (get_next_token(&tok) != 0) {
                    current_state = STATE_ERROR;
                    break;
                }
                if (tok.type == TOKEN_NUMBER) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (tok.type == TOKEN_WORD) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (tok.type == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (get_next_token(&tok) != 0) {
                    current_state = STATE_ERROR;
                    break;
                }
                if (tok.type == TOKEN_NUMBER) {
                    number_count++;
                } else if (tok.type == TOKEN_WORD) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (tok.type == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (get_next_token(&tok) != 0) {
                    current_state = STATE_ERROR;
                    break;
                }
                if (tok.type == TOKEN_NUMBER) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (tok.type == TOKEN_WORD) {
                    word_count++;
                } else if (tok.type == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input character\n");
                return 1;
                
            case STATE_DONE:
                break;
        }
    }
    
    if (current_state == STATE_DONE) {
        printf("Numbers found: %d\n", number_count);
        printf("Words found: %d\n", word_count);
    }
    
    return 0;
}