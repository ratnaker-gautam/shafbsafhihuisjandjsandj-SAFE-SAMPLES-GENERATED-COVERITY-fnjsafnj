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
    EVENT_SPACE,
    EVENT_ALPHA,
    EVENT_DIGIT,
    EVENT_OTHER,
    EVENT_END
} Event;

State transition_table[STATE_COUNT][4] = {
    {STATE_READING, STATE_READING, STATE_READING, STATE_ERROR},
    {STATE_PROCESSING, STATE_READING, STATE_READING, STATE_ERROR},
    {STATE_READING, STATE_READING, STATE_READING, STATE_ERROR},
    {STATE_FINISHED, STATE_FINISHED, STATE_FINISHED, STATE_FINISHED},
    {STATE_ERROR, STATE_ERROR, STATE_ERROR, STATE_ERROR}
};

Event get_event(char c) {
    if (c == '\0') return EVENT_END;
    if (isspace(c)) return EVENT_SPACE;
    if (isalpha(c)) return EVENT_ALPHA;
    if (isdigit(c)) return EVENT_DIGIT;
    return EVENT_OTHER;
}

int main(void) {
    char input[MAX_INPUT + 1];
    char *ptr = input;
    int word_count = 0;
    State current_state = STATE_START;
    
    printf("Enter text (max %d characters): ", MAX_INPUT);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 0;
    }
    
    while (*ptr != '\0' && current_state != STATE_ERROR && current_state != STATE_FINISHED) {
        Event event = get_event(*ptr);
        int event_index = (int)event;
        
        if (event_index >= 0 && event_index < 4) {
            State next_state = *(*(transition_table + (int)current_state) + event_index);
            
            if (current_state == STATE_START && next_state == STATE_READING) {
                word_count++;
            } else if (current_state == STATE_PROCESSING && next_state == STATE_READING) {
                word_count++;
            }
            
            current_state = next_state;
        } else {
            current_state = STATE_ERROR;
        }
        
        ptr++;
    }
    
    if (current_state == STATE_ERROR) {
        printf("Invalid input format\n");
        return 1;
    }
    
    printf("Word count: %d\n", word_count);
    return 0;
}