//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: state_machine
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
    EVENT_INVALID
} Event;

Event get_event(char c) {
    if (c == '\0' || c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State transition(State current, Event event) {
    switch (current) {
        case STATE_START:
            if (event == EVENT_DIGIT) return STATE_READING_NUMBER;
            if (event == EVENT_LETTER) return STATE_READING_WORD;
            if (event == EVENT_SPACE) return STATE_START;
            if (event == EVENT_EOF) return STATE_END;
            return STATE_ERROR;
        case STATE_READING_NUMBER:
            if (event == EVENT_DIGIT) return STATE_READING_NUMBER;
            if (event == EVENT_SPACE) return STATE_START;
            if (event == EVENT_EOF) return STATE_END;
            return STATE_ERROR;
        case STATE_READING_WORD:
            if (event == EVENT_LETTER) return STATE_READING_WORD;
            if (event == EVENT_SPACE) return STATE_START;
            if (event == EVENT_EOF) return STATE_END;
            return STATE_ERROR;
        case STATE_ERROR:
            if (event == EVENT_SPACE) return STATE_START;
            if (event == EVENT_EOF) return STATE_END;
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    State current_state = STATE_START;
    int number_count = 0;
    int word_count = 0;
    int pos = 0;
    int valid_input = 1;

    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }

    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }

    while (pos <= len && current_state != STATE_END && current_state != STATE_ERROR) {
        Event event = get_event(input[pos]);
        State next_state = transition(current_state, event);

        if (current_state == STATE_READING_NUMBER && next_state == STATE_START) {
            number_count++;
        } else if (current_state == STATE_READING_WORD && next_state == STATE_START) {
            word_count++;
        }

        current_state = next_state;
        pos++;
    }

    if (current_state == STATE_ERROR) {
        printf("Invalid input format\n");
        valid_input = 0;
    } else {
        if (current_state == STATE_READING_NUMBER) {
            number_count++;
        } else if (current_state == STATE_READING_WORD) {
            word_count++;
        }
    }

    if (valid_input) {
        printf("Numbers: %d\n", number_count);
        printf("Words: %d\n", word_count);
    }

    return valid_input ? 0 : 1;
}