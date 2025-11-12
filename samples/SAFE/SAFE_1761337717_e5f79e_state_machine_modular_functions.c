//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
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

typedef struct {
    int number_count;
    int word_count;
    int error_count;
} Statistics;

Event get_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_OTHER;
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
        case EVENT_EOF:
            return STATE_START;
        default:
            stats->error_count++;
            return STATE_ERROR;
    }
}

State handle_reading_number(Event ev, Statistics* stats) {
    switch (ev) {
        case EVENT_DIGIT:
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        case EVENT_LETTER:
        case EVENT_OTHER:
            stats->error_count++;
            return STATE_ERROR;
    }
    return STATE_START;
}

State handle_reading_word(Event ev, Statistics* stats) {
    switch (ev) {
        case EVENT_LETTER:
            return STATE_READING_WORD;
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        case EVENT_DIGIT:
        case EVENT_OTHER:
            stats->error_count++;
            return STATE_ERROR;
    }
    return STATE_START;
}

State handle_error(Event ev, Statistics* stats) {
    switch (ev) {
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State process_event(State current_state, Event ev, Statistics* stats) {
    switch (current_state) {
        case STATE_START:
            return handle_start(ev, stats);
        case STATE_READING_NUMBER:
            return handle_reading_number(ev, stats);
        case STATE_READING_WORD:
            return handle_reading_word(ev, stats);
        case STATE_ERROR:
            return handle_error(ev, stats);
        case STATE_END:
            return STATE_END;
    }
    return STATE_START;
}

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    return 1;
}

void process_input(const char* input, Statistics* stats) {
    if (!validate_input(input) || stats == NULL) return;
    
    State current_state = STATE_START;
    size_t i = 0;
    
    while (input[i] != '\0' && current_state != STATE_END) {
        Event ev = get_event(input[i]);
        current_state = process_event(current_state, ev, stats);
        i++;
    }
    
    process_event(current_state, EVENT_EOF, stats);
}

void print_statistics(const Statistics* stats) {
    if (stats == NULL) return;
    printf("Numbers: %d\n", stats->number_count);
    printf("Words: %d\n", stats->word_count);
    printf("Errors: %d\n", stats->error_count);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    Statistics stats = {0, 0, 0};
    
    printf("Enter text to analyze: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    process_input(input, &stats);
    print_statistics(&stats);
    
    return 0;
}