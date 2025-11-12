//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_INPUT_LEN 100
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINALIZING,
    STATE_DONE
} State;

typedef enum {
    EVENT_NEW_CHAR,
    EVENT_SPACE,
    EVENT_EOF,
    EVENT_ERROR
} Event;

static Event get_next_event(char c) {
    if (c == '\0' || c == EOF) return EVENT_EOF;
    if (c == ' ') return EVENT_SPACE;
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return EVENT_NEW_CHAR;
    return EVENT_ERROR;
}

static State transition(State current, Event event, uint32_t *word_count) {
    switch (current) {
        case STATE_START:
            if (event == EVENT_NEW_CHAR) {
                *word_count += 1;
                return STATE_READING;
            }
            return (event == EVENT_EOF) ? STATE_DONE : STATE_START;
        case STATE_READING:
            if (event == EVENT_SPACE) return STATE_PROCESSING;
            if (event == EVENT_EOF) return STATE_FINALIZING;
            return (event == EVENT_ERROR) ? STATE_START : STATE_READING;
        case STATE_PROCESSING:
            if (event == EVENT_NEW_CHAR) {
                *word_count += 1;
                return STATE_READING;
            }
            return (event == EVENT_EOF) ? STATE_FINALIZING : STATE_PROCESSING;
        case STATE_FINALIZING:
            return STATE_DONE;
        default:
            return STATE_START;
    }
}

static void process_input(const char *input) {
    State current = STATE_START;
    uint32_t word_count = 0;
    size_t len = strnlen(input, MAX_INPUT_LEN);
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        Event event = get_next_event(c);
        State next = transition(current, event, &word_count);
        current = next;
        
        if (current == STATE_DONE) break;
    }
    
    if (current != STATE_DONE) {
        transition(current, EVENT_EOF, &word_count);
    }
    
    printf("Word count: %u\n", word_count);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strnlen(input, MAX_INPUT_LEN + 1) > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    process_input(input);
    return EXIT_SUCCESS;
}