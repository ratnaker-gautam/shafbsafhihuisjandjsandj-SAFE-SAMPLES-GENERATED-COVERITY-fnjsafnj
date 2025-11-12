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
    if (isalpha(c)) return EVENT_LETTER;
    if (isdigit(c)) return EVENT_DIGIT;
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=') return EVENT_OPERATOR;
    if (isspace(c)) return EVENT_WHITESPACE;
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
    char input[MAX_INPUT + 1];
    printf("Enter expression to analyze: ");
    
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
    char* ptr = input;
    char* token_start = ptr;
    
    printf("Token analysis:\n");
    
    while (current_state != STATE_END) {
        Event evt = classify_char(*ptr);
        State next_state = transition_table[current_state][evt];
        
        if (current_state != STATE_START && current_state != STATE_WHITESPACE && 
            current_state != STATE_ERROR && next_state != current_state) {
            
            size_t token_len = ptr - token_start;
            if (token_len > 0 && token_len < MAX_INPUT) {
                char token[MAX_INPUT];
                for (size_t i = 0; i < token_len; i++) {
                    token[i] = *(token_start + i);
                }
                token[token_len] = '\0';
                printf