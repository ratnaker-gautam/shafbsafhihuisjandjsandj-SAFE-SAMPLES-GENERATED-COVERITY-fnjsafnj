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

Event get_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_OTHER;
}

State handle_start(Event evt, Counter* cnt) {
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

State handle_reading_number(Event evt, Counter* cnt) {
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
    return STATE_START;
}

State handle_reading_word(Event evt, Counter* cnt) {
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
    return STATE_START;
}

State handle_error(Event evt, Counter* cnt) {
    switch (evt) {
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State process_event(State current, Event evt, Counter* cnt) {
    switch (current) {
        case STATE_START:
            return handle_start(evt, cnt);
        case STATE_READING_NUMBER:
            return handle_reading_number(evt, cnt);
        case STATE_READING_WORD:
            return handle_reading_word(evt, cnt);
        case STATE_ERROR:
            return handle_error(evt, cnt);
        case STATE_END:
            return STATE_END;
    }
    return STATE_START;
}

int read_input_safe(char* buffer, size_t max_len) {
    if (buffer == NULL || max_len == 0) return 0;
    
    if (fgets(buffer, (int)max_len, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
    }
    
    return 1;
}

void initialize_counter(Counter* cnt) {
    if (cnt == NULL) return;
    cnt->number_count = 0;
    cnt->word_count = 0;
    cnt->error_count = 0;
}

void print_results(const Counter* cnt) {
    if (cnt == NULL) return;
    printf("Numbers found: %d\n", cnt->number_count);
    printf("Words found: %d\n", cnt->word_count);
    printf("Errors detected: %d\n", cnt->error_count);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    Counter counter;
    State current_state = STATE_START;
    
    initialize_counter(&counter);
    
    printf("Enter text to analyze: ");
    if (!read_input_safe(input, sizeof(input))) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t i = 0;
    while (input[i] != '\0' && current_state != STATE_END) {
        Event evt = get_event(input[i]);
        current_state = process_event(current_state, evt, &counter);
        i++;
    }
    
    process_event(current_state, EVENT_EOF, &counter);
    
    print_results(&counter);
    
    return 0;
}