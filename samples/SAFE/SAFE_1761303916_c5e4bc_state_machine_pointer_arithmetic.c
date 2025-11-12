//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
    EVENT_CHAR,
    EVENT_SPACE,
    EVENT_TERMINATOR,
    EVENT_INVALID
} Event;

Event get_event(char c) {
    if (c == '\0') return EVENT_TERMINATOR;
    if (c == ' ') return EVENT_SPACE;
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) return EVENT_CHAR;
    return EVENT_INVALID;
}

State transition_table[STATE_COUNT][4] = {
    {STATE_READING, STATE_START, STATE_DONE, STATE_START},
    {STATE_READING, STATE_PROCESSING, STATE_FINALIZING, STATE_READING},
    {STATE_READING, STATE_PROCESSING, STATE_FINALIZING, STATE_PROCESSING},
    {STATE_READING, STATE_PROCESSING, STATE_DONE, STATE_FINALIZING},
    {STATE_DONE, STATE_DONE, STATE_DONE, STATE_DONE}
};

void process_word(char *start, char *end) {
    if (start >= end) return;
    
    char *p = start;
    while (p < end) {
        if (*p >= 'a' && *p <= 'z') {
            *p = *p - 'a' + 'A';
        }
        p++;
    }
    
    char temp;
    char *left = start;
    char *right = end - 1;
    while (left < right) {
        temp = *left;
        *left = *right;
        *right = temp;
        left++;
        right--;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    State current_state = STATE_START;
    char *buffer_start = input;
    char *current_pos = input;
    char *word_start = NULL;
    
    while (current_state != STATE_DONE && current_pos < input + MAX_INPUT_LEN) {
        Event evt = get_event(*current_pos);
        State next_state = transition_table[current_state][evt];
        
        switch (current_state) {
            case STATE_START:
                if (evt == EVENT_CHAR) {
                    word_start = current_pos;
                }
                break;
                
            case STATE_READING:
                if (evt == EVENT_SPACE || evt == EVENT_TERMINATOR) {
                    process_word(word_start, current_pos);
                }
                break;
                
            case STATE_PROCESSING:
                if (evt == EVENT_CHAR) {
                    word_start = current_pos;
                }
                break;
                
            case STATE_FINALIZING:
                break;
                
            case STATE_DONE:
                break;
        }
        
        current_state = next_state;
        current_pos++;
    }
    
    if (current_state == STATE_READING || current_state == STATE_PROCESSING) {
        process_word(word_start, current_pos);
    }
    
    printf("Processed: %s\n", input);
    return 0;
}