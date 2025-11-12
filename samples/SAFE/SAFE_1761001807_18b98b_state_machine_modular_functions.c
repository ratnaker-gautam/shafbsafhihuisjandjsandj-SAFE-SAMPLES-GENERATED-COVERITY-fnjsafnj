//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    EVENT_OTHER
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

Event get_event(char c) {
    if (c == EOF) return EVENT_EOF;
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

State handle_end(Event ev, Statistics* stats) {
    return STATE_END;
}

typedef State (*StateHandler)(Event, Statistics*);

StateHandler get_handler(State state) {
    static StateHandler handlers[] = {
        handle_start,
        handle_reading_number,
        handle_reading_word,
        handle_error,
        handle_end
    };
    
    if (state >= 0 && state < (int)(sizeof(handlers)/sizeof(handlers[0]))) {
        return handlers[state];
    }
    return NULL;
}

void process_input(const char* input, Statistics* stats) {
    State current_state = STATE_START;
    size_t len = strlen(input);
    
    for (size_t i = 0; i <= len; i++) {
        char c = (i < len) ? input[i] : EOF;
        Event ev = get_event(c);
        StateHandler handler = get_handler(current_state);
        
        if (handler != NULL) {
            current_state = handler(ev, stats);
        } else {
            current_state = STATE_ERROR;
        }
        
        if (current_state == STATE_END) {
            break;
        }
    }
}

int get_user_input(char* buffer, size_t max_len) {
    if (buffer == NULL || max_len == 0) {
        return 0;
    }
    
    printf("Enter text (max %zu characters): ", max_len - 1);
    
    if (fgets(buffer, (int)max_len, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    Statistics stats = {0, 0, 0};
    
    if (!get_user_input(input, sizeof(input))) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (strlen(input) >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    process_input(input, &stats);
    
    printf("Analysis results:\n");
    printf("Numbers found: %d\n", stats.number_count);
    printf("Words found: %d\n", stats.word_count);
    printf("Errors detected: %d\n", stats.error_count);
    
    return 0;
}