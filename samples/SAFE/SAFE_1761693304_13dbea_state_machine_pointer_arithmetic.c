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
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
};

void process_input(const char *input) {
    if (input == NULL) return;
    
    State current_state = STATE_START;
    const char *start = input;
    const char *current = input;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        CharType ct = get_char_type(*current);
        State next_state = transition_table[current_state][ct];
        
        if (current_state != next_state) {
            size_t length = current - start;
            if (length > 0 && length < MAX_INPUT) {
                char token[MAX_INPUT];
                if (length < sizeof(token)) {
                    strncpy(token, start, length);
                    token[length] = '\0';
                    
                    switch (current_state) {
                        case STATE_IDENTIFIER:
                            printf("Identifier: %s\n", token);
                            break;
                        case STATE_NUMBER:
                            printf("Number: %s\n", token);
                            break;
                        case STATE_OPERATOR:
                            printf("Operator: %s\n", token);
                            break;
                        case STATE_WHITESPACE:
                            break;
                        default:
                            break;
                    }
                }
            }
            start = current;
        }
        
        current_state = next_state;
        
        if (current_state == STATE_END || *current == '\0') {
            break;
        }
        
        current++;
        if (current - input >= MAX_INPUT) {
            break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input sequence\n");
    }
}

int main() {
    char input[MAX_INPUT];
    
    printf("Enter expression (max %d chars): ", MAX_INPUT - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\0')) {
        printf("No input provided\n");
        return 1;
    }
    
    process_input(input);
    return 0;
}