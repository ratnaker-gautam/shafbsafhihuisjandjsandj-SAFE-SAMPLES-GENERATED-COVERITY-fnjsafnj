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

int parse_token(const char *input, int pos, struct Token *token) {
    if (input[pos] == '\0') {
        token->type = TOKEN_END;
        return pos;
    }
    
    int start = pos;
    while (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\n') {
        pos++;
    }
    
    if (input[pos] == '\0') {
        token->type = TOKEN_END;
        return pos;
    }
    
    start = pos;
    if (isdigit(input[pos])) {
        token->type = TOKEN_NUMBER;
        int len = 0;
        while (isdigit(input[pos]) && len < 31) {
            token->value[len++] = input[pos++];
        }
        token->value[len] = '\0';
    } else if (isalpha(input[pos])) {
        token->type = TOKEN_WORD;
        int len = 0;
        while (isalpha(input[pos]) && len < 31) {
            token->value[len++] = input[pos++];
        }
        token->value[len] = '\0';
    } else {
        return -1;
    }
    
    return pos;
}

int main(void) {
    char input[1024];
    printf("Enter text (alphanumeric only): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    for (int i = 0; input[i] != '\0'; i++) {
        if (!is_valid_input_char(input[i])) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    enum State current_state = STATE_START;
    int position = 0;
    int number_count = 0;
    int word_count = 0;
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        struct Token token;
        int new_pos = parse_token(input, position, &token);
        
        if (new_pos == -1) {
            current_state = STATE_ERROR;
            break;
        }
        
        position = new_pos;
        
        switch (current_state) {
            case STATE_START:
                switch (token.type) {
                    case TOKEN_NUMBER:
                        current_state = STATE_READING_NUMBER;
                        number_count++;
                        break;
                    case TOKEN_WORD:
                        current_state = STATE_READING_WORD;
                        word_count++;
                        break;
                    case TOKEN_END:
                        current_state = STATE_DONE;
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_NUMBER:
                switch (token.type) {
                    case TOKEN_NUMBER:
                        number_count++;
                        break;
                    case TOKEN_WORD:
                        current_state = STATE_READING_WORD;
                        word_count++;
                        break;
                    case TOKEN_END:
                        current_state = STATE_DONE;
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_WORD:
                switch (token.type) {
                    case TOKEN_NUMBER:
                        current_state = STATE_READING_NUMBER;
                        number_count++;
                        break;
                    case TOKEN_WORD:
                        word_count++;
                        break;
                    case TOKEN_END:
                        current_state = STATE_DONE;
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            default:
                current_state = STATE_ERROR;
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Error parsing input at position %d\n", position);
        return 1;
    }
    
    printf("Parsing completed successfully\n");
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}