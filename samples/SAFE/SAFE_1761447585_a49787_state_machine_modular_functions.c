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
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

State handle_start(Event ev, Statistics* stats) {
    if (stats == NULL) return STATE_ERROR;
    
    switch (ev) {
        case EVENT_DIGIT:
            stats->number_count++;
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            stats->word_count++;
            return STATE_READING_WORD;
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        default:
            stats->error_count++;
            return STATE_ERROR;
    }
}

State handle_reading_number(Event ev, Statistics* stats) {
    if (stats == NULL) return STATE_ERROR;
    
    switch (ev) {
        case EVENT_DIGIT:
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        case EVENT_LETTER:
        case EVENT_INVALID:
            stats->error_count++;
            return STATE_ERROR;
        default:
            return STATE_READING_NUMBER;
    }
}

State handle_reading_word(Event ev, Statistics* stats) {
    if (stats == NULL) return STATE_ERROR;
    
    switch (ev) {
        case EVENT_LETTER:
            return STATE_READING_WORD;
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        case EVENT_DIGIT:
        case EVENT_INVALID:
            stats->error_count++;
            return STATE_ERROR;
        default:
            return STATE_READING_WORD;
    }
}

State handle_error(Event ev, Statistics* stats) {
    switch (ev) {
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State process_state_machine(const char* input, Statistics* stats) {
    if (input == NULL || stats == NULL) return STATE_ERROR;
    
    State current_state = STATE_START;
    size_t len = strnlen(input, MAX_INPUT_LEN);
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        Event ev = get_event(c);
        
        switch (current_state) {
            case STATE_START:
                current_state = handle_start(ev, stats);
                break;
            case STATE_READING_NUMBER:
                current_state = handle_reading_number(ev, stats);
                break;
            case STATE_READING_WORD:
                current_state = handle_reading_word(ev, stats);
                break;
            case STATE_ERROR:
                current_state = handle_error(ev, stats);
                break;
            case STATE_END:
                return STATE_END;
        }
    }
    
    return current_state;
}

int get_user_input(char* buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0) {
        return -1;
    }
    
    printf("Enter text to analyze (max %zu characters): ", buffer_size - 1);
    
    if (fgets(buffer, (int)buffer_size, stdin) == NULL) {
        return -1;
    }
    
    size_t len = strnlen(buffer, buffer_size);
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
    char input_buffer[MAX_INPUT_LEN + 1];
    Statistics stats = {0, 0, 0};
    
    if (get_user_input(input_buffer, sizeof(input_buffer)) != 0) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    process_state_machine(input_buffer, &stats);
    print_statistics(&stats);
    
    return EXIT_SUCCESS;
}