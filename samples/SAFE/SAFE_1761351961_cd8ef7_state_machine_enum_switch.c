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

int get_next_token(struct token *t, FILE *input) {
    static char buffer[32];
    static int pos = 0;
    int c;
    
    while (1) {
        c = fgetc(input);
        if (!is_valid_char(c) && c != EOF) {
            return -1;
        }
        
        if (c == EOF) {
            t->type = TOKEN_EOF;
            t->value[0] = '\0';
            return 0;
        }
        
        if (c == ' ' || c == '\t' || c == '\n') {
            if (pos > 0) {
                buffer[pos] = '\0';
                strncpy(t->value, buffer, sizeof(t->value) - 1);
                t->value[sizeof(t->value) - 1] = '\0';
                
                int all_digits = 1;
                for (int i = 0; i < pos; i++) {
                    if (!isdigit(buffer[i])) {
                        all_digits = 0;
                        break;
                    }
                }
                
                t->type = all_digits ? TOKEN_NUMBER : TOKEN_WORD;
                pos = 0;
                return 0;
            }
            continue;
        }
        
        if (pos < (int)sizeof(buffer) - 1) {
            buffer[pos++] = (char)c;
        } else {
            return -1;
        }
    }
}

int main(void) {
    FILE *input = stdin;
    enum state current_state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    struct token current_token;
    
    printf("Enter text (alphanumeric characters and spaces only):\n");
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        int result = get_next_token(&current_token, input);
        
        if (result != 0) {
            current_state = STATE_ERROR;
            break;
        }
        
        switch (current_state) {
            case STATE_START:
                if (current_token.type == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else if (current_token.type == TOKEN_NUMBER) {
                    number_count++;
                    current_state = STATE_READING_NUMBER;
                } else if (current_token.type == TOKEN_WORD) {
                    word_count++;
                    current_state = STATE_READING_WORD;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (current_token.type == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else if (current_token.type == TOKEN_NUMBER) {
                    number_count++;
                } else if (current_token.type == TOKEN_WORD) {
                    word_count++;
                    current_state = STATE_READING_WORD;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (current_token.type == TOKEN_EOF) {
                    current_state = STATE_DONE;
                } else if (current_token.type == TOKEN_NUMBER) {
                    number_count++;
                    current_state = STATE_READING_NUMBER;
                } else if (current_token.type == TOKEN_WORD) {
                    word_count++;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                break;
                
            case STATE_DONE:
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input detected\n");
        return 1;
    }
    
    printf("Analysis complete:\n");
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}