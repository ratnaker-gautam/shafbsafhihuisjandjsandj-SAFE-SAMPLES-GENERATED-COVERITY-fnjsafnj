//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define STATE_COUNT 5
#define INPUT_SIZE 256

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_END
} State;

typedef enum {
    EVENT_NONE,
    EVENT_INPUT_RECEIVED,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_EXIT
} Event;

State transition_table[STATE_COUNT][STATE_COUNT] = {
    {STATE_START, STATE_READING, STATE_START, STATE_START, STATE_END},
    {STATE_READING, STATE_READING, STATE_PROCESSING, STATE_READING, STATE_END},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_WRITING, STATE_END},
    {STATE_WRITING, STATE_WRITING, STATE_WRITING, STATE_WRITING, STATE_END},
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}
};

Event get_next_event(State current_state) {
    static char input_buffer[INPUT_SIZE];
    static int input_index = 0;
    
    if (current_state == STATE_START) {
        return EVENT_INPUT_RECEIVED;
    }
    
    if (current_state == STATE_READING) {
        if (input_index >= INPUT_SIZE - 1) {
            return EVENT_PROCESS_COMPLETE;
        }
        
        int c = getchar();
        if (c == EOF || c == '\n') {
            *(input_buffer + input_index) = '\0';
            return EVENT_PROCESS_COMPLETE;
        }
        
        if (input_index < INPUT_SIZE - 1) {
            *(input_buffer + input_index) = (char)c;
            input_index++;
        }
        return EVENT_NONE;
    }
    
    if (current_state == STATE_PROCESSING) {
        char *ptr = input_buffer;
        while (*ptr != '\0') {
            if (*ptr >= 'a' && *ptr <= 'z') {
                *ptr = *ptr - 'a' + 'A';
            }
            ptr++;
        }
        return EVENT_WRITE_COMPLETE;
    }
    
    if (current_state == STATE_WRITING) {
        char *ptr = input_buffer;
        while (*ptr != '\0') {
            putchar(*ptr);
            ptr++;
        }
        putchar('\n');
        return EVENT_EXIT;
    }
    
    return EVENT_EXIT;
}

int main(void) {
    State current_state = STATE_START;
    int step_count = 0;
    const int max_steps = 1000;
    
    while (current_state != STATE_END && step_count < max_steps) {
        Event event = get_next_event(current_state);
        
        if (event != EVENT_NONE) {
            State *state_ptr = (State *)transition_table + (current_state * STATE_COUNT);
            State next_state = *(state_ptr + event);
            
            if (next_state >= STATE_START && next_state < STATE_COUNT) {
                current_state = next_state;
            } else {
                current_state = STATE_END;
            }
        }
        
        step_count++;
    }
    
    return 0;
}