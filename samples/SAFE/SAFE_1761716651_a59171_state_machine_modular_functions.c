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
    int number_count;
    int word_count;
    int error_count;
} Counter;

Event get_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State transition(State current, Event event, Counter *counter) {
    switch (current) {
        case STATE_START:
            switch (event) {
                case EVENT_DIGIT:
                    counter->number_count++;
                    return STATE_READING_NUMBER;
                case EVENT_LETTER:
                    counter->word_count++;
                    return STATE_READING_WORD;
                case EVENT_SPACE:
                    return STATE_START;
                case EVENT_EOF:
                    return STATE_END;
                default:
                    counter->error_count++;
                    return STATE_ERROR;
            }
        case STATE_READING_NUMBER:
            switch (event) {
                case EVENT_DIGIT:
                    return STATE_READING_NUMBER;
                case EVENT_SPACE:
                    return STATE_START;
                case EVENT_EOF:
                    return STATE_END;
                default:
                    counter->error_count++;
                    return STATE_ERROR;
            }
        case STATE_READING_WORD:
            switch (event) {
                case EVENT_LETTER:
                    return STATE_READING_WORD;
                case EVENT_SPACE:
                    return STATE_START;
                case EVENT_EOF:
                    return STATE_END;
                default:
                    counter->error_count++;
                    return STATE_ERROR;
            }
        case STATE_ERROR:
            switch (event) {
                case EVENT_SPACE:
                    return STATE_START;
                case EVENT_EOF:
                    return STATE_END;
                default:
                    return STATE_ERROR;
            }
        default:
            return STATE_ERROR;
    }
}

int process_input(const char *input, Counter *counter) {
    if (input == NULL || counter == NULL) return -1;
    
    State current = STATE_START;
    size_t len = strlen(input);
    
    if (len >= MAX_INPUT_LEN) return -1;
    
    for (size_t i = 0; i <= len; i++) {
        Event event = get_event(input[i]);
        current = transition(current, event, counter);
        if (current == STATE_END) break;
    }
    
    return 0;
}

int read_input(char *buffer, size_t size) {
    if (buffer == NULL || size == 0) return -1;
    
    printf("Enter text (max %zu chars): ", size - 1);
    
    if (fgets(buffer, size, stdin) == NULL) return -1;
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 0;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    Counter counter = {0, 0, 0};
    
    if (read_input(input, sizeof(input)) != 0) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    if (process_input(input, &counter) != 0) {
        fprintf(stderr, "Error processing input\n");
        return EXIT_FAILURE;
    }
    
    printf("Numbers: %d\n", counter.number_count);
    printf("Words: %d\n", counter.word_count);
    printf("Errors: %d\n", counter.error_count);
    
    return EXIT_SUCCESS;
}