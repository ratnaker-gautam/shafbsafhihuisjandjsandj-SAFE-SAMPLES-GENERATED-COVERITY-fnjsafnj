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
    if (c == '\0' || c == EOF) return EVENT_EOF;
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
        case EVENT_EOF:
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
        case EVENT_EOF:
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
        case EVENT_EOF:
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
        case EVENT_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

void process_input(const char* input, Statistics* stats) {
    State current_state = STATE_START;
    size_t i = 0;
    char c;
    
    while (current_state != STATE_END) {
        if (i < MAX_INPUT_LEN) {
            c = input[i];
            i++;
        } else {
            c = '\0';
        }
        
        Event ev = get_event(c);
        
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
    }
}

int read_input_safe(char* buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0) {
        return 0;
    }
    
    if (fgets(buffer, (int)buffer_size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else if (len == buffer_size - 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    Statistics stats = {0, 0, 0};
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (!read_input_safe(input, sizeof(input))) {
        printf("Error reading input\n");
        return 1;
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    process_input(input, &stats);
    
    printf("Analysis results:\n");
    printf("Numbers found: %d\n", stats.number_count);
    printf("Words found: %d\n", stats.word_count);
    printf("Errors detected: %d\n", stats.error_count);
    
    return 0;
}