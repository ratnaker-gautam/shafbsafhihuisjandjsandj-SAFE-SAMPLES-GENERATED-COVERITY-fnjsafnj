//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_READING_SYMBOL,
    STATE_ERROR
} parser_state_t;

typedef enum {
    TOKEN_DIGIT,
    TOKEN_LETTER,
    TOKEN_SYMBOL,
    TOKEN_SPACE,
    TOKEN_INVALID
} token_type_t;

static token_type_t classify_char(char c) {
    if (isdigit(c)) return TOKEN_DIGIT;
    if (isalpha(c)) return TOKEN_LETTER;
    if (isspace(c)) return TOKEN_SPACE;
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=') return TOKEN_SYMBOL;
    return TOKEN_INVALID;
}

static parser_state_t transition_table[STATE_COUNT][5] = {
    {STATE_READING_NUMBER, STATE_READING_WORD, STATE_READING_SYMBOL, STATE_START, STATE_ERROR},
    {STATE_READING_NUMBER, STATE_ERROR, STATE_ERROR, STATE_START, STATE_ERROR},
    {STATE_ERROR, STATE_READING_WORD, STATE_ERROR, STATE_START, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_START, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
};

static void process_input(const char* input) {
    parser_state_t current_state = STATE_START;
    size_t input_len = strlen(input);
    
    if (input_len >= MAX_INPUT_LEN) {
        printf("Error: Input too long\n");
        return;
    }
    
    size_t token_start = 0;
    int token_count = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char current_char = (i < input_len) ? input[i] : ' ';
        token_type_t token = classify_char(current_char);
        
        parser_state_t next_state = transition_table[current_state][token];
        
        if (next_state == STATE_START && current_state != STATE_START) {
            size_t token_len = i - token_start;
            if (token_len > 0 && token_len < MAX_INPUT_LEN) {
                char token_str[MAX_INPUT_LEN];
                strncpy(token_str, input + token_start, token_len);
                token_str[token_len] = '\0';
                
                switch (current_state) {
                    case STATE_READING_NUMBER:
                        printf("Token %d: NUMBER '%s'\n", ++token_count, token_str);
                        break;
                    case STATE_READING_WORD:
                        printf("Token %d: WORD '%s'\n", ++token_count, token_str);
                        break;
                    case STATE_READING_SYMBOL:
                        printf("Token %d: SYMBOL '%s'\n", ++token_count, token_str);
                        break;
                    default:
                        break;
                }
            }
            token_start = i;
        }
        
        if (next_state == STATE_ERROR) {
            printf("Error: Invalid character '%c' at position %zu\n", current_char, i);
            return;
        }
        
        current_state = next_state;
        
        if (current_state != STATE_START && i == token_start) {
            token_start = i;
        }
    }
    
    printf("Total tokens processed: %d\n", token_count);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text to tokenize (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 1 && input[0] == '\n') {
        printf("No input provided\n");
        return 1;
    }
    
    process_input(input);
    
    return 0;
}