//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 256
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINALIZING,
    STATE_DONE
} State;

typedef enum {
    EVENT_NONE,
    EVENT_CHAR,
    EVENT_SPACE,
    EVENT_TERMINATOR
} Event;

Event get_event(char c) {
    if (c == '\0') return EVENT_TERMINATOR;
    if (c == ' ' || c == '\t' || c == '\n') return EVENT_SPACE;
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return EVENT_CHAR;
    return EVENT_NONE;
}

State transition(State current, Event event, int *word_count) {
    static const State transitions[STATE_COUNT][4] = {
        {STATE_START, STATE_READING, STATE_START, STATE_DONE},
        {STATE_READING, STATE_READING, STATE_PROCESSING, STATE_FINALIZING},
        {STATE_PROCESSING, STATE_READING, STATE_PROCESSING, STATE_FINALIZING},
        {STATE_FINALIZING, STATE_READING, STATE_FINALIZING, STATE_DONE},
        {STATE_DONE, STATE_DONE, STATE_DONE, STATE_DONE}
    };
    
    if (current < STATE_COUNT && event < 4) {
        State next = *(*(transitions + current) + event);
        if (current == STATE_PROCESSING && next == STATE_READING) {
            if (word_count) (*word_count)++;
        }
        return next;
    }
    return STATE_DONE;
}

int process_input(const char *input) {
    if (!input) return -1;
    
    State current = STATE_START;
    int word_count = 0;
    const char *ptr = input;
    
    while (current != STATE_DONE && *ptr != '\0') {
        Event event = get_event(*ptr);
        current = transition(current, event, &word_count);
        ptr++;
    }
    
    if (current == STATE_PROCESSING || current == STATE_FINALIZING) {
        word_count++;
    }
    
    return word_count;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text: ");
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *newline = strchr(input, '\n');
    if (newline) *newline = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int words = process_input(input);
    if (words < 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    printf("Word count: %d\n", words);
    return 0;
}