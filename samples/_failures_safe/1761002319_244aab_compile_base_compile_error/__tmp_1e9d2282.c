//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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
    EVENT_LETTER,
    EVENT_DIGIT,
    EVENT_OPERATOR,
    EVENT_WHITESPACE,
    EVENT_TERMINATOR,
    EVENT_INVALID,
    EVENT_COUNT
} Event;

State transition_table[STATE_COUNT][EVENT_COUNT] = {
    [STATE_START] = {
        [EVENT_LETTER] = STATE_IDENTIFIER,
        [EVENT_DIGIT] = STATE_NUMBER,
        [EVENT_OPERATOR] = STATE_OPERATOR,
        [EVENT_WHITESPACE] = STATE_WHITESPACE,
        [EVENT_TERMINATOR] = STATE_END,
        [EVENT_INVALID] = STATE_ERROR
    },
    [STATE_IDENTIFIER] = {
        [EVENT_LETTER] = STATE_IDENTIFIER,
        [EVENT_DIGIT] = STATE_IDENTIFIER,
        [EVENT_OPERATOR] = STATE_OPERATOR,
        [EVENT_WHITESPACE] = STATE_WHITESPACE,
        [EVENT_TERMINATOR] = STATE_END,
        [EVENT_INVALID] = STATE_ERROR
    },
    [STATE_NUMBER] = {
        [EVENT_LETTER] = STATE_ERROR,
        [EVENT_DIGIT] = STATE_NUMBER,
        [EVENT_OPERATOR] = STATE_OPERATOR,
        [EVENT_WHITESPACE] = STATE_WHITESPACE,
        [EVENT_TERMINATOR] = STATE_END,
        [EVENT_INVALID] = STATE_ERROR
    },
    [STATE_OPERATOR] = {
        [EVENT_LETTER] = STATE_IDENTIFIER,
        [EVENT_DIGIT] = STATE_NUMBER,
        [EVENT_OPERATOR] = STATE_OPERATOR,
        [EVENT_WHITESPACE] = STATE_WHITESPACE,
        [EVENT_TERMINATOR] = STATE_END,
        [EVENT_INVALID] = STATE_ERROR
    },
    [STATE_WHITESPACE] = {
        [EVENT_LETTER] = STATE_IDENTIFIER,
        [EVENT_DIGIT] = STATE_NUMBER,
        [EVENT_OPERATOR] = STATE_OPERATOR,
        [EVENT_WHITESPACE] = STATE_WHITESPACE,
        [EVENT_TERMINATOR] = STATE_END,
        [EVENT_INVALID] = STATE_ERROR
    },
    [STATE_ERROR] = {
        [EVENT_LETTER] = STATE_ERROR,
        [EVENT_DIGIT] = STATE_ERROR,
        [EVENT_OPERATOR] = STATE_ERROR,
        [EVENT_WHITESPACE] = STATE_ERROR,
        [EVENT_TERMINATOR] = STATE_END,
        [EVENT_INVALID] = STATE_ERROR
    }
};

Event classify_char(char c) {
    if (c == '\0') return EVENT_TERMINATOR;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=') return EVENT_OPERATOR;
    if (isspace((unsigned char)c)) return EVENT_WHITESPACE;
    return EVENT_INVALID;
}

const char* state_name(State s) {
    static const char* names[] = {
        "START", "IDENTIFIER", "NUMBER", "OPERATOR", 
        "WHITESPACE", "ERROR", "END"
    };
    if (s >= 0 && s < STATE_COUNT) {
        return names[s];
    }
    return "UNKNOWN";
}

int main(void) {
    char input[MAX_INPUT];
    char *ptr;
    
    printf("Enter expression to tokenize: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    State current_state = STATE_START;
    State previous_state = STATE_START;
    char *token_start = input;
    char *current_pos = input;
    
    printf("Tokenizing: '%s'\n", input);
    printf("Tokens found:\n");
    
    while (current_state != STATE_END) {
        Event event = classify_char(*current_pos);
        previous_state = current_state;
        current_state = transition_table[current_state][event];
        
        if (previous_state != current_state && previous_state != STATE_START && previous_state != STATE_WHITESPACE) {
            if (previous_state != STATE_ERROR) {
                size_t token_len = current_pos - token_start;
                if (token_len > 0 && token_len < MAX_INPUT) {
                    char token[MAX_IN