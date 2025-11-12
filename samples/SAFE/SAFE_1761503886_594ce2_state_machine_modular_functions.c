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
} Counter;

static Event get_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_OTHER;
}

static State handle_start(Event evt, Counter* cnt) {
    switch (evt) {
        case EVENT_DIGIT:
            cnt->number_count++;
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            cnt->word_count++;
            return STATE_READING_WORD;
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        default:
            cnt->error_count++;
            return STATE_ERROR;
    }
}

static State handle_reading_number(Event evt, Counter* cnt) {
    switch (evt) {
        case EVENT_DIGIT:
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        case EVENT_LETTER:
        case EVENT_OTHER:
            cnt->error_count++;
            return STATE_ERROR;
    }
    return STATE_ERROR;
}

static State handle_reading_word(Event evt, Counter* cnt) {
    switch (evt) {
        case EVENT_LETTER:
            return STATE_READING_WORD;
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        case EVENT_DIGIT:
        case EVENT_OTHER:
            cnt->error_count++;
            return STATE_ERROR;
    }
    return STATE_ERROR;
}

static State handle_error(Event evt, Counter* cnt) {
    switch (evt) {
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

static void process_input(const char* input, Counter* cnt) {
    State current_state = STATE_START;
    size_t len = strnlen(input, MAX_INPUT_LEN);
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        Event evt = get_event(c);
        State next_state;
        
        switch (current_state) {
            case STATE_START:
                next_state = handle_start(evt, cnt);
                break;
            case STATE_READING_NUMBER:
                next_state = handle_reading_number(evt, cnt);
                break;
            case STATE_READING_WORD:
                next_state = handle_reading_word(evt, cnt);
                break;
            case STATE_ERROR:
                next_state = handle_error(evt, cnt);
                break;
            default:
                next_state = STATE_ERROR;
                cnt->error_count++;
        }
        
        current_state = next_state;
    }
}

static int read_input(char* buffer, size_t size) {
    if (size == 0) return 0;
    
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strnlen(buffer, size - 1);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    Counter counter = {0, 0, 0};
    
    printf("Enter text to analyze: ");
    
    if (!read_input(input, sizeof(input))) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (strnlen(input, MAX_INPUT_LEN) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    process_input(input, &counter);
    
    printf("Numbers found: %d\n", counter.number_count);
    printf("Words found: %d\n", counter.word_count);
    printf("Errors detected: %d\n", counter.error_count);
    
    return 0;
}