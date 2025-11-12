//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    
    for (size_t i = 0; i < input_len; i++) {
        token_type_t token = classify_char(input[i]);
        
        if (token == TOKEN_INVALID) {
            current_state = STATE_ERROR;
            break;
        }
        
        current_state = transition_table[current_state][token];
        
        if (current_state == STATE_ERROR) {
            break;
        }
        
        if (current_state == STATE_START && i < input_len - 1) {
            printf("Token complete\n");
        }
    }
    
    switch (current_state) {
        case STATE_START:
            printf("Parsing completed successfully\n");
            break;
        case STATE_READING_NUMBER:
            printf("Final token: number\n");
            break;
        case STATE_READING_WORD:
            printf("Final token: word\n");
            break;
        case STATE_READING_SYMBOL:
            printf("Final token: symbol\n");
            break;
        case STATE_ERROR:
            printf("Parsing error at position\n");
            break;
    }
}

static int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i])) return 0;
    }
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    
    printf("Enter text to parse (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!validate_input(input_buffer)) {
        printf("Invalid input\n");
        return 1;
    }
    
    process_input(input_buffer);
    
    return 0;
}