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
    EVENT_INVALID
} Event;

typedef struct {
    int number_count;
    int word_count;
    int total_chars;
} Statistics;

Event get_event(int c) {
    if (c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
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
        case EVENT_EOF:
            return STATE_START;
        default:
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
        case EVENT_INVALID:
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, Statistics* stats) {
    switch (ev) {
        case EVENT_LETTER:
            return STATE_READING_WORD;
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        case EVENT_DIGIT:
        case EVENT_INVALID:
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Event ev, Statistics* stats) {
    if (ev == EVENT_SPACE || ev == EVENT_EOF) {
        return STATE_START;
    }
    return STATE_ERROR;
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
        default:
            return STATE_ERROR;
    }
}

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

void print_statistics(const Statistics* stats) {
    printf("Statistics:\n");
    printf("  Numbers found: %d\n", stats->number_count);
    printf("  Words found: %d\n", stats->word_count);
    printf("  Total characters: %d\n", stats->total_chars);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    Statistics stats = {0, 0, 0};
    State current_state = STATE_START;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        stats.total_chars++;
        Event ev = get_event(input[i]);
        current_state = process_event(current_state, ev, &stats);
        
        if (current_state == STATE_ERROR) {
            fprintf(stderr, "Invalid input format at position %zu\n", i + 1);
            return 1;
        }
    }
    
    Event final_ev = get_event(EOF);
    current_state = process_event(current_state, final_ev, &stats);
    
    if (current_state != STATE_START && current_state != STATE_END) {
        fprintf(stderr, "Input ended in incomplete state\n");
        return 1;
    }
    
    print_statistics(&stats);
    return 0;
}