//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 5

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
        case STATE_END:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

void process_input(const char* input) {
    State current = STATE_START;
    int pos = 0;
    char c;
    
    while (current != STATE_END && current != STATE_ERROR) {
        c = input[pos];
        Event evt = get_event(c);
        State next = transition(current, evt);
        
        if (current == STATE_START && next == STATE_READING_NUMBER) {
            printf("Found number: ");
        } else if (current == STATE_START && next == STATE_READING_WORD) {
            printf("Found word: ");
        }
        
        if ((current == STATE_READING_NUMBER || current == STATE_READING_WORD) && 
            (next == STATE_START || next == STATE_END)) {
            printf("\n");
        }
        
        if (current == STATE_READING_NUMBER || current == STATE_READING_WORD) {
            if (evt != EVENT_SPACE && evt != EVENT_EOF) {
                putchar(c);
            }
        }
        
        current = next;
        pos++;
        
        if (pos >= MAX_INPUT_LEN) {
            break;
        }
    }
    
    if (current == STATE_ERROR) {
        printf("Error: Invalid input format\n");
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 1;
    }
    
    process_input(input);
    return 0;
}