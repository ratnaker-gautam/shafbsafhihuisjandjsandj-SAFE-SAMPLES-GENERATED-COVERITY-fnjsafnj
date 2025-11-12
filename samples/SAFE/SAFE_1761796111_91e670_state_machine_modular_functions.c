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
    EVENT_TERMINATOR,
    EVENT_INVALID
} Event;

typedef struct {
    int number_count;
    int word_count;
    int error_count;
} Statistics;

Event get_event(char c) {
    if (c == '\0') return EVENT_TERMINATOR;
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
            return STATE_START;
        case EVENT_TERMINATOR:
            return STATE_END;
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
            return STATE_START;
        case EVENT_TERMINATOR:
            return STATE_END;
        default:
            stats->error_count++;
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, Statistics* stats) {
    switch (ev) {
        case EVENT_LETTER:
            return STATE_READING_WORD;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_TERMINATOR:
            return STATE_END;
        default:
            stats->error_count++;
            return STATE_ERROR;
    }
}

State handle_error(Event ev, Statistics* stats) {
    switch (ev) {
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_TERMINATOR:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

void process_input(const char* input, Statistics* stats) {
    State current_state = STATE_START;
    size_t i = 0;
    
    while (current_state != STATE_END && i < MAX_INPUT_LEN) {
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
            case STATE_END:
                break;
        }
        
        if (ev == EVENT_TERMINATOR) {
            break;
        }
        i++;
    }
}

int get_user_input(char* buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0) {
        return 0;
    }
    
    printf("Enter text (max %zu characters): ", buffer_size - 1);
    
    if (fgets(buffer, buffer_size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 1;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    Statistics stats = {0};
    
    if (!get_user_input(input_buffer, sizeof(input_buffer))) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (strlen(input_buffer) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    process_input(input_buffer, &stats);
    
    printf("Analysis results:\n");
    printf("Numbers found: %d\n", stats.number_count);
    printf("Words found: %d\n", stats.word_count);
    printf("Errors detected: %d\n", stats.error_count);
    
    return 0;
}