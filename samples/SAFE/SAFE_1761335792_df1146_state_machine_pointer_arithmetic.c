//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT 256
#define MAX_STATES 8

typedef enum {
    STATE_START,
    STATE_READING_WORD,
    STATE_READING_NUMBER,
    STATE_PUNCTUATION,
    STATE_END,
    STATE_ERROR
} parser_state_t;

typedef enum {
    EVENT_CHAR,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_PUNCTUATION,
    EVENT_INVALID
} parser_event_t;

static parser_event_t get_event(int c) {
    if (c == EOF) return EVENT_EOF;
    if (isspace(c)) return EVENT_SPACE;
    if (isalnum(c)) return EVENT_CHAR;
    if (ispunct(c)) return EVENT_PUNCTUATION;
    return EVENT_INVALID;
}

static void process_state(parser_state_t *state, parser_event_t event, int *word_count, int *number_count, int *punct_count) {
    static const parser_state_t transition_table[MAX_STATES][5] = {
        {STATE_READING_WORD, STATE_START, STATE_END, STATE_PUNCTUATION, STATE_ERROR},
        {STATE_READING_WORD, STATE_START, STATE_END, STATE_PUNCTUATION, STATE_READING_NUMBER},
        {STATE_READING_NUMBER, STATE_START, STATE_END, STATE_PUNCTUATION, STATE_ERROR},
        {STATE_PUNCTUATION, STATE_START, STATE_END, STATE_PUNCTUATION, STATE_ERROR},
        {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END},
        {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
    };

    if (*state >= MAX_STATES || event >= 5) {
        *state = STATE_ERROR;
        return;
    }

    parser_state_t new_state = transition_table[*state][event];
    
    if (*state == STATE_READING_WORD && new_state == STATE_START) {
        if (*word_count < INT_MAX) (*word_count)++;
    } else if (*state == STATE_READING_NUMBER && new_state == STATE_START) {
        if (*number_count < INT_MAX) (*number_count)++;
    } else if (*state == STATE_PUNCTUATION && new_state == STATE_START) {
        if (*punct_count < INT_MAX) (*punct_count)++;
    }
    
    *state = new_state;
}

int main(void) {
    char input_buffer[MAX_INPUT + 1];
    parser_state_t current_state = STATE_START;
    int word_count = 0;
    int number_count = 0;
    int punct_count = 0;
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return EXIT_SUCCESS;
    }
    
    if (input_len >= sizeof(input_buffer)) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    char *current_char = input_buffer;
    char *end_ptr = input_buffer + input_len;
    
    while (current_char < end_ptr && current_state != STATE_END && current_state != STATE_ERROR) {
        int c = *current_char;
        parser_event_t event = get_event(c);
        
        if (isdigit(c) && current_state == STATE_READING_WORD) {
            event = EVENT_INVALID;
        } else if (isalpha(c) && current_state == STATE_READING_NUMBER) {
            event = EVENT_INVALID;
        }
        
        process_state(&current_state, event, &word_count, &number_count, &punct_count);
        current_char++;
    }
    
    if (current_state != STATE_ERROR) {
        process_state(&current_state, EVENT_EOF, &word_count, &number_count, &punct_count);
    }
    
    if (current_state == STATE_ERROR) {
        printf("Invalid input format\n");
        return EXIT_FAILURE;
    }
    
    printf("Analysis results:\n");
    printf("Words: %d\n", word_count);
    printf("Numbers: %d\n", number_count);
    printf("Punctuation marks: %d\n", punct_count);
    
    return EXIT_SUCCESS;
}