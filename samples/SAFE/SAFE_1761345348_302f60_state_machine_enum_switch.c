//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum State {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_ERROR,
    STATE_END
};

enum Event {
    EVENT_WHITESPACE,
    EVENT_LETTER,
    EVENT_DIGIT,
    EVENT_EOF,
    EVENT_OTHER
};

enum Event get_event(char c) {
    if (c == '\0' || c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_WHITESPACE;
    if (isalpha(c)) return EVENT_LETTER;
    if (isdigit(c)) return EVENT_DIGIT;
    return EVENT_OTHER;
}

int main(void) {
    char input[1024];
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    int word_count = 0;
    int number_count = 0;
    enum State current_state = STATE_START;
    size_t i = 0;
    char c;

    while (current_state != STATE_END && current_state != STATE_ERROR) {
        c = input[i];
        enum Event event = get_event(c);

        switch (current_state) {
            case STATE_START:
                switch (event) {
                    case EVENT_LETTER:
                        current_state = STATE_READING_WORD;
                        word_count++;
                        break;
                    case EVENT_DIGIT:
                        current_state = STATE_READING_NUMBER;
                        number_count++;
                        break;
                    case EVENT_WHITESPACE:
                        break;
                    case EVENT_EOF:
                        current_state = STATE_END;
                        break;
                    case EVENT_OTHER:
                        current_state = STATE_ERROR;
                        break;
                }
                break;

            case STATE_READING_WORD:
                switch (event) {
                    case EVENT_LETTER:
                        break;
                    case EVENT_WHITESPACE:
                        current_state = STATE_START;
                        break;
                    case EVENT_DIGIT:
                    case EVENT_OTHER:
                        current_state = STATE_ERROR;
                        break;
                    case EVENT_EOF:
                        current_state = STATE_END;
                        break;
                }
                break;

            case STATE_READING_NUMBER:
                switch (event) {
                    case EVENT_DIGIT:
                        break;
                    case EVENT_WHITESPACE:
                        current_state = STATE_START;
                        break;
                    case EVENT_LETTER:
                    case EVENT_OTHER:
                        current_state = STATE_ERROR;
                        break;
                    case EVENT_EOF:
                        current_state = STATE_END;
                        break;
                }
                break;

            case STATE_ERROR:
                break;

            case STATE_END:
                break;
        }

        i++;
        if (i >= sizeof(input)) {
            current_state = STATE_ERROR;
        }
    }

    if (current_state == STATE_ERROR) {
        printf("Invalid input format\n");
        return 1;
    }

    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    return 0;
}