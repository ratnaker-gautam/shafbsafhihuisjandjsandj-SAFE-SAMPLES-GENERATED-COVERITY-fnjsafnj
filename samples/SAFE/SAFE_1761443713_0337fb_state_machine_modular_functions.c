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
    int error_count;
} Statistics;

Event get_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
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
        case EVENT_INVALID:
            stats->error_count++;
            return STATE_ERROR;
        default:
            return STATE_READING_NUMBER;
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
            stats->error_count++;
            return STATE_ERROR;
        default:
            return STATE_READING_WORD;
    }
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

void process_input(const char* input, Statistics* stats) {
    State current_state = STATE_START;
    size_t i = 0;
    
    while (input[i] != '\0' && current_state != STATE_END) {
        Event ev = get_event(input[i]);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start(ev, stats);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(ev, stats);
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(ev, stats);
                break;
            case STATE_ERROR:
                current_state = handle_error(ev, stats);
                break;
            default:
                current_state = STATE_ERROR;
                break;
        }
        
        i++;
        if (i >= MAX_INPUT_LEN) break;
    }
}

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    return 1;
}

void print_statistics(const Statistics* stats) {
    printf("Numbers found: %d\n", stats->number_count);
    printf("Words found: %d\n", stats->word_count);
    printf("Errors found: %d\n", stats->error_count);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    Statistics stats = {0, 0, 0};
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT_LEN);
    
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