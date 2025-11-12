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
    STATE_OUTPUT,
    STATE_END
} State;

typedef enum {
    EVENT_NEWLINE,
    EVENT_ALPHA,
    EVENT_DIGIT,
    EVENT_OTHER,
    EVENT_EOF
} Event;

Event get_next_event(char c) {
    if (c == '\n') return EVENT_NEWLINE;
    if (isalpha(c)) return EVENT_ALPHA;
    if (isdigit(c)) return EVENT_DIGIT;
    if (c == EOF) return EVENT_EOF;
    return EVENT_OTHER;
}

State transition(State current, Event event, int *word_count, int *digit_count) {
    switch (current) {
        case STATE_START:
            if (event == EVENT_ALPHA) {
                (*word_count)++;
                return STATE_READING;
            } else if (event == EVENT_DIGIT) {
                (*digit_count)++;
                return STATE_READING;
            }
            return STATE_START;
        case STATE_READING:
            if (event == EVENT_NEWLINE) {
                return STATE_PROCESSING;
            } else if (event == EVENT_ALPHA) {
                return STATE_READING;
            } else if (event == EVENT_DIGIT) {
                return STATE_READING;
            } else if (event == EVENT_OTHER) {
                return STATE_READING;
            }
            return STATE_READING;
        case STATE_PROCESSING:
            if (event == EVENT_ALPHA) {
                (*word_count)++;
                return STATE_READING;
            } else if (event == EVENT_DIGIT) {
                (*digit_count)++;
                return STATE_READING;
            } else if (event == EVENT_EOF) {
                return STATE_OUTPUT;
            }
            return STATE_PROCESSING;
        case STATE_OUTPUT:
            return STATE_END;
        case STATE_END:
            return STATE_END;
        default:
            return STATE_START;
    }
}

int main(void) {
    State current_state = STATE_START;
    int word_count = 0;
    int digit_count = 0;
    char input[MAX_INPUT_LEN];
    
    printf("Enter text (max %d chars per line, Ctrl+D to end):\n", MAX_INPUT_LEN - 1);
    
    while (current_state != STATE_END) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            current_state = transition(current_state, EVENT_EOF, &word_count, &digit_count);
            continue;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (len >= MAX_INPUT_LEN - 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
        
        for (size_t i = 0; i < len && current_state != STATE_END; i++) {
            Event event = get_next_event(input[i]);
            current_state = transition(current_state, event, &word_count, &digit_count);
        }
        
        if (len > 0) {
            Event event = get_next_event('\n');
            current_state = transition(current_state, event, &word_count, &digit_count);
        }
    }
    
    printf("Words: %d\n", word_count);
    printf("Digits: %d\n", digit_count);
    
    return 0;
}