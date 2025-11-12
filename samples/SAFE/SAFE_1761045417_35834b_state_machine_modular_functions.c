//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    int numbers;
    int words;
} Counter;

Event get_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State handle_start(Event ev, Counter* cnt) {
    switch (ev) {
        case EVENT_DIGIT:
            cnt->numbers++;
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            cnt->words++;
            return STATE_READING_WORD;
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_number(Event ev, Counter* cnt) {
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

State handle_reading_word(Event ev, Counter* cnt) {
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

State handle_error(Event ev, Counter* cnt) {
    return STATE_ERROR;
}

State handle_end(Event ev, Counter* cnt) {
    return STATE_END;
}

typedef State (*StateHandler)(Event, Counter*);

StateHandler state_handlers[] = {
    handle_start,
    handle_reading_number,
    handle_reading_word,
    handle_error,
    handle_end
};

int process_input(const char* input, Counter* counter) {
    State current_state = STATE_START;
    size_t len = strnlen(input, MAX_INPUT_LEN);
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        Event event = get_event(c);
        
        if (current_state < 0 || current_state >= sizeof(state_handlers)/sizeof(state_handlers[0])) {
            return -1;
        }
        
        StateHandler handler = state_handlers[current_state];
        State next_state = handler(event, counter);
        
        if (next_state == STATE_ERROR) {
            return -1;
        }
        
        current_state = next_state;
        
        if (current_state == STATE_END) {
            break;
        }
    }
    
    return 0;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    Counter counter = {0, 0};
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strnlen(input, sizeof(input));
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (process_input(input, &counter) != 0) {
        fprintf(stderr, "Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    printf("Analysis results:\n");
    printf("Numbers found: %d\n", counter.numbers);
    printf("Words found: %d\n", counter.words);
    
    return EXIT_SUCCESS;
}