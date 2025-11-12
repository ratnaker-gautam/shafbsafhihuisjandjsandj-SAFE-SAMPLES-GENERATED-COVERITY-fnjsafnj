//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
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
    TOKEN_END
};

struct token {
    enum token_type type;
    char value[32];
};

int is_valid_input_char(char c) {
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n' || c == '\0';
}

int get_next_token(const char *input, int pos, struct token *tok) {
    if (input == NULL || tok == NULL || pos < 0) return -1;
    
    int len = strlen(input);
    if (pos >= len) {
        tok->type = TOKEN_END;
        tok->value[0] = '\0';
        return pos;
    }
    
    while (pos < len && (input[pos] == ' ' || input[pos] == '\t')) {
        pos++;
    }
    
    if (pos >= len) {
        tok->type = TOKEN_END;
        tok->value[0] = '\0';
        return pos;
    }
    
    int i = 0;
    if (isalpha(input[pos])) {
        tok->type = TOKEN_WORD;
        while (pos < len && isalpha(input[pos]) && i < 31) {
            tok->value[i++] = input[pos++];
        }
    } else if (isdigit(input[pos])) {
        tok->type = TOKEN_NUMBER;
        while (pos < len && isdigit(input[pos]) && i < 31) {
            tok->value[i++] = input[pos++];
        }
    } else {
        return -1;
    }
    
    tok->value[i] = '\0';
    return pos;
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    
    int len = strlen(input);
    if (len > 1000) return 0;
    
    for (int i = 0; i < len; i++) {
        if (!is_valid_input_char(input[i])) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    char input[1001];
    
    printf("Enter text (letters and numbers only, max 1000 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input: only letters, numbers, and spaces allowed\n");
        return 1;
    }
    
    enum state current_state = STATE_START;
    int pos = 0;
    int word_count = 0;
    int number_count = 0;
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        struct token tok;
        int new_pos = get_next_token(input, pos, &tok);
        
        if (new_pos == -1) {
            current_state = STATE_ERROR;
            break;
        }
        
        switch (current_state) {
            case STATE_START:
                if (tok.type == TOKEN_WORD) {
                    word_count++;
                    current_state = STATE_READING_WORD;
                } else if (tok.type == TOKEN_NUMBER) {
                    number_count++;
                    current_state = STATE_READING_NUMBER;
                } else if (tok.type == TOKEN_END) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (tok.type == TOKEN_WORD) {
                    word_count++;
                } else if (tok.type == TOKEN_NUMBER) {
                    number_count++;
                    current_state = STATE_READING_NUMBER;
                } else if (tok.type == TOKEN_END) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (tok.type == TOKEN_NUMBER) {
                    number_count++;
                } else if (tok.type == TOKEN_WORD) {
                    word_count++;
                    current_state = STATE_READING_WORD;
                } else if (tok.type == TOKEN_END) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            default:
                current_state = STATE_ERROR;
                break;
        }
        
        pos = new_pos;
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    
    return 0;
}