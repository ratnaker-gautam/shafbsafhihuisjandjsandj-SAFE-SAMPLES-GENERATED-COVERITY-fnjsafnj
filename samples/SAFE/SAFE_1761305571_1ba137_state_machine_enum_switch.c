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
    TOKEN_END
};

struct token {
    enum token_type type;
    char value[32];
};

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '\t' || c == '\n' || c == '\0';
}

int read_token(struct token *t, const char *input, int *pos) {
    int len = strlen(input);
    if (*pos >= len) {
        t->type = TOKEN_END;
        t->value[0] = '\0';
        return 1;
    }
    
    while (*pos < len && (input[*pos] == ' ' || input[*pos] == '\t')) {
        (*pos)++;
    }
    
    if (*pos >= len) {
        t->type = TOKEN_END;
        t->value[0] = '\0';
        return 1;
    }
    
    int start = *pos;
    if (isdigit(input[*pos])) {
        t->type = TOKEN_NUMBER;
        int i = 0;
        while (*pos < len && isdigit(input[*pos]) && i < 31) {
            t->value[i++] = input[(*pos)++];
        }
        t->value[i] = '\0';
    } else if (isalpha(input[*pos])) {
        t->type = TOKEN_WORD;
        int i = 0;
        while (*pos < len && isalpha(input[*pos]) && i < 31) {
            t->value[i++] = input[(*pos)++];
        }
        t->value[i] = '\0';
    } else {
        return 0;
    }
    
    return 1;
}

int main(void) {
    char input[256];
    printf("Enter text (max 255 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    for (int i = 0; input[i] != '\0'; i++) {
        if (!is_valid_char(input[i])) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    enum state current_state = STATE_START;
    int pos = 0;
    int number_count = 0;
    int word_count = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        struct token t;
        
        switch (current_state) {
            case STATE_START:
                if (!read_token(&t, input, &pos)) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                switch (t.type) {
                    case TOKEN_NUMBER:
                        current_state = STATE_READING_NUMBER;
                        number_count++;
                        printf("Found number: %s\n", t.value);
                        break;
                    case TOKEN_WORD:
                        current_state = STATE_READING_WORD;
                        word_count++;
                        printf("Found word: %s\n", t.value);
                        break;
                    case TOKEN_END:
                        current_state = STATE_END;
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (!read_token(&t, input, &pos)) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                switch (t.type) {
                    case TOKEN_NUMBER:
                        number_count++;
                        printf("Found number: %s\n", t.value);
                        break;
                    case TOKEN_WORD:
                        current_state = STATE_READING_WORD;
                        word_count++;
                        printf("Found word: %s\n", t.value);
                        break;
                    case TOKEN_END:
                        current_state = STATE_END;
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_READING_WORD:
                if (!read_token(&t, input, &pos)) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                switch (t.type) {
                    case TOKEN_NUMBER:
                        current_state = STATE_READING_NUMBER;
                        number_count++;
                        printf("Found number: %s\n", t.value);
                        break;
                    case TOKEN_WORD:
                        word_count++;
                        printf("Found word: %s\n", t.value);
                        break;
                    case TOKEN_END:
                        current_state = STATE_END;
                        break;
                    default:
                        current_state = STATE_ERROR;
                        break;
                }
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Error processing input\n");
                break;
                
            case STATE_END:
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Failed to process input\n");
        return 1;
    }
    
    printf("Summary: %d numbers, %d words\n", number_count, word_count);
    return 0;
}