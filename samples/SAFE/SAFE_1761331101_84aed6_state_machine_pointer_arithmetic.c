//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256
#define STATE_COUNT 5

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINISHED,
    STATE_ERROR
} State;

typedef enum {
    EVENT_CHAR,
    EVENT_SPACE,
    EVENT_END,
    EVENT_INVALID
} Event;

State transition_table[STATE_COUNT][4] = {
    {STATE_READING, STATE_START, STATE_FINISHED, STATE_ERROR},
    {STATE_READING, STATE_PROCESSING, STATE_FINISHED, STATE_ERROR},
    {STATE_READING, STATE_PROCESSING, STATE_FINISHED, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
};

Event get_event(char c) {
    if (c == '\0') return EVENT_END;
    if (isspace((unsigned char)c)) return EVENT_SPACE;
    if (isalnum((unsigned char)c)) return EVENT_CHAR;
    return EVENT_INVALID;
}

int main(void) {
    char input[MAX_INPUT];
    char *ptr = input;
    int word_count = 0;
    State current_state = STATE_START;
    
    printf("Enter text (max %d chars): ", MAX_INPUT - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && *(ptr + len - 1) == '\n') {
        *(ptr + len - 1) = '\0';
    }
    
    while (*ptr != '\0' && current_state != STATE_ERROR && current_state != STATE_FINISHED) {
        Event evt = get_event(*ptr);
        
        if (evt == EVENT_INVALID) {
            current_state = STATE_ERROR;
            break;
        }
        
        State next_state = transition_table[current_state][evt];
        
        if (next_state == STATE_PROCESSING) {
            word_count++;
        }
        
        current_state = next_state;
        ptr++;
    }
    
    if (current_state == STATE_ERROR) {
        printf("Invalid input detected\n");
        return 1;
    }
    
    if (current_state == STATE_READING || current_state == STATE_PROCESSING) {
        word_count++;
    }
    
    printf("Word count: %d\n", word_count);
    return 0;
}