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

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '\n' || c == '\t' || c == EOF;
}

int get_next_char(void) {
    int c = getchar();
    if (c != EOF && !is_valid_char(c)) {
        return -1;
    }
    return c;
}

int parse_token(struct token *t) {
    static int current_char = ' ';
    static int position = 0;
    int index = 0;
    enum state current_state = STATE_START;
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        if (current_char == ' ' || current_char == '\t' || current_char == '\n') {
            if (current_state == STATE_START) {
                current_char = get_next_char();
                continue;
            } else if (current_state == STATE_READING_WORD || current_state == STATE_READING_NUMBER) {
                current_state = STATE_DONE;
                continue;
            }
        }
        
        if (current_char == EOF) {
            if (current_state == STATE_START) {
                t->type = TOKEN_EOF;
                return 1;
            } else {
                current_state = STATE_DONE;
                continue;
            }
        }
        
        switch (current_state) {
            case STATE_START:
                if (isalpha(current_char)) {
                    current_state = STATE_READING_WORD;
                    t->type = TOKEN_WORD;
                } else if (isdigit(current_char)) {
                    current_state = STATE_READING_NUMBER;
                    t->type = TOKEN_NUMBER;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(current_char)) {
                    if (index < 31) {
                        t->value[index++] = current_char;
                    } else {
                        current_state = STATE_ERROR;
                    }
                    current_char = get_next_char();
                } else {
                    current_state = STATE_DONE;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(current_char)) {
                    if (index < 31) {
                        t->value[index++] = current_char;
                    } else {
                        current_state = STATE_ERROR;
                    }
                    current_char = get_next_char();
                } else {
                    current_state = STATE_DONE;
                }
                break;
                
            case STATE_ERROR:
                return -1;
                
            default:
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        return -1;
    }
    
    if (index > 0) {
        t->value[index] = '\0';
        return 1;
    } else {
        t->type = TOKEN_EOF;
        return 1;
    }
}

int main(void) {
    struct token t;
    int result;
    int word_count = 0;
    int number_count = 0;
    
    printf("Enter text (words and numbers, EOF to end):\n");
    
    while (1) {
        result = parse_token(&t);
        
        if (result == -1) {
            printf("Error: Invalid input character\n");
            return 1;
        }
        
        if (t.type == TOKEN_EOF) {
            break;
        }
        
        switch (t.type) {
            case TOKEN_WORD:
                printf("Word: %s\n", t.value);
                word_count++;
                break;
            case TOKEN_NUMBER:
                printf("Number: %s\n", t.value);
                number_count++;
                break;
            default:
                break;
        }
    }
    
    printf("\nSummary: %d words, %d numbers\n", word_count, number_count);
    return 0;
}