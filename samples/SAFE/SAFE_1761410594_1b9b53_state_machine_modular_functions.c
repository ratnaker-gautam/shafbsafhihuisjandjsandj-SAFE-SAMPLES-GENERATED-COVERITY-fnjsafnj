//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    EVENT_OTHER
} Event;

typedef struct {
    int number_count;
    int word_count;
} Counter;

Event get_event(char c) {
    if (c == '\0' || c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_OTHER;
}

State transition(State current, Event event, Counter *counter) {
    switch (current) {
        case STATE_START:
            if (event == EVENT_DIGIT) {
                counter->number_count++;
                return STATE_READING_NUMBER;
            } else if (event == EVENT_LETTER) {
                counter->word_count++;
                return STATE_READING_WORD;
            } else if (event == EVENT_SPACE) {
                return STATE_START;
            } else if (event == EVENT_EOF) {
                return STATE_END;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_READING_NUMBER:
            if (event == EVENT_DIGIT) {
                return STATE_READING_NUMBER;
            } else if (event == EVENT_SPACE) {
                return STATE_START;
            } else if (event == EVENT_EOF) {
                return STATE_END;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_READING_WORD:
            if (event == EVENT_LETTER) {
                return STATE_READING_WORD;
            } else if (event == EVENT_SPACE) {
                return STATE_START;
            } else if (event == EVENT_EOF) {
                return STATE_END;
            } else {
                return STATE_ERROR;
            }
            
        case STATE_ERROR:
            if (event == EVENT_SPACE) {
                return STATE_START;
            } else if (event == EVENT_EOF) {
                return STATE_END;
            } else {
                return STATE_ERROR;
            }
            
        default:
            return STATE_ERROR;
    }
}

int process_input(const char *input, Counter *counter) {
    State current_state = STATE_START;
    size_t len = strlen(input);
    
    if (len >= MAX_INPUT_LEN) {
        return 0;
    }
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        Event event = get_event(c);
        current_state = transition(current_state, event, counter);
        
        if (current_state == STATE_ERROR) {
            return 0;
        }
        
        if (current_state == STATE_END) {
            break;
        }
    }
    
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    Counter counter = {0, 0};
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (!process_input(input, &counter)) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    printf("Numbers found: %d\n", counter.number_count);
    printf("Words found: %d\n", counter.word_count);
    
    return 0;
}