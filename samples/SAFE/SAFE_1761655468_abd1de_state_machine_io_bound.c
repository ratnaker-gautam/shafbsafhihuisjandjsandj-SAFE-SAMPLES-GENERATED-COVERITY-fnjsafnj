//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINAL,
    STATE_ERROR
} State;

typedef enum {
    EVENT_CHAR,
    EVENT_SPACE,
    EVENT_DIGIT,
    EVENT_EOF,
    EVENT_INVALID
} Event;

Event get_event(char c) {
    if (c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_CHAR;
    return EVENT_INVALID;
}

State transition(State current, Event event) {
    switch (current) {
        case STATE_START:
            if (event == EVENT_CHAR) return STATE_READING;
            if (event == EVENT_SPACE) return STATE_START;
            return STATE_ERROR;
        case STATE_READING:
            if (event == EVENT_CHAR) return STATE_READING;
            if (event == EVENT_SPACE) return STATE_PROCESSING;
            if (event == EVENT_DIGIT) return STATE_READING;
            return STATE_ERROR;
        case STATE_PROCESSING:
            if (event == EVENT_CHAR) return STATE_READING;
            if (event == EVENT_SPACE) return STATE_PROCESSING;
            if (event == EVENT_EOF) return STATE_FINAL;
            return STATE_ERROR;
        case STATE_FINAL:
            return STATE_FINAL;
        case STATE_ERROR:
            return STATE_ERROR;
    }
    return STATE_ERROR;
}

int main(void) {
    State current_state = STATE_START;
    char input[MAX_INPUT_LEN + 1];
    int word_count = 0;
    int valid_input = 1;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len >= MAX_INPUT_LEN) {
        printf("Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        Event event = get_event(c);
        
        State next_state = transition(current_state, event);
        
        if (next_state == STATE_ERROR) {
            valid_input = 0;
            break;
        }
        
        if (current_state == STATE_READING && next_state == STATE_PROCESSING) {
            word_count++;
        }
        
        current_state = next_state;
        
        if (current_state == STATE_FINAL) {
            break;
        }
    }
    
    if (!valid_input || current_state != STATE_FINAL) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Word count: %d\n", word_count);
    return 0;
}