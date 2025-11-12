//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define MAX_STATES 8

typedef enum {
    STATE_START,
    STATE_IDENTIFIER,
    STATE_NUMBER,
    STATE_OPERATOR,
    STATE_WHITESPACE,
    STATE_ERROR,
    STATE_END,
    STATE_COUNT
} State;

typedef enum {
    CHAR_ALPHA,
    CHAR_DIGIT,
    CHAR_OPERATOR,
    CHAR_WHITESPACE,
    CHAR_TERMINATOR,
    CHAR_INVALID
} CharType;

CharType get_char_type(char c) {
    if (isalpha(c)) return CHAR_ALPHA;
    if (isdigit(c)) return CHAR_DIGIT;
    if (strchr("+-*/=", c)) return CHAR_OPERATOR;
    if (isspace(c)) return CHAR_WHITESPACE;
    if (c == '\0') return CHAR_TERMINATOR;
    return CHAR_INVALID;
}

State transition_table[STATE_COUNT][6] = {
    {STATE_IDENTIFIER, STATE_NUMBER, STATE_OPERATOR, STATE_WHITESPACE, STATE_END, STATE_ERROR},
    {STATE_IDENTIFIER, STATE_IDENTIFIER, STATE_OPERATOR, STATE_WHITESPACE, STATE_END, STATE_ERROR},
    {STATE_IDENTIFIER, STATE_NUMBER, STATE_OPERATOR, STATE_WHITESPACE, STATE_END, STATE_ERROR},
    {STATE_IDENTIFIER, STATE_NUMBER, STATE_OPERATOR, STATE_WHITESPACE, STATE_END, STATE_ERROR},
    {STATE_IDENTIFIER, STATE_NUMBER, STATE_OPERATOR, STATE_WHITESPACE, STATE_END, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}
};

void process_token(State state, char *start, char *end) {
    if (end > start) {
        printf("Token: ");
        for (char *p = start; p < end; p++) {
            putchar(*p);
        }
        printf(" (Type: ");
        switch (state) {
            case STATE_IDENTIFIER: printf("Identifier"); break;
            case STATE_NUMBER: printf("Number"); break;
            case STATE_OPERATOR: printf("Operator"); break;
            case STATE_WHITESPACE: printf("Whitespace"); break;
            default: printf("Unknown"); break;
        }
        printf(")\n");
    }
}

int main(void) {
    char input[MAX_INPUT + 1];
    
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    State current_state = STATE_START;
    State previous_state = STATE_START;
    char *token_start = input;
    char *current_ptr = input;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        CharType ctype = get_char_type(*current_ptr);
        if (ctype == CHAR_INVALID) {
            current_state = STATE_ERROR;
            break;
        }
        
        previous_state = current_state;
        current_state = transition_table[current_state][ctype];
        
        if (current_state != previous_state) {
            process_token(previous_state, token_start, current_ptr);
            token_start = current_ptr;
        }
        
        if (current_state == STATE_END) {
            break;
        }
        
        current_ptr++;
        if (current_ptr - input >= (long)len) {
            break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Invalid character at position %ld\n", current_ptr - input);
        return 1;
    }
    
    process_token(current_state, token_start, current_ptr);
    
    return 0;
}