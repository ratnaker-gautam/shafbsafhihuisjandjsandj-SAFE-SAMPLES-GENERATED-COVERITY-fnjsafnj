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
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
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
            default:
                current_state = STATE_ERROR;
                break;
        }
    }
    
    print_statistics(&stats);
    return 0;
}

State handle_start(Event ev, Statistics* stats) {
    if (ev == EVENT_DIGIT) {
        return STATE_READING_NUMBER;
    } else if (ev == EVENT_LETTER) {
        return STATE_READING_WORD;
    } else if (ev == EVENT_SPACE) {
        return STATE_START;
    } else if (ev == EVENT_EOF) {
        return STATE_END;
    } else {
        stats->error_count++;
        return STATE_ERROR;
    }
}

State handle_reading_number(Event ev, Statistics* stats) {
    if (ev == EVENT_DIGIT) {
        return STATE_READING_NUMBER;
    } else if (ev == EVENT_SPACE) {
        stats->number_count++;
        return STATE_START;
    } else if (ev == EVENT_EOF) {
        stats->number_count++;
        return STATE_END;
    } else {
        stats->error_count++;
        return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, Statistics* stats) {
    if (ev == EVENT_LETTER) {
        return STATE_READING_WORD;
    } else if (ev == EVENT_SPACE) {
        stats->word_count++;
        return STATE_START;
    } else if (ev == EVENT_EOF) {
        stats->word_count++;
        return STATE_END;
    } else {
        stats->error_count++;
        return STATE_ERROR;
    }
}

State handle_error(Event ev, Statistics* stats) {
    if (ev == EVENT_SPACE) {
        return STATE_START;
    } else if (ev == EVENT_EOF) {
        return STATE_END;
    } else {
        return STATE_ERROR;
    }
}

State handle_end(Event ev, Statistics* stats) {
    return STATE_END;
}

Event get_next_event(FILE* input) {
    int c = fgetc(input);
    
    if (c == EOF) {
        return EVENT_EOF;
    } else if (c == '\n') {
        return EVENT_EOF;
    } else if (isdigit(c)) {
        return EVENT_DIGIT;
    } else if (isalpha(c)) {
        return EVENT_LETTER;
    } else if (isspace(c)) {
        return EVENT_SPACE;
    } else {
        return EVENT_INVALID;
    }
}

void print_statistics(const Statistics* stats) {
    printf("\nAnalysis Results:\n");
    printf("Numbers found: %d\n", stats->number_count);
    printf("Words found: %d\n", stats->word_count);
    printf("Errors found: %d\n", stats->error_count);
}