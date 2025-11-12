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
} Statistics;

Event get_event(char c) {
    if (c == '\0' || c == '\n') return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State transition(State current, Event event, Statistics* stats) {
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

int process_input(const char* input, Statistics* stats) {
    if (input == NULL || stats == NULL) return -1;
    
    State current_state = STATE_START;
    size_t len = strlen(input);
    
    if (len >= MAX_INPUT_LEN) return -1;
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        Event event = get_event(c);
        current_state = transition(current_state, event, stats);
        
        if (current_state == STATE_END) break;
    }
    
    return 0;
}

int get_user_input(char* buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0) return -1;
    
    printf("Enter text (max %zu chars): ", buffer_size - 1);
    
    if (fgets(buffer, buffer_size, stdin) == NULL) return -1;
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 0;
}

void print_statistics(const Statistics* stats) {
    if (stats == NULL) return;
    
    printf("\nAnalysis Results:\n");
    printf("Numbers found: %d\n", stats->number_count);
    printf("Words found: %d\n", stats->word_count);
    printf("Errors detected: %d\n", stats->error_count);
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    Statistics stats = {0};
    
    if (get_user_input(input_buffer, sizeof(input_buffer)) != 0) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    if (process_input(input_buffer, &stats) != 0) {
        fprintf(stderr, "Error processing input\n");
        return EXIT_FAILURE;
    }
    
    print_statistics(&stats);
    
    return EXIT_SUCCESS;
}