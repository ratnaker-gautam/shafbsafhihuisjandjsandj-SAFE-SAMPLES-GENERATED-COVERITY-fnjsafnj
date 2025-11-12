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

static State transition_table[STATE_COUNT][EVENT_COUNT] = {
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

static const char* state_names[STATE_COUNT] = {
    "START", "IDENTIFIER", "NUMBER", "OPERATOR", "WHITESPACE", "ERROR", "END"
};

Event classify_char(char c) {
    if (c == '\0') return EVENT_TERMINATOR;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=') return EVENT_OPERATOR;
    if (isspace((unsigned char)c)) return EVENT_WHITESPACE;
    return EVENT_INVALID;
}

void process_input(const char* input) {
    if (input == NULL) return;
    
    State current_state = STATE_START;
    const char* start_ptr = input;
    const char* current_ptr = input;
    
    while (current_state != STATE_END) {
        Event event = classify_char(*current_ptr);
        State next_state = transition_table[current_state][event];
        
        if (current_state != next_state && current_state != STATE_START) {
            size_t token_len = current_ptr - start_ptr;
            if (token_len > 0 && token_len <= MAX_INPUT) {
                printf("Token: %.*s -> State: %s\n", (int)token_len, start_ptr, state_names[current_state]);
            }
            start_ptr = current_ptr;
        }
        
        current_state = next_state;
        
        if (current_state == STATE_ERROR) {
            printf("Error: Invalid character '%c' at position %ld\n", *current_ptr, current_ptr - input);
            break;
        }
        
        if (*current_ptr != '\0') {
            current_ptr++;
        }
        
        if (current_ptr - input >= MAX_INPUT) {
            printf("Error: Input too long\n");
            break;
        }
    }
}

int main(void) {
    char input[MAX_INPUT + 1] = {0};
    
    printf("Enter expression to tokenize: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);