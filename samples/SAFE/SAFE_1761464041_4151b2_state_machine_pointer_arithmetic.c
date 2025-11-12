//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 256
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINALIZING,
    STATE_DONE
} State;

typedef enum {
    EVENT_NONE,
    EVENT_CHAR,
    EVENT_SPACE,
    EVENT_TERMINATOR
} Event;

Event get_event(char c) {
    if (c == '\0') return EVENT_TERMINATOR;
    if (c == ' ' || c == '\t' || c == '\n') return EVENT_SPACE;
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return EVENT_CHAR;
    return EVENT_NONE;
}

State transition(State current, Event event, int *word_count) {
    State next = current;
    
    switch (current) {
        case STATE_START:
            if (event == EVENT_CHAR) {
                next = STATE_READING;
                *word_count += 1;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_SPACE) {
                next = STATE_PROCESSING;
            } else if (event == EVENT_TERMINATOR) {
                next = STATE_FINALIZING;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_CHAR) {
                next = STATE_READING;
                *word_count += 1;
            } else if (event == EVENT_TERMINATOR) {
                next = STATE_FINALIZING;
            }
            break;
            
        case STATE_FINALIZING:
            next = STATE_DONE;
            break;
            
        case STATE_DONE:
            break;
    }
    
    return next;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char *ptr = input;
    int word_count = 0;
    State current_state = STATE_START;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && *(ptr + len - 1) == '\n') {
        *(ptr + len - 1) = '\0';
        len--;
    }
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    while (current_state != STATE_DONE && *ptr != '\0') {
        Event event = get_event(*ptr);
        current_state = transition(current_state, event, &word_count);
        ptr++;
    }
    
    if (current_state != STATE_DONE) {
        Event final_event = get_event(*ptr);
        current_state = transition(current_state, final_event, &word_count);
    }
    
    printf("Word count: %d\n", word_count);
    
    return EXIT_SUCCESS;
}