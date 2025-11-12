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
    CHAR_LETTER,
    CHAR_DIGIT,
    CHAR_OPERATOR,
    CHAR_WHITESPACE,
    CHAR_TERMINATOR,
    CHAR_INVALID
} CharType;

CharType get_char_type(char c) {
    if (isalpha(c)) return CHAR_LETTER;
    if (isdigit(c)) return CHAR_DIGIT;
    if (strchr("+-*/=", c) != NULL) return CHAR_OPERATOR;
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
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
};

const char* state_names[STATE_COUNT] = {
    "START", "IDENTIFIER", "NUMBER", "OPERATOR", "WHITESPACE", "ERROR", "END", "INVALID"
};

void process_input(const char* input) {
    if (input == NULL) {
        printf("Error: Null input\n");
        return;
    }
    
    size_t len = strlen(input);
    if (len >= MAX_INPUT) {
        printf("Error: Input too long\n");
        return;
    }
    
    State current_state = STATE_START;
    const char* start_ptr = input;
    const char* current_ptr = input;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        CharType ct = get_char_type(*current_ptr);
        State next_state = *(*(transition_table + current_state) + ct);
        
        if (current_state != next_state && current_state != STATE_START) {
            size_t token_len = current_ptr - start_ptr;
            if (token_len > 0 && token_len < MAX_INPUT) {
                char token[MAX_INPUT];
                strncpy(token, start_ptr, token_len);
                *(token + token_len) = '\0';
                printf("Token: '%.*s' -> %s\n", (int)token_len, start_ptr, 
                       *(state_names + current_state));
            }
            start_ptr = current_ptr;
        }
        
        current_state = next_state;
        
        if (current_state == STATE_END || current_state == STATE_ERROR) {
            break;
        }
        
        current_ptr++;
        if (current_ptr - input >= (long)len) {
            break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input at position %ld\n", current_ptr - input);
    }
}

int main() {
    char input[MAX_INPUT];
    
    printf("Enter input to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && *(input + len - 1) == '\n') {
        *(input + len - 1) = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Error: Empty input\n");
        return 1;
    }
    
    process_input(input);
    return 0;
}