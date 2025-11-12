//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING_NUMBER,
    STATE_READING_WORD,
    STATE_ERROR,
    STATE_END
} State;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_OTHER
} Event;

Event get_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_OTHER;
}

State transition(State current, Event event) {
    switch (current) {
        case STATE_START:
            switch (event) {
                case EVENT_DIGIT: return STATE_READING_NUMBER;
                case EVENT_LETTER: return STATE_READING_WORD;
                case EVENT_SPACE: return STATE_START;
                case EVENT_EOF: return STATE_END;
                default: return STATE_ERROR;
            }
        case STATE_READING_NUMBER:
            switch (event) {
                case EVENT_DIGIT: return STATE_READING_NUMBER;
                case EVENT_SPACE: return STATE_START;
                case EVENT_EOF: return STATE_END;
                default: return STATE_ERROR;
            }
        case STATE_READING_WORD:
            switch (event) {
                case EVENT_LETTER: return STATE_READING_WORD;
                case EVENT_SPACE: return STATE_START;
                case EVENT_EOF: return STATE_END;
                default: return STATE_ERROR;
            }
        case STATE_ERROR:
            return STATE_ERROR;
        case STATE_END:
            return STATE_END;
    }
    return STATE_ERROR;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    State current_state = STATE_START;
    int numbers = 0;
    int words = 0;
    int pos = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    while (current_state != STATE_END && current_state != STATE_ERROR && pos < MAX_INPUT_LEN) {
        Event event = get_event(input[pos]);
        State next_state = transition(current_state, event);
        
        if (current_state == STATE_START && next_state == STATE_READING_NUMBER) {
            numbers++;
        } else if (current_state == STATE_START && next_state == STATE_READING_WORD) {
            words++;
        }
        
        current_state = next_state;
        
        if (event == EVENT_EOF) {
            break;
        }
        
        pos++;
    }
    
    if (current_state == STATE_ERROR) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", numbers);
    printf("Words found: %d\n", words);
    
    return 0;
}