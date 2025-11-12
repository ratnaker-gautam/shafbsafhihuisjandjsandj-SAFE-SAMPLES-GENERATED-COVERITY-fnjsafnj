//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_PUNCTUATION,
    STATE_END
} State;

typedef enum {
    EVENT_SPACE,
    EVENT_DIGIT,
    EVENT_ALPHA,
    EVENT_PUNCT,
    EVENT_END,
    EVENT_INVALID
} Event;

Event get_event(char c) {
    if (c == '\0') return EVENT_END;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_ALPHA;
    if (ispunct(c)) return EVENT_PUNCT;
    return EVENT_INVALID;
}

State transition_table[STATE_COUNT][6] = {
    {STATE_START, STATE_READING_WORD, STATE_READING_NUMBER, STATE_PUNCTUATION, STATE_END, STATE_START},
    {STATE_START, STATE_READING_WORD, STATE_READING_WORD, STATE_PUNCTUATION, STATE_END, STATE_START},
    {STATE_START, STATE_READING_NUMBER, STATE_READING_NUMBER, STATE_PUNCTUATION, STATE_END, STATE_START},
    {STATE_START, STATE_PUNCTUATION, STATE_PUNCTUATION, STATE_PUNCTUATION, STATE_END, STATE_START},
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}
};

void process_token(char *start, char *end, State state) {
    if (start >= end) return;
    
    size_t length = end - start;
    if (length >= MAX_INPUT) return;
    
    char token[MAX_INPUT];
    strncpy(token, start, length);
    token[length] = '\0';
    
    switch (state) {
        case STATE_READING_WORD:
            printf("WORD: %s\n", token);
            break;
        case STATE_READING_NUMBER:
            printf("NUMBER: %s\n", token);
            break;
        case STATE_PUNCTUATION:
            printf("PUNCTUATION: %s\n", token);
            break;
        default:
            break;
    }
}

int main(void) {
    char input[MAX_INPUT];
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    State current_state = STATE_START;
    char *current_pos = input;
    char *token_start = input;
    
    while (current_state != STATE_END) {
        Event event = get_event(*current_pos);
        State next_state = transition_table[current_state][event];
        
        if (current_state != next_state) {
            if (current_state == STATE_READING_WORD || 
                current_state == STATE_READING_NUMBER || 
                current_state == STATE_PUNCTUATION) {
                process_token(token_start, current_pos, current_state);
            }
            token_start = current_pos;
        }
        
        current_state = next_state;
        
        if (current_state == STATE_END) break;
        
        current_pos++;
        if (current_pos - input >= (long)MAX_INPUT) break;
    }
    
    return 0;
}