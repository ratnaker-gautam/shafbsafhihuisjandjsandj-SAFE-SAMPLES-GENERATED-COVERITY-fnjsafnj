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
    int numbers;
    int words;
} Counter;

Event get_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
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

State process_event(State current, Event ev, Counter* cnt) {
    switch (current) {
        case STATE_START:
            return handle_start(ev, cnt);
        case STATE_READING_NUMBER:
            return handle_reading_number(ev, cnt);
        case STATE_READING_WORD:
            return handle_reading_word(ev, cnt);
        case STATE_ERROR:
            return handle_error(ev, cnt);
        default:
            return STATE_ERROR;
    }
}

int process_input(const char* input, Counter* cnt) {
    State current = STATE_START;
    size_t len = strlen(input);
    
    if (len >= MAX_INPUT_LEN) {
        return 0;
    }
    
    for (size_t i = 0; i <= len; i++) {
        Event ev = get_event(input[i]);
        current = process_event(current, ev, cnt);
        if (current == STATE_ERROR) {
            return 0;
        }
        if (current == STATE_END) {
            break;
        }
    }
    
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    Counter counter = {0, 0};
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (!process_input(input, &counter)) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    printf("Numbers: %d\n", counter.numbers);
    printf("Words: %d\n", counter.words);
    
    return 0;
}