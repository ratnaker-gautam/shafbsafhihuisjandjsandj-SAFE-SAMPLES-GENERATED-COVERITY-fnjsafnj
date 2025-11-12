//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: state_machine
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

typedef struct {
    int numbers;
    int words;
} counter_t;

event_t get_event(int c) {
    if (c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

state_t transition(state_t current, event_t event, counter_t *counters) {
    switch (current) {
        case STATE_START:
            switch (event) {
                case EVENT_DIGIT:
                    counters->numbers++;
                    return STATE_READING_NUMBER;
                case EVENT_LETTER:
                    counters->words++;
                    return STATE_READING_WORD;
                case EVENT_SPACE:
                    return STATE_START;
                case EVENT_EOF:
                    return STATE_END;
                default:
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
                    return STATE_ERROR;
            }
        case STATE_ERROR:
            return STATE_ERROR;
        case STATE_END:
            return STATE_END;
    }
    return STATE_ERROR;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    counter_t counters = {0, 0};
    state_t current_state = STATE_START;
    
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
    
    int i = 0;
    while (i <= len) {
        int c = (i < len) ? input[i] : EOF;
        event_t event = get_event(c);
        state_t next_state = transition(current_state, event, &counters);
        
        if (next_state == STATE_ERROR) {
            fprintf(stderr, "Invalid input format\n");
            return 1;
        }
        
        current_state = next_state;
        
        if (current_state == STATE_END) {
            break;
        }
        
        if (event != EVENT_EOF) {
            i++;
        }
    }
    
    if (current_state != STATE_END && current_state != STATE_START) {
        fprintf(stderr, "Unexpected end of input\n");
        return 1;
    }
    
    printf("Numbers: %d\n", counters.numbers);
    printf("Words: %d\n", counters.words);
    
    return 0;
}