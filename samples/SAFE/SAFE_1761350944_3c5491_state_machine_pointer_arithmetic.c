//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_IDENTIFIER,
    STATE_NUMBER,
    STATE_OPERATOR,
    STATE_END,
    STATE_ERROR
} State;

typedef enum {
    EVENT_LETTER,
    EVENT_DIGIT,
    EVENT_OPERATOR,
    EVENT_SPACE,
    EVENT_END,
    EVENT_INVALID
} Event;

State transition_table[MAX_STATES][6] = {
    {STATE_IDENTIFIER, STATE_NUMBER, STATE_OPERATOR, STATE_START, STATE_END, STATE_ERROR},
    {STATE_IDENTIFIER, STATE_IDENTIFIER, STATE_OPERATOR, STATE_START, STATE_END, STATE_ERROR},
    {STATE_ERROR, STATE_NUMBER, STATE_OPERATOR, STATE_START, STATE_END, STATE_ERROR},
    {STATE_IDENTIFIER, STATE_NUMBER, STATE_OPERATOR, STATE_START, STATE_END, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_END, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
};

Event get_event(char c) {
    if (c == '\0') return EVENT_END;
    if (isspace(c)) return EVENT_SPACE;
    if (isalpha(c)) return EVENT_LETTER;
    if (isdigit(c)) return EVENT_DIGIT;
    if (strchr("+-*/=", c) != NULL) return EVENT_OPERATOR;
    return EVENT_INVALID;
}

void process_token(char *start, char *end, State state) {
    if (end > start) {
        char token[MAX_INPUT_LEN];
        size_t len = end - start;
        if (len >= MAX_INPUT_LEN) len = MAX_INPUT_LEN - 1;
        strncpy(token, start, len);
        token[len] = '\0';
        
        switch (state) {
            case STATE_IDENTIFIER:
                printf("Identifier: %s\n", token);
                break;
            case STATE_NUMBER:
                printf("Number: %s\n", token);
                break;
            case STATE_OPERATOR:
                printf("Operator: %s\n", token);
                break;
            default:
                break;
        }
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter expression: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Invalid input length\n");
        return 1;
    }
    
    State current_state = STATE_START;
    char *current_pos = input;
    char *token_start = input;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        Event event = get_event(*current_pos);
        State next_state = transition_table[current_state][event];
        
        if (current_state != next_state) {
            process_token(token_start, current_pos, current_state);
            token_start = current_pos;
        }
        
        current_state = next_state;
        
        if (*current_pos != '\0') {
            current_pos++;
        } else {
            break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Invalid expression\n");
        return 1;
    }
    
    process_token(token_start, current_pos, current_state);
    
    return 0;
}