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
} state_t;

typedef enum {
    EVENT_DIGIT,
    EVENT_LETTER,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_INVALID
} event_t;

typedef struct {
    int number_count;
    int word_count;
    int error_count;
} counter_t;

static event_t get_event(char c) {
    if (c == '\0' || c == EOF) return EVENT_EOF;
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isdigit((unsigned char)c)) return EVENT_DIGIT;
    if (isalpha((unsigned char)c)) return EVENT_LETTER;
    return EVENT_INVALID;
}

static state_t handle_start(event_t event) {
    switch (event) {
        case EVENT_DIGIT: return STATE_READING_NUMBER;
        case EVENT_LETTER: return STATE_READING_WORD;
        case EVENT_SPACE: return STATE_START;
        case EVENT_EOF: return STATE_END;
        default: return STATE_ERROR;
    }
}

static state_t handle_reading_number(event_t event) {
    switch (event) {
        case EVENT_DIGIT: return STATE_READING_NUMBER;
        case EVENT_SPACE: return STATE_START;
        case EVENT_EOF: return STATE_END;
        default: return STATE_ERROR;
    }
}

static state_t handle_reading_word(event_t event) {
    switch (event) {
        case EVENT_LETTER: return STATE_READING_WORD;
        case EVENT_SPACE: return STATE_START;
        case EVENT_EOF: return STATE_END;
        default: return STATE_ERROR;
    }
}

static state_t handle_error(event_t event) {
    switch (event) {
        case EVENT_SPACE: return STATE_START;
        case EVENT_EOF: return STATE_END;
        default: return STATE_ERROR;
    }
}

static void process_state_transition(state_t *current_state, event_t event, counter_t *counters) {
    state_t new_state = *current_state;
    
    switch (*current_state) {
        case STATE_START:
            new_state = handle_start(event);
            break;
        case STATE_READING_NUMBER:
            new_state = handle_reading_number(event);
            if (new_state == STATE_START) counters->number_count++;
            break;
        case STATE_READING_WORD:
            new_state = handle_reading_word(event);
            if (new_state == STATE_START) counters->word_count++;
            break;
        case STATE_ERROR:
            new_state = handle_error(event);
            if (new_state == STATE_START) counters->error_count++;
            break;
        case STATE_END:
            break;
    }
    
    *current_state = new_state;
}

static int read_safe_input(char *buffer, size_t size) {
    if (buffer == NULL || size == 0) return -1;
    
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else if (len == size - 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    
    return 0;
}

int main(void) {
    char input[MAX_INPUT_LEN + 2];
    state_t current_state = STATE_START;
    counter_t counters = {0, 0, 0};
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (read_safe_input(input, sizeof(input)) != 0) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    if (strlen(input) > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    size_t i = 0;
    while (input[i] != '\0') {
        event_t event = get_event(input[i]);
        process_state_transition(&current_state, event, &counters);
        i++;
    }
    
    process_state_transition(&current_state, EVENT_EOF, &counters);
    
    printf("Numbers found: %d\n", counters.number_count);
    printf("Words found: %d\n", counters.word_count);
    printf("Errors found: %d\n", counters.error_count);
    
    return EXIT_SUCCESS;
}