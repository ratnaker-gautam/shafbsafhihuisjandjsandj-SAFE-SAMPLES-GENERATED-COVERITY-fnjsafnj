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

State transition(State current, Event event) {
    switch(current) {
        case STATE_START:
            switch(event) {
                case EVENT_DIGIT: return STATE_READING_NUMBER;
                case EVENT_LETTER: return STATE_READING_WORD;
                case EVENT_SPACE: return STATE_START;
                case EVENT_EOF: return STATE_END;
                default: return STATE_ERROR;
            }
        case STATE_READING_NUMBER:
            switch(event) {
                case EVENT_DIGIT: return STATE_READING_NUMBER;
                case EVENT_SPACE: return STATE_START;
                case EVENT_EOF: return STATE_END;
                default: return STATE_ERROR;
            }
        case STATE_READING_WORD:
            switch(event) {
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

Event get_event(int c) {
    if (c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_OTHER;
}

int main(void) {
    State current_state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    int current_char;
    char buffer[MAX_INPUT_LEN];
    int buffer_pos = 0;
    
    printf("Enter text (max %d chars per line): ", MAX_INPUT_LEN - 1);
    
    while ((current_char = getchar()) != EOF && buffer_pos < MAX_INPUT_LEN - 1) {
        Event event = get_event(current_char);
        State next_state = transition(current_state, event);
        
        if (next_state == STATE_ERROR) {
            printf("Invalid input sequence detected.\n");
            return 1;
        }
        
        if (current_state == STATE_READING_NUMBER && next_state == STATE_START) {
            number_count++;
        } else if (current_state == STATE_READING_WORD && next_state == STATE_START) {
            word_count++;
        }
        
        current_state = next_state;
        
        if (current_char == '\n') {
            break;
        }
        
        if (buffer_pos < MAX_INPUT_LEN - 1) {
            buffer[buffer_pos++] = (char)current_char;
        }
    }
    
    if (buffer_pos >= MAX_INPUT_LEN - 1) {
        printf("Input too long.\n");
        return 1;
    }
    
    buffer[buffer_pos] = '\0';
    
    if (current_state == STATE_READING_NUMBER) {
        number_count++;
    } else if (current_state == STATE_READING_WORD) {
        word_count++;
    } else if (current_state == STATE_ERROR) {
        printf("Invalid input sequence.\n");
        return 1;
    }
    
    printf("Input: %s\n", buffer);
    printf("Numbers found: %d\n", number_count);
    printf("Words found: %d\n", word_count);
    
    return 0;
}