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
    int numbers;
    int words;
} Counter;

static Event get_event(char c) {
    if (c == '\0' || c == EOF) return EVENT_EOF;
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
    return EVENT_OTHER;
}

static State handle_start(Event ev, Counter* cnt) {
    switch (ev) {
        case EVENT_DIGIT:
            cnt->numbers++;
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            cnt->words++;
            return STATE_READING_WORD;
        case EVENT_SPACE:
        case EVENT_OTHER:
            return STATE_START;
        case EVENT_EOF:
            return STATE_END;
    }
    return STATE_ERROR;
}

static State handle_reading_number(Event ev, Counter* cnt) {
    switch (ev) {
        case EVENT_DIGIT:
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_LETTER:
        case EVENT_OTHER:
            return STATE_ERROR;
        case EVENT_EOF:
            return STATE_END;
    }
    return STATE_ERROR;
}

static State handle_reading_word(Event ev, Counter* cnt) {
    switch (ev) {
        case EVENT_LETTER:
            return STATE_READING_WORD;
        case EVENT_SPACE:
            return STATE_START;
        case EVENT_DIGIT:
        case EVENT_OTHER:
            return STATE_ERROR;
        case EVENT_EOF:
            return STATE_END;
    }
    return STATE_ERROR;
}

static State handle_error(Event ev, Counter* cnt) {
    if (ev == EVENT_SPACE) return STATE_START;
    if (ev == EVENT_EOF) return STATE_END;
    return STATE_ERROR;
}

static State process_event(State current, Event ev, Counter* cnt) {
    switch (current) {
        case STATE_START:
            return handle_start(ev, cnt);
        case STATE_READING_NUMBER:
            return handle_reading_number(ev, cnt);
        case STATE_READING_WORD:
            return handle_reading_word(ev, cnt);
        case STATE_ERROR:
            return handle_error(ev, cnt);
        case STATE_END:
            return STATE_END;
    }
    return STATE_ERROR;
}

static int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') return 1;
    }
    return 1;
}

static void run_state_machine(const char* input) {
    Counter cnt = {0, 0};
    State current = STATE_START;
    size_t i = 0;
    
    while (current != STATE_END) {
        if (i >= MAX_INPUT_LEN || input[i] == '\0') {
            current = process_event(current, EVENT_EOF, &cnt);
            break;
        }
        
        Event ev = get_event(input[i]);
        current = process_event(current, ev, &cnt);
        
        if (current == STATE_ERROR) {
            printf("Invalid input format at position %zu\n", i);
            return;
        }
        
        i++;
    }
    
    printf("Numbers: %d\n", cnt.numbers);
    printf("Words: %d\n", cnt.words);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input, len)) {
        printf("Invalid input\n");
        return 1;
    }
    
    run_state_machine(input);
    return 0;
}