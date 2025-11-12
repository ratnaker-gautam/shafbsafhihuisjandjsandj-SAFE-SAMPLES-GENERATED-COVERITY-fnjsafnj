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
    int numbers;
    int words;
    int errors;
} Counter;

Event get_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State transition(State current, Event event, Counter *counter) {
    static int in_word = 0;
    static int in_number = 0;
    
    switch (current) {
        case STATE_START:
            switch (event) {
                case EVENT_DIGIT:
                    in_number = 1;
                    return STATE_READING_NUMBER;
                case EVENT_LETTER:
                    in_word = 1;
                    return STATE_READING_WORD;
                case EVENT_SPACE:
                case EVENT_EOF:
                    return STATE_START;
                default:
                    counter->errors++;
                    return STATE_ERROR;
            }
            
        case STATE_READING_NUMBER:
            switch (event) {
                case EVENT_DIGIT:
                    return STATE_READING_NUMBER;
                case EVENT_SPACE:
                    if (in_number) {
                        counter->numbers++;
                        in_number = 0;
                    }
                    return STATE_START;
                case EVENT_EOF:
                    if (in_number) {
                        counter->numbers++;
                        in_number = 0;
                    }
                    return STATE_END;
                default:
                    counter->errors++;
                    in_number = 0;
                    return STATE_ERROR;
            }
            
        case STATE_READING_WORD:
            switch (event) {
                case EVENT_LETTER:
                    return STATE_READING_WORD;
                case EVENT_SPACE:
                    if (in_word) {
                        counter->words++;
                        in_word = 0;
                    }
                    return STATE_START;
                case EVENT_EOF:
                    if (in_word) {
                        counter->words++;
                        in_word = 0;
                    }
                    return STATE_END;
                default:
                    counter->errors++;
                    in_word = 0;
                    return STATE_ERROR;
            }
            
        case STATE_ERROR:
            switch (event) {
                case EVENT_SPACE:
                case EVENT_EOF:
                    return STATE_START;
                default:
                    return STATE_ERROR;
            }
            
        default:
            return STATE_ERROR;
    }
}

int process_input(const char *input, Counter *counter) {
    if (input == NULL || counter == NULL) {
        return -1;
    }
    
    size_t len = strlen(input);
    if (len >= MAX_INPUT_LEN) {
        return -1;
    }
    
    State current_state = STATE_START;
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        Event event = get_event(c);
        current_state = transition(current_state, event, counter);
        
        if (current_state == STATE_END) {
            break;
        }
    }
    
    return 0;
}

void reset_counter(Counter *counter) {
    if (counter != NULL) {
        counter->numbers = 0;
        counter->words = 0;
        counter->errors = 0;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    Counter counter = {0, 0, 0};
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (process_input(input, &counter) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    printf("Analysis results:\n");
    printf("Numbers: %d\n", counter.numbers);
    printf("Words: %d\n", counter.words);
    printf("Errors: %d\n", counter.errors);
    
    return 0;
}