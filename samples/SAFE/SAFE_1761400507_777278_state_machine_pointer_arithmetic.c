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

State transition_table[STATE_COUNT][4] = {
    {STATE_START, STATE_READING, STATE_START, STATE_START},
    {STATE_READING, STATE_READING, STATE_PROCESSING, STATE_FINALIZING},
    {STATE_PROCESSING, STATE_READING, STATE_PROCESSING, STATE_FINALIZING},
    {STATE_FINALIZING, STATE_READING, STATE_FINALIZING, STATE_DONE},
    {STATE_DONE, STATE_DONE, STATE_DONE, STATE_DONE}
};

Event get_event(char c) {
    if (c == '\0') return EVENT_TERMINATOR;
    if (c == ' ' || c == '\t' || c == '\n') return EVENT_SPACE;
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return EVENT_CHAR;
    return EVENT_NONE;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char *ptr = input;
    char *end = input + MAX_INPUT_LEN;
    int word_count = 0;
    State current_state = STATE_START;
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    char *null_pos = memchr(input, '\0', sizeof(input));
    if (null_pos == NULL) {
        input[MAX_INPUT_LEN] = '\0';
    }
    
    while (ptr < end && *ptr != '\0') {
        Event evt = get_event(*ptr);
        State next_state = transition_table[current_state][evt];
        
        if (current_state == STATE_READING && next_state == STATE_PROCESSING) {
            word_count++;
        }
        
        current_state = next_state;
        ptr++;
        
        if (current_state == STATE_DONE) {
            break;
        }
    }
    
    if (current_state == STATE_READING || current_state == STATE_FINALIZING) {
        word_count++;
    }
    
    printf("Word count: %d\n", word_count);
    
    return 0;
}