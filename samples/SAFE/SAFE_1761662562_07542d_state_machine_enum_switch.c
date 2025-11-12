//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
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

int get_next_char(void) {
    int ch = getchar();
    if (ch == EOF) return EOF;
    return ch;
}

void unget_char(int ch) {
    if (ch != EOF) ungetc(ch, stdin);
}

int is_valid_char(int ch) {
    return ch == EOF || isspace(ch) || isalnum(ch);
}

int read_token(struct token *tok) {
    int ch;
    int pos = 0;
    
    memset(tok->value, 0, sizeof(tok->value));
    
    while ((ch = get_next_char()) != EOF && isspace(ch));
    
    if (ch == EOF) {
        tok->type = TOKEN_EOF;
        return 1;
    }
    
    if (!is_valid_char(ch)) {
        return 0;
    }
    
    if (isdigit(ch)) {
        tok->type = TOKEN_NUMBER;
        while (isdigit(ch) && pos < 31) {
            tok->value[pos++] = ch;
            ch = get_next_char();
        }
        if (pos > 0 && ch != EOF) unget_char(ch);
        tok->value[pos] = '\0';
        return 1;
    }
    
    if (isalpha(ch)) {
        tok->type = TOKEN_WORD;
        while (isalpha(ch) && pos < 31) {
            tok->value[pos++] = ch;
            ch = get_next_char();
        }
        if (pos > 0 && ch != EOF) unget_char(ch);
        tok->value[pos] = '\0';
        return 1;
    }
    
    return 0;
}

int main(void) {
    enum state current_state = STATE_START;
    struct token current_token;
    int number_count = 0;
    int word_count = 0;
    
    printf("Enter text (alphanumeric only, Ctrl+D to end):\n");
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        switch (current_state) {
            case STATE_START:
                if (read_token(&current_token)) {
                    if (current_token.type == TOKEN_EOF) {
                        current_state = STATE_END;
                    } else if (current_token.type == TOKEN_NUMBER) {
                        current_state = STATE_READING_NUMBER;
                    } else if (current_token.type == TOKEN_WORD) {
                        current_state = STATE_READING_WORD;
                    } else {
                        current_state = STATE_ERROR;
                    }
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                number_count++;
                printf("Found number: %s\n", current_token.value);
                current_state = STATE_START;
                break;
                
            case STATE_READING_WORD:
                word_count++;
                printf("Found word: %s\n", current_token.value);
                current_state = STATE_START;
                break;
                
            case STATE_ERROR:
                printf("Error: Invalid input character\n");
                current_state = STATE_END;
                break;
                
            case STATE_END:
                break;
        }
    }
    
    printf("\nSummary: %d numbers, %d words\n", number_count, word_count);
    
    return current_state == STATE_ERROR ? 1 : 0;
}