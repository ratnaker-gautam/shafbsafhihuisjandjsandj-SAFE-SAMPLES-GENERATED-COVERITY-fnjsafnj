//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: state_machine
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
    EVENT_NEWLINE,
    EVENT_EOF,
    EVENT_INVALID
} Event;

State handle_start(Event ev, char ch);
State handle_reading(Event ev, char ch);
State handle_processing(Event ev, char ch);
State handle_final(Event ev, char ch);
State handle_error(Event ev, char ch);

Event get_event(char ch) {
    if (ch == EOF) return EVENT_EOF;
    if (ch == '\n') return EVENT_NEWLINE;
    if (isspace(ch)) return EVENT_SPACE;
    if (isprint(ch)) return EVENT_CHAR;
    return EVENT_INVALID;
}

State handle_start(Event ev, char ch) {
    switch (ev) {
        case EVENT_CHAR:
            putchar(toupper(ch));
            return STATE_READING;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_NEWLINE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_FINAL;
        default:
            return STATE_ERROR;
    }
}

State handle_reading(Event ev, char ch) {
    switch (ev) {
        case EVENT_CHAR:
            putchar(tolower(ch));
            return STATE_READING;
        case EVENT_SPACE:
            putchar(' ');
            return STATE_PROCESSING;
        case EVENT_NEWLINE:
            putchar('\n');
            return STATE_START;
        case EVENT_EOF:
            return STATE_FINAL;
        default:
            return STATE_ERROR;
    }
}

State handle_processing(Event ev, char ch) {
    switch (ev) {
        case EVENT_CHAR:
            putchar(toupper(ch));
            return STATE_READING;
        case EVENT_SPACE:
            return STATE_PROCESSING;
        case EVENT_NEWLINE:
            putchar('\n');
            return STATE_START;
        case EVENT_EOF:
            return STATE_FINAL;
        default:
            return STATE_ERROR;
    }
}

State handle_final(Event ev, char ch) {
    return STATE_FINAL;
}

State handle_error(Event ev, char ch) {
    return STATE_ERROR;
}

int main(void) {
    State current_state = STATE_START;
    char input[MAX_INPUT_LEN];
    int pos = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    while (pos < MAX_INPUT_LEN - 1) {
        int ch = getchar();
        if (ch == EOF) break;
        
        input[pos] = (char)ch;
        pos++;
        
        if (ch == '\n') {
            input[pos] = '\0';
            break;
        }
    }
    
    if (pos == MAX_INPUT_LEN - 1) {
        input[pos] = '\0';
        while (getchar() != '\n' && getchar() != EOF);
    }
    
    printf("Processed: ");
    
    typedef State (*StateHandler)(Event, char);
    StateHandler handlers[MAX_STATES] = {
        handle_start,
        handle_reading,
        handle_processing,
        handle_final,
        handle_error
    };
    
    for (int i = 0; i < pos && input[i] != '\0'; i++) {
        Event ev = get_event(input[i]);
        State next_state = handlers[current_state](ev, input[i]);
        
        if (next_state == STATE_ERROR) {
            fprintf(stderr, "Error processing input\n");
            return 1;
        }
        
        current_state = next_state;
    }
    
    Event final_ev = get_event(EOF);
    State final_state = handlers[current_state](final_ev, EOF);
    
    if (final_state != STATE_FINAL) {
        fprintf(stderr, "Unexpected termination state\n");
        return 1;
    }
    
    if (current_state != STATE_START && current_state != STATE_FINAL) {
        putchar('\n');
    }
    
    return 0;
}