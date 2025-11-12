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
    STATE_FINISHED,
    STATE_ERROR
} State;

typedef enum {
    EVENT_CHAR,
    EVENT_SPACE,
    EVENT_NEWLINE,
    EVENT_EOF,
    EVENT_INVALID
} Event;

Event get_event(char c) {
    if (c == EOF) return EVENT_EOF;
    if (c == '\n') return EVENT_NEWLINE;
    if (isspace(c)) return EVENT_SPACE;
    if (isprint(c)) return EVENT_CHAR;
    return EVENT_INVALID;
}

State transition(State current, Event event, int *word_count) {
    switch (current) {
        case STATE_START:
            if (event == EVENT_CHAR) {
                (*word_count)++;
                return STATE_READING;
            }
            if (event == EVENT_SPACE || event == EVENT_NEWLINE) return STATE_START;
            if (event == EVENT_EOF) return STATE_FINISHED;
            return STATE_ERROR;
        
        case STATE_READING:
            if (event == EVENT_CHAR) return STATE_READING;
            if (event == EVENT_SPACE) return STATE_PROCESSING;
            if (event == EVENT_NEWLINE) return STATE_PROCESSING;
            if (event == EVENT_EOF) return STATE_FINISHED;
            return STATE_ERROR;
        
        case STATE_PROCESSING:
            if (event == EVENT_CHAR) {
                (*word_count)++;
                return STATE_READING;
            }
            if (event == EVENT_SPACE || event == EVENT_NEWLINE) return STATE_PROCESSING;
            if (event == EVENT_EOF) return STATE_FINISHED;
            return STATE_ERROR;
        
        case STATE_FINISHED:
            return STATE_FINISHED;
        
        case STATE_ERROR:
            return STATE_ERROR;
    }
    return STATE_ERROR;
}

int main(void) {
    State current_state = STATE_START;
    int word_count = 0;
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
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
        
        State next_state = transition(current_state, event, &word_count);
        
        if (next_state == STATE_ERROR) {
            printf("Invalid input character at position %zu\n", i);
            return 1;
        }
        
        current_state = next_state;
        
        if (current_state == STATE_FINISHED) {
            break;
        }
    }
    
    if (current_state != STATE_FINISHED && current_state != STATE_ERROR) {
        current_state = transition(current_state, EVENT_EOF, &word_count);
    }
    
    if (current_state == STATE_ERROR) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Word count: %d\n", word_count);
    
    return 0;
}