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
    int errors;
} Counter;

static Event get_event(char c) {
    if (c == '\0' || c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

static State handle_start(Event evt, Counter *cnt) {
    switch (evt) {
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
            cnt->errors++;
            return STATE_ERROR;
    }
}

static State handle_reading_number(Event evt, Counter *cnt) {
    switch (evt) {
        case EVENT_DIGIT:
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        case EVENT_LETTER:
        case EVENT_INVALID:
            cnt->errors++;
            return STATE_ERROR;
        default:
            return STATE_READING_NUMBER;
    }
}

static State handle_reading_word(Event evt, Counter *cnt) {
    switch (evt) {
        case EVENT_LETTER:
            return STATE_READING_WORD;
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        case EVENT_DIGIT:
        case EVENT_INVALID:
            cnt->errors++;
            return STATE_ERROR;
        default:
            return STATE_READING_WORD;
    }
}

static State handle_error(Event evt, Counter *cnt) {
    switch (evt) {
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

static void process_input(const char *input, Counter *cnt) {
    State current_state = STATE_START;
    size_t len = strnlen(input, MAX_INPUT_LEN);
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        Event evt = get_event(c);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start(evt, cnt);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(evt, cnt);
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(evt, cnt);
                break;
            case STATE_ERROR:
                current_state = handle_error(evt, cnt);
                break;
            case STATE_END:
                return;
        }
    }
}

static int read_input(char *buffer, size_t size) {
    if (size == 0) return 0;
    
    printf("Enter text (max %zu chars): ", size - 1);
    
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 0;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    Counter counter = {0, 0, 0};
    
    if (read_input(input, sizeof(input)) != 0) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    process_input(input, &counter);
    
    printf("Numbers: %d\n", counter.numbers);
    printf("Words: %d\n", counter.words);
    printf("Errors: %d\n", counter.errors);
    
    return EXIT_SUCCESS;
}