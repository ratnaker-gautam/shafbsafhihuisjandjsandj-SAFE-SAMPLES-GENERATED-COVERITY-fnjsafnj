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

int get_next_char(FILE *stream) {
    int c = fgetc(stream);
    if (c == EOF) return EOF;
    if (!is_valid_char(c)) return -1;
    return c;
}

int parse_token(FILE *stream, struct token *token) {
    int c;
    int pos = 0;
    enum state current_state = STATE_START;
    
    memset(token->value, 0, sizeof(token->value));
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        c = get_next_char(stream);
        
        switch (current_state) {
            case STATE_START:
                if (c == EOF) {
                    token->type = TOKEN_EOF;
                    current_state = STATE_DONE;
                } else if (isdigit(c)) {
                    if (pos < 31) token->value[pos++] = c;
                    current_state = STATE_READING_NUMBER;
                } else if (isalpha(c)) {
                    if (pos < 31) token->value[pos++] = tolower(c);
                    current_state = STATE_READING_WORD;
                } else if (c == ' ' || c == '\t' || c == '\n') {
                    continue;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (isdigit(c)) {
                    if (pos < 31) token->value[pos++] = c;
                } else if (c == ' ' || c == '\t' || c == '\n' || c == EOF) {
                    token->type = TOKEN_NUMBER;
                    current_state = STATE_DONE;
                    if (c != EOF) ungetc(c, stream);
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (isalpha(c)) {
                    if (pos < 31) token->value[pos++] = tolower(c);
                } else if (c == ' ' || c == '\t' || c == '\n' || c == EOF) {
                    token->type = TOKEN_WORD;
                    current_state = STATE_DONE;
                    if (c != EOF) ungetc(c, stream);
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
    
    token->value[31] = '\0';
    return 0;
}

int main(void) {
    FILE *input = stdin;
    struct token token;
    int number_count = 0;
    int word_count = 0;
    int error_count = 0;
    
    printf("Enter text (alphanumeric characters only): ");
    
    while (1) {
        int result = parse_token(input, &token);
        
        if (result == -1) {
            error_count++;
            printf("Error: Invalid character sequence\n");
            break;
        }
        
        switch (token.type) {
            case TOKEN_NUMBER:
                number_count++;
                printf("Number: %s\n", token.value);
                break;
            case TOKEN_WORD:
                word_count++;
                printf("Word: %s\n", token.value);
                break;
            case TOKEN_EOF:
                printf("\nSummary: %d numbers, %d words, %d errors\n", 
                       number_count, word_count, error_count);
                return 0;
        }
        
        if (number_count + word_count > 1000) {
            printf("Maximum token limit reached\n");
            break;
        }
    }
    
    return 1;
}