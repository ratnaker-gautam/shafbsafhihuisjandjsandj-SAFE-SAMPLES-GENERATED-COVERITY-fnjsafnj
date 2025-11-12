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
} state_t;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_INVALID
} event_t;

typedef struct {
    int number_count;
    int word_count;
    int error_count;
} counter_t;

static event_t get_event(int c) {
    if (c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

static state_t handle_start(event_t evt, int c, counter_t *counters) {
    switch (evt) {
        case EVENT_DIGIT:
            counters->number_count++;
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            counters->word_count++;
            return STATE_READING_WORD;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            counters->error_count++;
            return STATE_ERROR;
    }
}

static state_t handle_reading_number(event_t evt, int c, counter_t *counters) {
    switch (evt) {
        case EVENT_DIGIT:
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            counters->error_count++;
            return STATE_ERROR;
    }
}

static state_t handle_reading_word(event_t evt, int c, counter_t *counters) {
    switch (evt) {
        case EVENT_LETTER:
            return STATE_READING_WORD;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            counters->error_count++;
            return STATE_ERROR;
    }
}

static state_t handle_error(event_t evt, int c, counter_t *counters) {
    switch (evt) {
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

static void process_input(counter_t *counters) {
    state_t current_state = STATE_START;
    int c;
    
    while (current_state != STATE_END) {
        c = getchar();
        event_t evt = get_event(c);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start(evt, c, counters);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(evt, c, counters);
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(evt, c, counters);
                break;
            case STATE_ERROR:
                current_state = handle_error(evt, c, counters);
                break;
            case STATE_END:
                break;
        }
    }
}

static void print_results(const counter_t *counters) {
    printf("Numbers found: %d\n", counters->number_count);
    printf("Words found: %d\n", counters->word_count);
    printf("Errors found: %d\n", counters->error_count);
}

int main(void) {
    counter_t counters = {0, 0, 0};
    
    printf("Enter text (Ctrl+D to end):\n");
    process_input(&counters);
    print_results(&counters);
    
    return 0;
}