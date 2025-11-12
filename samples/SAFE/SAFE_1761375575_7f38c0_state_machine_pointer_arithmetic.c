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
    EVENT_INPUT_RECEIVED,
    EVENT_PROCESS_COMPLETE,
    EVENT_FINALIZE_COMPLETE,
    EVENT_ERROR
} Event;

State transition_table[STATE_COUNT][5] = {
    {STATE_START, STATE_READING, STATE_START, STATE_START, STATE_START},
    {STATE_READING, STATE_READING, STATE_PROCESSING, STATE_READING, STATE_START},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_FINALIZING, STATE_START},
    {STATE_FINALIZING, STATE_FINALIZING, STATE_FINALIZING, STATE_FINALIZING, STATE_DONE},
    {STATE_DONE, STATE_DONE, STATE_DONE, STATE_DONE, STATE_DONE}
};

Event get_next_event(const char *input, size_t *pos) {
    if (*pos >= strlen(input)) {
        return EVENT_FINALIZE_COMPLETE;
    }
    
    char current = *(input + *pos);
    (*pos)++;
    
    if (current == 'P') {
        return EVENT_PROCESS_COMPLETE;
    } else if (current == 'F') {
        return EVENT_FINALIZE_COMPLETE;
    } else if (current == 'E') {
        return EVENT_ERROR;
    } else {
        return EVENT_INPUT_RECEIVED;
    }
}

void process_state_machine(const char *input) {
    State current_state = STATE_START;
    size_t position = 0;
    char buffer[MAX_INPUT_LEN] = {0};
    char *buf_ptr = buffer;
    
    while (current_state != STATE_DONE) {
        Event event = get_next_event(input, &position);
        
        if (event >= 5) {
            event = EVENT_ERROR;
        }
        
        State next_state = *(*(transition_table + current_state) + event);
        
        switch (current_state) {
            case STATE_START:
                if (next_state == STATE_READING) {
                    printf("Starting state machine\n");
                }
                break;
                
            case STATE_READING:
                if (event == EVENT_INPUT_RECEIVED && buf_ptr < buffer + MAX_INPUT_LEN - 1) {
                    *buf_ptr = *(input + position - 1);
                    buf_ptr++;
                    *buf_ptr = '\0';
                }
                break;
                
            case STATE_PROCESSING:
                if (event == EVENT_PROCESS_COMPLETE) {
                    printf("Processing: %s\n", buffer);
                }
                break;
                
            case STATE_FINALIZING:
                if (event == EVENT_FINALIZE_COMPLETE) {
                    printf("Final result: %s\n", buffer);
                }
                break;
                
            default:
                break;
        }
        
        current_state = next_state;
        
        if (event == EVENT_ERROR) {
            printf("Error occurred, resetting\n");
            buf_ptr = buffer;
            *buf_ptr = '\0';
            position = 0;
        }
    }
    
    printf("State machine completed successfully\n");
}

int main() {
    char input[MAX_INPUT_LEN];
    
    printf("Enter input sequence (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && *(input + len - 1) == '\n') {
        *(input + len - 1) = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    process_state_machine(input);
    
    return 0;
}