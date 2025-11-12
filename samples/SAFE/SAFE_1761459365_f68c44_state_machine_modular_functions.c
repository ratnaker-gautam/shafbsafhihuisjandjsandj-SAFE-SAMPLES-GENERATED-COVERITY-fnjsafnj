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

Event get_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_OTHER;
}

State transition(State current, Event event, Counter *counter) {
    switch (current) {
        case STATE_START:
            switch (event) {
                case EVENT_DIGIT:
                    counter->numbers++;
                    return STATE_READING_NUMBER;
                case EVENT_LETTER:
                    counter->words++;
                    return STATE_READING_WORD;
                case EVENT_SPACE:
                case EVENT_EOF:
                    return STATE_START;
                default:
                    return STATE_ERROR;
            }
        
        case STATE_READING_NUMBER:
            switch (event) {
                case EVENT_DIGIT:
                    return STATE_READING_NUMBER;
                case EVENT_SPACE:
                case EVENT_EOF:
                    return STATE_START;
                default:
                    return STATE_ERROR;
            }
        
        case STATE_READING_WORD:
            switch (event) {
                case EVENT_LETTER:
                    return STATE_READING_WORD;
                case EVENT_SPACE:
                case EVENT_EOF:
                    return STATE_START;
                default:
                    return STATE_ERROR;
            }
        
        case STATE_ERROR:
            return STATE_ERROR;
        
        case STATE_END:
            return STATE_END;
    }
    return STATE_ERROR;
}

int process_input(const char *input, Counter *counter) {
    if (input == NULL || counter == NULL) return 0;
    
    State current = STATE_START;
    size_t len = strnlen(input, MAX_INPUT_LEN);
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        Event event = get_event(c);
        current = transition(current, event, counter);
        
        if (current == STATE_ERROR) {
            return 0;
        }
        
        if (current == STATE_END) {
            break;
        }
    }
    
    return 1;
}

int read_input(char *buffer, size_t size) {
    if (buffer == NULL || size == 0) return 0;
    
    printf("Enter text (max %zu characters): ", size - 1);
    
    if (fgets(buffer, size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strnlen(buffer, size);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    Counter counter = {0, 0};
    
    if (!read_input(input, sizeof(input))) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (!process_input(input, &counter)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", counter.numbers);
    printf("Words found: %d\n", counter.words);
    
    return 0;
}