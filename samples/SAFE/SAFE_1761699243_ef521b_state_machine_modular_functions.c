//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
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

typedef struct {
    int number_count;
    int word_count;
    int error_count;
} Statistics;

State handle_start(Event ev, Statistics* stats);
State handle_reading_number(Event ev, Statistics* stats);
State handle_reading_word(Event ev, Statistics* stats);
State handle_error(Event ev, Statistics* stats);
State handle_end(Event ev, Statistics* stats);

Event get_next_event(FILE* input);
void print_statistics(const Statistics* stats);

int main(void) {
    State current_state = STATE_START;
    Statistics stats = {0, 0, 0};
    Event ev;
    
    printf("Enter text (max %d chars, Ctrl+D to end): ", MAX_INPUT_LEN);
    
    while (current_state != STATE_END) {
        ev = get_next_event(stdin);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start(ev, &stats);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(ev, &stats);
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(ev, &stats);
                break;
            case STATE_ERROR:
                current_state = handle_error(ev, &stats);
                break;
            case STATE_END:
                break;
        }
    }
    
    print_statistics(&stats);
    return 0;
}

Event get_next_event(FILE* input) {
    int c = fgetc(input);
    
    if (c == EOF) {
        return EVENT_EOF;
    }
    
    if (isspace(c)) {
        return EVENT_SPACE;
    }
    
    if (isdigit(c)) {
        return EVENT_DIGIT;
    }
    
    if (isalpha(c)) {
        return EVENT_LETTER;
    }
    
    return EVENT_INVALID;
}

State handle_start(Event ev, Statistics* stats) {
    switch (ev) {
        case EVENT_DIGIT:
            stats->number_count++;
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            stats->word_count++;
            return STATE_READING_WORD;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        case EVENT_INVALID:
            stats->error_count++;
            return STATE_ERROR;
    }
    return STATE_START;
}

State handle_reading_number(Event ev, Statistics* stats) {
    switch (ev) {
        case EVENT_DIGIT:
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
        case EVENT_INVALID:
            stats->error_count++;
            return STATE_ERROR;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
    }
    return STATE_READING_NUMBER;
}

State handle_reading_word(Event ev, Statistics* stats) {
    switch (ev) {
        case EVENT_LETTER:
            return STATE_READING_WORD;
        case EVENT_DIGIT:
        case EVENT_INVALID:
            stats->error_count++;
            return STATE_ERROR;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
    }
    return STATE_READING_WORD;
}

State handle_error(Event ev, Statistics* stats) {
    switch (ev) {
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_end(Event ev, Statistics* stats) {
    return STATE_END;
}

void print_statistics(const Statistics* stats) {
    printf("\nAnalysis Results:\n");
    printf("Numbers found: %d\n", stats->number_count);
    printf("Words found: %d\n", stats->word_count);
    printf("Errors detected: %d\n", stats->error_count);
}