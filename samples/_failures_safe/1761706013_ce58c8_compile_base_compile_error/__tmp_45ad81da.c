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
    if (s >= 0 && s < STATE_COUNT) return names[s];
    return "UNKNOWN";
}

void process_input(const char* input) {
    State current_state = STATE_START;
    const char* ptr = input;
    char token[MAX_INPUT];
    char* token_ptr = token;
    
    if (input == NULL) {
        printf("Error: NULL input\n");
        return;
    }
    
    printf("Input: %s\n", input);
    printf("State transitions:\n");
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        Event event = classify_char(*ptr);
        State next_state = transition_table[current_state][event];
        
        printf("  %s -> %s (char: ", state_name(current_state), state_name(next_state));
        if (*ptr == '\0') printf("\\0");
        else if (*ptr == '\n') printf("\\n");
        else printf("%c", *ptr);
        printf(")\n");
        
        if (next_state == STATE_IDENTIFIER || next_state == STATE_NUMBER || 
            next_state == STATE_OPERATOR) {
            if ((size_t)(token_ptr - token) < MAX_INPUT - 1) {
                *token_ptr++ = *ptr;
            }
        } else if (current_state == STATE_IDENTIFIER || current_state == STATE_NUMBER || 
                  current_state == STATE_OPERATOR) {
            if (token_ptr > token) {
                *token_ptr = '\0';
                printf("    Token: %s