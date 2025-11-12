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

typedef struct {
    int number_count;
    int word_count;
    int error_count;
} Statistics;

Event get_event(char c) {
    if (c == '\0' || c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State transition(State current, Event event, Statistics *stats) {
    switch (current) {
        case STATE_START:
            switch (event) {
                case EVENT_DIGIT:
                    stats->number_count++;
                    return STATE_READING_NUMBER;
                case EVENT_LETTER:
                    stats->word_count++;
                    return STATE_READING_WORD;
                case EVENT_SPACE:
                    return STATE_START;
                case EVENT_EOF:
                    return STATE_END;
                default:
                    stats->error_count++;
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
                    stats->error_count++;
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
                    stats->error_count++;
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

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    State current_state = STATE_START;
    Statistics stats = {0, 0, 0};
    int pos = 0;
    
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
    
    while (pos <= len) {
        char current_char = (pos < len) ? input[pos] : '\0';
        Event event = get_event(current_char);
        State next_state = transition(current_state, event, &stats);
        
        current_state = next_state;
        pos++;
        
        if (current_state == STATE_END) {
            break;
        }
    }
    
    printf("Analysis complete:\n");
    printf("Numbers found: %d\n", stats.number_count);
    printf("Words found: %d\n", stats.word_count);
    printf("Errors detected: %d\n", stats.error_count);
    
    return 0;
}