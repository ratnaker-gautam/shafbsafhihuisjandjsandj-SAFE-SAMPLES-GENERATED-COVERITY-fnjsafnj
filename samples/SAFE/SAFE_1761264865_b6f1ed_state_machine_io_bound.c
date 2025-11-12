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
    EVENT_INVALID
} Event;

Event get_event(char c) {
    if (c == '\0' || c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State transition(State current, Event evt) {
    switch (current) {
        case STATE_START:
            if (evt == EVENT_DIGIT) return STATE_READING_NUMBER;
            if (evt == EVENT_LETTER) return STATE_READING_WORD;
            if (evt == EVENT_SPACE) return STATE_START;
            if (evt == EVENT_EOF) return STATE_END;
            return STATE_ERROR;
        case STATE_READING_NUMBER:
            if (evt == EVENT_DIGIT) return STATE_READING_NUMBER;
            if (evt == EVENT_SPACE) return STATE_START;
            if (evt == EVENT_EOF) return STATE_END;
            return STATE_ERROR;
        case STATE_READING_WORD:
            if (evt == EVENT_LETTER) return STATE_READING_WORD;
            if (evt == EVENT_SPACE) return STATE_START;
            if (evt == EVENT_EOF) return STATE_END;
            return STATE_ERROR;
        case STATE_ERROR:
            if (evt == EVENT_SPACE) return STATE_START;
            if (evt == EVENT_EOF) return STATE_END;
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    int numbers = 0;
    int words = 0;
    State current = STATE_START;
    
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
        char c = input[i];
        Event evt = get_event(c);
        State next = transition(current, evt);
        
        if (current == STATE_READING_NUMBER && next != STATE_READING_NUMBER) {
            numbers++;
        } else if (current == STATE_READING_WORD && next != STATE_READING_WORD) {
            words++;
        }
        
        current = next;
        
        if (current == STATE_ERROR) {
            fprintf(stderr, "Invalid input format at position %zu\n", i);
            return 1;
        }
        
        if (current == STATE_END) {
            break;
        }
    }
    
    printf("Numbers found: %d\n", numbers);
    printf("Words found: %d\n", words);
    
    return 0;
}