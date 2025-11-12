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

int is_valid_input_char(char c) {
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n' || c == EOF;
}

int get_next_char(FILE *stream) {
    int c = fgetc(stream);
    if (c == EOF) return EOF;
    if (!is_valid_input_char(c)) return -1;
    return c;
}

int parse_token(FILE *input, struct token *token) {
    int c;
    int index = 0;
    enum state current_state = STATE_START;
    
    memset(token->value, 0, sizeof(token->value));
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        c = get_next_char(input);
        
        switch (current_state) {
            case STATE_START:
                if (c == EOF) {
                    token->type = TOKEN_EOF;
                    current_state = STATE_DONE;
                } else if (isdigit(c)) {
                    if (index < 31) {
                        token->value[index++] = c;
                        token->type = TOKEN_NUMBER;
                        current_state = STATE_READING_NUMBER;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (isalpha(c)) {
                    if (index < 31) {
                        token->value[index++] = tolower(c);
                        token->type = TOKEN_WORD;
                        current_state = STATE_READING_WORD;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\n') {
                    continue;
                } else if (c == -1) {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(c)) {
                    if (index < 31) {
                        token->value[index++] = c;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\n' || c == EOF) {
                    if (c != EOF) ungetc(c, input);
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(c)) {
                    if (index < 31) {
                        token->value[index++] = tolower(c);
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else if (c == ' ' || c == '\t' || c == '\n' || c == EOF) {
                    if (c != EOF) ungetc(c, input);
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            default:
                current_state = STATE_ERROR;
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        return -1;
    }
    
    if (index > 0) {
        token->value[index] = '\0';
    }
    
    return 0;
}

int main(void) {
    FILE *input = stdin;
    struct token current_token;
    int number_count = 0;
    int word_count = 0;
    int error_occurred = 0;
    
    printf("Enter text (alphanumeric characters and spaces only):\n");
    
    while (!error_occurred) {
        if (parse_token(input, &current_token) != 0) {
            printf("Error: Invalid input character detected\n");
            error_occurred = 1;
            break;
        }
        
        switch (current_token.type) {
            case TOKEN_NUMBER:
                printf("Found number: %s\n", current_token.value);
                number_count++;
                break;
            case TOKEN_WORD:
                printf("Found word: %s\n", current_token.value);
                word_count++;
                break;
            case TOKEN_EOF:
                printf("\nParsing complete.\n");
                printf("Numbers found: %d\n", number_count);
                printf("Words found: %d\n", word_count);
                return 0;
            default:
                error_occurred = 1;
                break;
        }
    }
    
    if (error_occurred) {
        printf("Parsing failed due to invalid input.\n");
        return 1;
    }
    
    return 0;
}