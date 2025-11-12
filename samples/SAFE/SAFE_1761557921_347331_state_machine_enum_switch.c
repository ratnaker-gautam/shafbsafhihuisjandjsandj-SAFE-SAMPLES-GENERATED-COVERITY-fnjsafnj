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
    STATE_END
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
    return isalnum(c) || c == ' ' || c == '\n' || c == '\t' || c == '\0';
}

int get_next_token(const char *input, int *pos, int max_len, struct token *tok) {
    if (input == NULL || pos == NULL || tok == NULL || *pos < 0 || max_len <= 0) {
        return -1;
    }
    
    if (*pos >= max_len) {
        tok->type = TOKEN_EOF;
        tok->value[0] = '\0';
        return 0;
    }
    
    while (*pos < max_len && isspace(input[*pos])) {
        (*pos)++;
    }
    
    if (*pos >= max_len) {
        tok->type = TOKEN_EOF;
        tok->value[0] = '\0';
        return 0;
    }
    
    char current = input[*pos];
    if (!is_valid_char(current)) {
        return -1;
    }
    
    if (isdigit(current)) {
        tok->type = TOKEN_NUMBER;
        int i = 0;
        while (*pos < max_len && i < 31 && isdigit(input[*pos])) {
            tok->value[i++] = input[(*pos)++];
        }
        tok->value[i] = '\0';
    } else if (isalpha(current)) {
        tok->type = TOKEN_WORD;
        int i = 0;
        while (*pos < max_len && i < 31 && isalpha(input[*pos])) {
            tok->value[i++] = input[(*pos)++];
        }
        tok->value[i] = '\0';
    } else {
        return -1;
    }
    
    return 0;
}

int main(void) {
    char input[1024];
    printf("Enter text (max 1023 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int input_len = strlen(input);
    if (input_len <= 0 || input_len >= sizeof(input)) {
        fprintf(stderr, "Invalid input length\n");
        return 1;
    }
    
    enum state current_state = STATE_START;
    int pos = 0;
    struct token current_token;
    int number_count = 0;
    int word_count = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_START:
                if (get_next_token(input, &pos, input_len, &current_token) != 0) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                if (current_token.type == TOKEN_NUMBER) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (current_token.type == TOKEN_WORD) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (current_token.type == TOKEN_EOF) {
                    current_state = STATE_END;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                printf("Found number: %s\n", current_token.value);
                
                if (get_next_token(input, &pos, input_len, &current_token) != 0) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                if (current_token.type == TOKEN_NUMBER) {
                    number_count++;
                } else if (current_token.type == TOKEN_WORD) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                } else if (current_token.type == TOKEN_EOF) {
                    current_state = STATE_END;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                printf("Found word: %s\n", current_token.value);
                
                if (get_next_token(input, &pos, input_len, &current_token) != 0) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                if (current_token.type == TOKEN_NUMBER) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                } else if (current_token.type == TOKEN_WORD) {
                    word_count++;
                } else if (current_token.type == TOKEN_EOF) {
                    current_state = STATE_END;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Error processing input at position %d\n", pos);
                break;
                
            case STATE_END:
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Processing failed\n");
        return 1;
    }
    
    printf("Summary: %d numbers, %d words\n", number_count, word_count);
    return 0;
}