//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    EVENT_OTHER
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
    return EVENT_OTHER;
}

static state_t handle_start(event_t event, counter_t *counters) {
    switch (event) {
        case EVENT_DIGIT:
            counters->number_count++;
            return STATE_READING_NUMBER;
        case EVENT_LETTER:
            counters->word_count++;
            return STATE_READING_WORD;
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        default:
            counters->error_count++;
            return STATE_ERROR;
    }
}

static state_t handle_reading_number(event_t event, counter_t *counters) {
    switch (event) {
        case EVENT_DIGIT:
            return STATE_READING_NUMBER;
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        case EVENT_LETTER:
        case EVENT_OTHER:
            counters->error_count++;
            return STATE_ERROR;
    }
    return STATE_ERROR;
}

static state_t handle_reading_word(event_t event, counter_t *counters) {
    switch (event) {
        case EVENT_LETTER:
            return STATE_READING_WORD;
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        case EVENT_DIGIT:
        case EVENT_OTHER:
            counters->error_count++;
            return STATE_ERROR;
    }
    return STATE_ERROR;
}

static state_t handle_error(event_t event, counter_t *counters) {
    switch (event) {
        case EVENT_SPACE:
        case EVENT_EOF:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

static state_t process_event(state_t current_state, event_t event, counter_t *counters) {
    switch (current_state) {
        case STATE_START:
            return handle_start(event, counters);
        case STATE_READING_NUMBER:
            return handle_reading_number(event, counters);
        case STATE_READING_WORD:
            return handle_reading_word(event, counters);
        case STATE_ERROR:
            return handle_error(event, counters);
        case STATE_END:
            return STATE_END;
    }
    return STATE_ERROR;
}

static int read_input_safely(char *buffer, size_t size) {
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
    counter_t counters = {0, 0, 0};
    state_t current_state = STATE_START;
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT_LEN);
    
    if (read_input_safely(input, sizeof(input)) != 0) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i <= input_len; i++) {
        char current_char = input[i];
        event_t event = get_event(current_char);
        
        current_state = process_event(current_state, event, &counters);
        
        if (current_state == STATE_END) {
            break;
        }
    }
    
    printf("Analysis results:\n");
    printf("Numbers found: %d\n", counters.number_count);
    printf("Words found: %d\n", counters.word_count);
    printf("Errors detected: %d\n", counters.error_count);
    
    return EXIT_SUCCESS;
}