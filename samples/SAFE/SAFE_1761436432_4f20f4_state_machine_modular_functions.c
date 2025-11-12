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
    int error_count;
} Counter;

Event get_event(char c) {
    if (c == '\0' || c == EOF) return EVENT_EOF;
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
    return EVENT_OTHER;
}

State transition(State current, Event event, Counter *counter) {
    switch (current) {
        case STATE_START:
            switch (event) {
                case EVENT_DIGIT:
                    if (counter != NULL) counter->number_count++;
                    return STATE_READING_NUMBER;
                case EVENT_LETTER:
                    if (counter != NULL) counter->word_count++;
                    return STATE_READING_WORD;
                case EVENT_SPACE:
                    return STATE_START;
                case EVENT_EOF:
                    return STATE_END;
                default:
                    if (counter != NULL) counter->error_count++;
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
                    if (counter != NULL) counter->error_count++;
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
                    if (counter != NULL) counter->error_count++;
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
    
    State current_state = STATE_START;
    size_t len = strnlen(input, MAX_INPUT_LEN);
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        Event event = get_event(c);
        current_state = transition(current_state, event, counter);
        
        if (current_state == STATE_END) break;
    }
    
    return 0;
}

int get_user_input(char *buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0) return -1;
    
    printf("Enter text to analyze (max %zu characters): ", buffer_size - 1);
    
    if (fgets(buffer, (int)buffer_size, stdin) == NULL) return -1;
    
    size_t len = strnlen(buffer, buffer_size);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 0;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    Counter counter = {0, 0, 0};
    
    if (get_user_input(input_buffer, sizeof(input_buffer)) != 0) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    if (process_input(input_buffer, &counter) != 0) {
        fprintf(stderr, "Error processing input\n");
        return EXIT_FAILURE;
    }
    
    printf("Analysis results:\n");
    printf("Numbers found: %d\n", counter.number_count);
    printf("Words found: %d\n", counter.word_count);
    printf("Errors detected: %d\n", counter.error_count);
    
    return EXIT_SUCCESS;
}