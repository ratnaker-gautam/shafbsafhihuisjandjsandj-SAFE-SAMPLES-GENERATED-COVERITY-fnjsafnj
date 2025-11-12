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
        printf("Input too long\n");
        return;
    }
    
    size_t token_start = 0;
    int token_count = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char current_char = (i < input_len) ? input[i] : ' ';
        token_type_t token = classify_char(current_char);
        
        parser_state_t next_state = transition_table[current_state][token];
        
        if (current_state != next_state) {
            if (current_state != STATE_START && current_state != STATE_ERROR) {
                size_t token_length = i - token_start;
                if (token_length > 0 && token_length < MAX_INPUT_LEN) {
                    char token_str[MAX_INPUT_LEN];
                    strncpy(token_str, input + token_start, token_length);
                    token_str[token_length] = '\0';
                    
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
            }
            
            if (next_state != STATE_START && next_state != STATE_ERROR) {
                token_start = i;
            }
        }
        
        current_state = next_state;
        
        if (current_state == STATE_ERROR) {
            printf("Invalid input at position %zu\n", i);
            return;
        }
    }
    
    printf("Total tokens processed: %d\n", token_count);
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 2];
    
    printf("Enter text to tokenize: ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (len == 1 && input_buffer[0] == '\n') {
        printf("Empty input\n");
        return 1;
    }
    
    if (len >= MAX_INPUT_LEN) {
        printf("Input too long\n");
        return 1;
    }
    
    process_input(input_buffer);
    
    return 0;
}