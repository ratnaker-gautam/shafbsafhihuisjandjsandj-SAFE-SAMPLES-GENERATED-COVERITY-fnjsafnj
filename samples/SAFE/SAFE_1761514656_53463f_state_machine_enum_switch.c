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
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n' || c == EOF;
}

int read_token(struct token *t, FILE *input) {
    int c;
    int pos = 0;
    
    while ((c = fgetc(input)) != EOF && (c == ' ' || c == '\t' || c == '\n')) {
        continue;
    }
    
    if (c == EOF) {
        t->type = TOKEN_END;
        return 1;
    }
    
    if (!is_valid_input_char(c)) {
        return 0;
    }
    
    if (isalpha(c)) {
        t->type = TOKEN_WORD;
        while (isalpha(c) && pos < 31) {
            t->value[pos++] = (char)c;
            c = fgetc(input);
        }
        if (c != EOF && !is_valid_input_char(c)) {
            return 0;
        }
        if (c != EOF) {
            ungetc(c, input);
        }
    } else if (isdigit(c)) {
        t->type = TOKEN_NUMBER;
        while (isdigit(c) && pos < 31) {
            t->value[pos++] = (char)c;
            c = fgetc(input);
        }
        if (c != EOF && !is_valid_input_char(c)) {
            return 0;
        }
        if (c != EOF) {
            ungetc(c, input);
        }
    } else {
        return 0;
    }
    
    t->value[pos] = '\0';
    return 1;
}

int main(void) {
    FILE *input = stdin;
    enum state current_state = STATE_START;
    struct token current_token;
    int word_count = 0;
    int number_count = 0;
    
    printf("Enter text (words and numbers): ");
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_START:
                if (!read_token(&current_token, input)) {
                    current_state = STATE_ERROR;
                } else if (current_token.type == TOKEN_END) {
                    current_state = STATE_DONE;
                } else if (current_token.type == TOKEN_WORD) {
                    current_state = STATE_READING_WORD;
                } else if (current_token.type == TOKEN_NUMBER) {
                    current_state = STATE_READING_NUMBER;
                }
                break;
                
            case STATE_READING_WORD:
                word_count++;
                printf("Found word: %s\n", current_token.value);
                if (!read_token(&current_token, input)) {
                    current_state = STATE_ERROR;
                } else if (current_token.type == TOKEN_END) {
                    current_state = STATE_DONE;
                } else if (current_token.type == TOKEN_WORD) {
                    current_state = STATE_READING_WORD;
                } else if (current_token.type == TOKEN_NUMBER) {
                    current_state = STATE_READING_NUMBER;
                }
                break;
                
            case STATE_READING_NUMBER:
                number_count++;
                printf("Found number: %s\n", current_token.value);
                if (!read_token(&current_token, input)) {
                    current_state = STATE_ERROR;
                } else if (current_token.type == TOKEN_END) {
                    current_state = STATE_DONE;
                } else if (current_token.type == TOKEN_WORD) {
                    current_state = STATE_READING_WORD;
                } else if (current_token.type == TOKEN_NUMBER) {
                    current_state = STATE_READING_NUMBER;
                }
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Error: Invalid input character\n");
                return EXIT_FAILURE;
                
            case STATE_DONE:
                break;
        }
    }
    
    if (current_state == STATE_DONE) {
        printf("Processing complete.\n");
        printf("Words: %d, Numbers: %d\n", word_count, number_count);
    }
    
    return EXIT_SUCCESS;
}