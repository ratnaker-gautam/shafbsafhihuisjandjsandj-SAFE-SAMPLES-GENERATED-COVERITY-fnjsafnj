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
    int errors;
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
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            cnt->errors++;
            return STATE_ERROR;
    }
}

State handle_reading_number(Event ev, Counter* cnt) {
    switch (ev) {
        case EVENT_DIGIT:
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            cnt->errors++;
            return STATE_ERROR;
    }
}

State handle_reading_word(Event ev, Counter* cnt) {
    switch (ev) {
        case EVENT_LETTER:
            return STATE_READING_WORD;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            cnt->errors++;
            return STATE_ERROR;
    }
}

State handle_error(Event ev, Counter* cnt) {
    switch (ev) {
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
        default:
            return STATE_ERROR;
    }
}

void process_input(const char* input, Counter* cnt) {
    State current_state = STATE_START;
    size_t len = strlen(input);
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        Event ev = get_event(c);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start(ev, cnt);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(ev, cnt);
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(ev, cnt);
                break;
            case STATE_ERROR:
                current_state = handle_error(ev, cnt);
                break;
            case STATE_END:
                return;
        }
    }
}

int get_user_input(char* buffer, size_t max_len) {
    if (buffer == NULL || max_len == 0) {
        return -1;
    }
    
    printf("Enter text to analyze (max %zu chars): ", max_len - 1);
    
    if (fgets(buffer, max_len, stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 0;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    Counter counter = {0, 0, 0};
    
    if (get_user_input(input, sizeof(input)) != 0) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    process_input(input, &counter);
    
    printf("Analysis results:\n");
    printf("Numbers found: %d\n", counter.numbers);
    printf("Words found: %d\n", counter.words);
    printf("Errors detected: %d\n", counter.errors);
    
    return EXIT_SUCCESS;
}