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

static int is_valid_char(char c) {
    return isdigit(c) || isalpha(c) || c == ' ' || c == '\n' || c == '\t';
}

static event_t get_event(char c) {
    if (c == EOF) return EVENT_EOF;
    if (isdigit(c)) return EVENT_DIGIT;
    if (isalpha(c)) return EVENT_LETTER;
    if (c == ' ' || c == '\n' || c == '\t') return EVENT_SPACE;
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
    return STATE_ERROR;
}

static state_t handle_end(event_t event) {
    return STATE_END;
}

static void process_state_machine(void) {
    state_t current_state = STATE_START;
    int numbers_found = 0;
    int words_found = 0;
    char buffer[MAX_INPUT_LEN + 1];
    size_t buf_pos = 0;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    while (current_state != STATE_END && current_state != STATE_ERROR) {
        int c = getchar();
        
        if (c == EOF && buf_pos < MAX_INPUT_LEN) {
            c = '\n';
        }
        
        if (buf_pos >= MAX_INPUT_LEN) {
            current_state = STATE_ERROR;
            break;
        }
        
        if (!is_valid_char(c) && c != EOF) {
            current_state = STATE_ERROR;
            break;
        }
        
        event_t event = get_event(c);
        state_t next_state;
        
        switch (current_state) {
            case STATE_START: next_state = handle_start(event); break;
            case STATE_READING_NUMBER: next_state = handle_reading_number(event); break;
            case STATE_READING_WORD: next_state = handle_reading_word(event); break;
            case STATE_ERROR: next_state = handle_error(event); break;
            case STATE_END: next_state = handle_end(event); break;
            default: next_state = STATE_ERROR; break;
        }
        
        if (current_state == STATE_READING_NUMBER && next_state == STATE_START) {
            numbers_found++;
        }
        
        if (current_state == STATE_READING_WORD && next_state == STATE_START) {
            words_found++;
        }
        
        if (c != EOF && buf_pos < MAX_INPUT_LEN) {
            buffer[buf_pos++] = (char)c;
        }
        
        current_state = next_state;
    }
    
    if (buf_pos < MAX_INPUT_LEN) {
        buffer[buf_pos] = '\0';
    } else {
        buffer[MAX_INPUT_LEN] = '\0';
    }
    
    if (current_state == STATE_ERROR) {
        printf("Error: Invalid input detected\n");
    } else {
        printf("Processed input: %s\n", buffer);
        printf("Numbers found: %d\n", numbers_found);
        printf("Words found: %d\n", words_found);
    }
}

int main(void) {
    process_state_machine();
    return 0;
}