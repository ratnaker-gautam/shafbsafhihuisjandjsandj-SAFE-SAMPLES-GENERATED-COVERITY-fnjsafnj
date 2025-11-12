//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

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
    TOKEN_EOF,
    TOKEN_INVALID
};

struct token {
    enum token_type type;
    char value[32];
};

int get_next_token(struct token *tok, const char *input, int *pos) {
    int start = *pos;
    int len = strlen(input);
    
    if (start >= len) {
        tok->type = TOKEN_EOF;
        tok->value[0] = '\0';
        return 1;
    }
    
    while (start < len && isspace(input[start])) {
        start++;
    }
    
    if (start >= len) {
        tok->type = TOKEN_EOF;
        tok->value[0] = '\0';
        *pos = start;
        return 1;
    }
    
    *pos = start;
    
    if (isdigit(input[start])) {
        int i = 0;
        while (start < len && isdigit(input[start]) && i < 31) {
            tok->value[i++] = input[start++];
        }
        tok->value[i] = '\0';
        tok->type = TOKEN_NUMBER;
        *pos = start;
        return 1;
    }
    
    if (isalpha(input[start])) {
        int i = 0;
        while (start < len && isalnum(input[start]) && i < 31) {
            tok->value[i++] = input[start++];
        }
        tok->value[i] = '\0';
        tok->type = TOKEN_WORD;
        *pos = start;
        return 1;
    }
    
    tok->type = TOKEN_INVALID;
    tok->value[0] = input[start];
    tok->value[1] = '\0';
    *pos = start + 1;
    return 1;
}

int main(void) {
    char input[256];
    printf("Enter text to analyze: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    enum state current_state = STATE_START;
    int position = 0;
    int number_count = 0;
    int word_count = 0;
    
    printf("Analysis results:\n");
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        struct token tok;
        
        if (!get_next_token(&tok, input, &position)) {
            current_state = STATE_ERROR;
            break;
        }
        
        switch (current_state) {
            case STATE_START:
                switch (tok.type) {
                    case TOKEN_NUMBER:
                        printf("Found number: %s\n", tok.value);
                        number_count++;
                        current_state = STATE_READING_NUMBER;
                        break;
                    case TOKEN_WORD:
                        printf("Found word: %s\n", tok.value);
                        word_count++;
                        current_state = STATE_READING_WORD;
                        break;
                    case TOKEN_EOF:
                        current_state = STATE_DONE;
                        break;
                    case TOKEN_INVALID:
                        printf("Invalid character: %s\n", tok.value);
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_NUMBER:
                switch (tok.type) {
                    case TOKEN_NUMBER:
                        printf("Found number: %s\n", tok.value);
                        number_count++;
                        break;
                    case TOKEN_WORD:
                        printf("Found word: %s\n", tok.value);
                        word_count++;
                        current_state = STATE_READING_WORD;
                        break;
                    case TOKEN_EOF:
                        current_state = STATE_DONE;
                        break;
                    case TOKEN_INVALID:
                        printf("Invalid character: %s\n", tok.value);
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_WORD:
                switch (tok.type) {
                    case TOKEN_NUMBER:
                        printf("Found number: %s\n", tok.value);
                        number_count++;
                        current_state = STATE_READING_NUMBER;
                        break;
                    case TOKEN_WORD:
                        printf("Found word: %s\n", tok.value);
                        word_count++;
                        break;
                    case TOKEN_EOF:
                        current_state = STATE_DONE;
                        break;
                    case TOKEN_INVALID:
                        printf("Invalid character: %s\n", tok.value);
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_ERROR:
                break;
                
            case STATE_DONE:
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Analysis terminated due to error\n");
    } else {
        printf("Summary: %d numbers, %d words\n", number_count, word_count);
    }
    
    return 0;
}