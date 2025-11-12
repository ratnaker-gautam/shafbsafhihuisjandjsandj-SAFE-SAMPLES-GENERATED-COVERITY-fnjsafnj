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

int is_valid_char(char c) {
    return isalnum(c) || c == ' ' || c == '\n' || c == '\t' || c == '\0';
}

int get_next_token(struct token *t, const char *input, int *pos) {
    int len = strlen(input);
    if (*pos >= len) {
        t->type = TOKEN_END;
        return 1;
    }
    
    int start = *pos;
    while (*pos < len && isspace(input[*pos])) {
        (*pos)++;
    }
    
    if (*pos >= len) {
        t->type = TOKEN_END;
        return 1;
    }
    
    start = *pos;
    if (isalpha(input[*pos])) {
        int i = 0;
        while (*pos < len && isalpha(input[*pos]) && i < 31) {
            t->value[i++] = input[*pos];
            (*pos)++;
        }
        t->value[i] = '\0';
        t->type = TOKEN_WORD;
    } else if (isdigit(input[*pos])) {
        int i = 0;
        while (*pos < len && isdigit(input[*pos]) && i < 31) {
            t->value[i++] = input[*pos];
            (*pos)++;
        }
        t->value[i] = '\0';
        t->type = TOKEN_NUMBER;
    } else {
        return 0;
    }
    
    return 1;
}

int main(void) {
    char input[1024];
    printf("Enter text (max 1023 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    int input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    for (int i = 0; i < input_len; i++) {
        if (!is_valid_char(input[i])) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    enum state current_state = STATE_START;
    int pos = 0;
    int word_count = 0;
    int number_count = 0;
    
    while (current_state != STATE_DONE && current_state != STATE_ERROR) {
        struct token t;
        memset(&t, 0, sizeof(t));
        
        switch (current_state) {
            case STATE_START:
                if (!get_next_token(&t, input, &pos)) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                if (t.type == TOKEN_WORD) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                    printf("Found word: %s\n", t.value);
                } else if (t.type == TOKEN_NUMBER) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                    printf("Found number: %s\n", t.value);
                } else if (t.type == TOKEN_END) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_WORD:
                if (!get_next_token(&t, input, &pos)) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                if (t.type == TOKEN_WORD) {
                    word_count++;
                    printf("Found word: %s\n", t.value);
                } else if (t.type == TOKEN_NUMBER) {
                    current_state = STATE_READING_NUMBER;
                    number_count++;
                    printf("Found number: %s\n", t.value);
                } else if (t.type == TOKEN_END) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_READING_NUMBER:
                if (!get_next_token(&t, input, &pos)) {
                    current_state = STATE_ERROR;
                    break;
                }
                
                if (t.type == TOKEN_WORD) {
                    current_state = STATE_READING_WORD;
                    word_count++;
                    printf("Found word: %s\n", t.value);
                } else if (t.type == TOKEN_NUMBER) {
                    number_count++;
                    printf("Found number: %s\n", t.value);
                } else if (t.type == TOKEN_END) {
                    current_state = STATE_DONE;
                } else {
                    current_state = STATE_ERROR;
                }
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Error processing input\n");
                break;
                
            case STATE_DONE:
                break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Processing failed\n");
        return 1;
    }
    
    printf("Summary: %d words, %d numbers\n", word_count, number_count);
    return 0;
}