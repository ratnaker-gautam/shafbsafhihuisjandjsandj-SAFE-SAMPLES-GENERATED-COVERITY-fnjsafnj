//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
    int numbers_found;
    int words_found;
} Counter;

Event get_next_event(char c) {
    if (c >= '0' && c <= '9') return EVENT_DIGIT;
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return EVENT_LETTER;
    if (c == ' ' || c == '\t' || c == '\n') return EVENT_SPACE;
    if (c == '\0') return EVENT_EOF;
    return EVENT_INVALID;
}

State handle_state_start(Event event, Counter* counter) {
    switch (event) {
        case EVENT_DIGIT:
            counter->numbers_found++;
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            counter->words_found++;
            return STATE_READING_WORD;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_state_reading_number(Event event, Counter* counter) {
    switch (event) {
        case EVENT_DIGIT:
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_state_reading_word(Event event, Counter* counter) {
    switch (event) {
        case EVENT_LETTER:
            return STATE_READING_WORD;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

State handle_state_error(void) {
    return STATE_ERROR;
}

State process_state(State current_state, Event event, Counter* counter) {
    switch (current_state) {
        case STATE_START:
            return handle_state_start(event, counter);
        case STATE_READING_NUMBER:
            return handle_state_reading_number(event, counter);
        case STATE_READING_WORD:
            return handle_state_reading_word(event, counter);
        case STATE_ERROR:
            return handle_state_error();
        default:
            return STATE_ERROR;
    }
}

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    return 1;
}

void run_state_machine(const char* input, Counter* counter) {
    if (!validate_input(input)) {
        printf("Invalid input\n");
        return;
    }

    State current_state = STATE_START;
    size_t i = 0;
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        if (i >= MAX_INPUT_LEN) {
            current_state = STATE_ERROR;
            break;
        }
        
        char current_char = input[i];
        Event event = get_next_event(current_char);
        current_state = process_state(current_state, event, counter);
        i++;
        
        if (current_char == '\0') break;
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input format\n");
        counter->numbers_found = 0;
        counter->words_found = 0;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    Counter counter = {0, 0};
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || len >= MAX_INPUT_LEN) {
        printf("Invalid input length\n");
        return 1;
    }
    
    run_state_machine(input, &counter);
    
    if (counter.numbers_found > 0 || counter.words_found > 0) {
        printf("Found %d numbers and %d words\n", counter.numbers_found, counter.words_found);
    }
    
    return 0;
}