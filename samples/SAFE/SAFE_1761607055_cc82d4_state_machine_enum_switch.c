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
    TOKEN_END
};

struct Token {
    enum TokenType type;
    char value[32];
};

int is_valid_input_char(char c) {
    return isalnum(c) || c == ' ' || c == '\n' || c == '\t';
}

int get_next_token(const char *input, int *pos, int max_len, struct Token *token) {
    if (*pos >= max_len || input[*pos] == '\0') {
        token->type = TOKEN_END;
        return 1;
    }
    
    while (*pos < max_len && (input[*pos] == ' ' || input[*pos] == '\t' || input[*pos] == '\n')) {
        (*pos)++;
    }
    
    if (*pos >= max_len || input[*pos] == '\0') {
        token->type = TOKEN_END;
        return 1;
    }
    
    int start = *pos;
    if (isdigit(input[*pos])) {
        token->type = TOKEN_NUMBER;
        int i = 0;
        while (*pos < max_len && i < 31 && isdigit(input[*pos])) {
            token->value[i++] = input[*pos];
            (*pos)++;
        }
        token->value[i] = '\0';
    } else if (isalpha(input[*pos])) {
        token->type = TOKEN_WORD;
        int i = 0;
        while (*pos < max_len && i < 31 && isalpha(input[*pos])) {
            token->value[i++] = input[*pos];
            (*pos)++;
        }
        token->value[i] = '\0';
    } else {
        return 0;
    }
    
    return 1;
}

int validate_input(const char *input, int max_len) {
    if (input == NULL || max_len <= 0) {
        return 0;
    }
    
    for (int i = 0; i < max_len && input[i] != '\0'; i++) {
        if (!is_valid_input_char(input[i])) {
            return 0;
        }
    }
    
    return 1;
}

int main(void) {
    char input_buffer[1024];
    printf("Enter text (alphanumeric only): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int input_len = strnlen(input_buffer, sizeof(input_buffer));
    if (!validate_input(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input characters detected\n");
        return 1;
    }
    
    enum State current_state = STATE_START;
    int position = 0;
    int number_count = 0;
    int word_count = 0;
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        struct Token token;
        
        switch (current_state) {
            case STATE_START:
                if (!get_next_token(input_buffer, &position, input_len, &token)) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                if (token.type == TOKEN_NUMBER) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (token.type == TOKEN_WORD) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (token.type == TOKEN_END) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (!get_next_token(input_buffer, &position, input_len, &token)) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                if (token.type == TOKEN_NUMBER) {
                    number_count++;
                } else if (token.type == TOKEN_WORD) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (token.type == TOKEN_END) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (!get_next_token(input_buffer, &position, input_len, &token)) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                if (token.type == TOKEN_NUMBER) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (token.type == TOKEN_WORD) {
                    word_count++;
                } else if (token.type == TOKEN_END) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Error processing input at position %d\n", position);
                break;
                
            case STATE_DONE:
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        return 1;
    }
    
    printf("Processing complete:\n");
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}