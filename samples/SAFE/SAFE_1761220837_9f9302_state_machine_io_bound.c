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

State handle_start(Event ev, char ch);
State handle_reading_number(Event ev, char ch);
State handle_reading_word(Event ev, char ch);
State handle_error(Event ev, char ch);
State handle_end(Event ev, char ch);

Event get_event(char ch);
void process_input(void);

State (*state_handlers[MAX_STATES])(Event, char) = {
    handle_start,
    handle_reading_number,
    handle_reading_word,
    handle_error,
    handle_end
};

int main(void) {
    process_input();
    return 0;
}

Event get_event(char ch) {
    if (ch == EOF) return EVENT_EOF;
    if (isspace(ch)) return EVENT_SPACE;
    if (isdigit(ch)) return EVENT_DIGIT;
    if (isalpha(ch)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State handle_start(Event ev, char ch) {
    switch (ev) {
        case EVENT_DIGIT:
            putchar(ch);
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            putchar(ch);
            return STATE_READING_WORD;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_number(Event ev, char ch) {
    switch (ev) {
        case EVENT_DIGIT:
            putchar(ch);
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
            putchar('\n');
            return STATE_START;
        case EVENT_EOF:
            putchar('\n');
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, char ch) {
    switch (ev) {
        case EVENT_LETTER:
            putchar(ch);
            return STATE_READING_WORD;
        case EVENT_SPACE:
            putchar('\n');
            return STATE_START;
        case EVENT_EOF:
            putchar('\n');
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Event ev, char ch) {
    fprintf(stderr, "Invalid input sequence\n");
    return STATE_END;
}

State handle_end(Event ev, char ch) {
    return STATE_END;
}

void process_input(void) {
    State current_state = STATE_START;
    char input[MAX_INPUT_LEN + 1];
    size_t pos = 0;
    int c;
    
    while (current_state != STATE_END && pos < MAX_INPUT_LEN) {
        c = getchar();
        if (c == EOF) {
            current_state = state_handlers[current_state](EVENT_EOF, c);
            break;
        }
        
        if (pos >= MAX_INPUT_LEN) {
            current_state = STATE_ERROR;
            break;
        }
        
        input[pos] = (char)c;
        Event ev = get_event(input[pos]);
        State next_state = state_handlers[current_state](ev, input[pos]);
        
        if (next_state == STATE_ERROR) {
            current_state = STATE_ERROR;
            break;
        }
        
        current_state = next_state;
        pos++;
        
        if (current_state == STATE_END) {
            break;
        }
    }
    
    if (current_state == STATE_ERROR) {
        fprintf(stderr, "Input processing error\n");
    }
}