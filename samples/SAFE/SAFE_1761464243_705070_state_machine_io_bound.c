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
    EVENT_END,
    EVENT_INVALID
} Event;

Event get_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_END;
    if (isspace(c)) return EVENT_SPACE;
    if (isprint(c)) return EVENT_CHAR;
    return EVENT_INVALID;
}

State transition(State current, Event ev, int *word_count) {
    switch (current) {
        case STATE_START:
            if (ev == EVENT_CHAR) {
                (*word_count)++;
                return STATE_READING;
            }
            if (ev == EVENT_SPACE) return STATE_START;
            if (ev == EVENT_END) return STATE_FINAL;
            return STATE_ERROR;
        
        case STATE_READING:
            if (ev == EVENT_CHAR) return STATE_READING;
            if (ev == EVENT_SPACE) return STATE_PROCESSING;
            if (ev == EVENT_END) return STATE_FINAL;
            return STATE_ERROR;
        
        case STATE_PROCESSING:
            if (ev == EVENT_CHAR) {
                (*word_count)++;
                return STATE_READING;
            }
            if (ev == EVENT_SPACE) return STATE_PROCESSING;
            if (ev == EVENT_END) return STATE_FINAL;
            return STATE_ERROR;
        
        case STATE_FINAL:
            return STATE_FINAL;
        
        case STATE_ERROR:
            return STATE_ERROR;
    }
    return STATE_ERROR;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    State current_state = STATE_START;
    int word_count = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i <= len; i++) {
        Event ev = get_event(input[i]);
        State next_state = transition(current_state, ev, &word_count);
        
        if (next_state == STATE_ERROR) {
            fprintf(stderr, "Invalid input character at position %zu\n", i);
            return 1;
        }
        
        current_state = next_state;
        
        if (current_state == STATE_FINAL) {
            break;
        }
    }
    
    if (current_state != STATE_FINAL) {
        fprintf(stderr, "Unexpected end of input\n");
        return 1;
    }
    
    printf("Word count: %d\n", word_count);
    return 0;
}