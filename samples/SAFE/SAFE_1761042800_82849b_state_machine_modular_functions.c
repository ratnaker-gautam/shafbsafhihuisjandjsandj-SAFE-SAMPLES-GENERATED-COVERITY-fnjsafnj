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
} state_t;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_INVALID
} event_t;

static int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len > MAX_INPUT_LEN) {
        return 0;
    }
    return 1;
}

static event_t classify_char(char c) {
    if (c == '\0') return EVENT_EOF;
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

static state_t handle_start(event_t event) {
    switch (event) {
        case EVENT_DIGIT: return STATE_READING_NUMBER;
        case EVENT_LETTER: return STATE_READING_WORD;
        case EVENT_SPACE: return STATE_START;
        case EVENT_EOF: return STATE_END;
        default: return STATE_ERROR;
    }
}

static state_t handle_reading_number(event_t event) {
    switch (event) {
        case EVENT_DIGIT: return STATE_READING_NUMBER;
        case EVENT_SPACE: return STATE_START;
        case EVENT_EOF: return STATE_END;
        default: return STATE_ERROR;
    }
}

static state_t handle_reading_word(event_t event) {
    switch (event) {
        case EVENT_LETTER: return STATE_READING_WORD;
        case EVENT_SPACE: return STATE_START;
        case EVENT_EOF: return STATE_END;
        default: return STATE_ERROR;
    }
}

static state_t process_event(state_t current_state, event_t event) {
    switch (current_state) {
        case STATE_START: return handle_start(event);
        case STATE_READING_NUMBER: return handle_reading_number(event);
        case STATE_READING_WORD: return handle_reading_word(event);
        case STATE_ERROR: return STATE_ERROR;
        case STATE_END: return STATE_END;
        default: return STATE_ERROR;
    }
}

static void process_input(const char *input) {
    if (!validate_input(input, strlen(input))) {
        printf("Invalid input\n");
        return;
    }

    state_t current_state = STATE_START;
    size_t pos = 0;
    char c;
    int number_count = 0;
    int word_count = 0;

    while (current_state != STATE_END && current_state != STATE_ERROR) {
        c = input[pos];
        event_t event = classify_char(c);
        state_t next_state = process_event(current_state, event);

        if (current_state == STATE_READING_NUMBER && next_state == STATE_START) {
            number_count++;
        } else if (current_state == STATE_READING_WORD && next_state == STATE_START) {
            word_count++;
        }

        current_state = next_state;
        pos++;

        if (pos > MAX_INPUT_LEN) {
            current_state = STATE_ERROR;
            break;
        }
    }

    if (current_state == STATE_END) {
        if (current_state == STATE_READING_NUMBER) number_count++;
        else if (current_state == STATE_READING_WORD) word_count++;
        printf("Numbers: %d, Words: %d\n", number_count, word_count);
    } else if (current_state == STATE_ERROR) {
        printf("Invalid input format\n");
    }
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
    }

    process_input(input);
    return 0;
}