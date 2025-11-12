//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: state_machine
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
    STATE_OUTPUT,
    STATE_END
} State;

typedef enum {
    EVENT_NONE,
    EVENT_INPUT_RECEIVED,
    EVENT_PROCESS_COMPLETE,
    EVENT_OUTPUT_DONE,
    EVENT_EXIT
} Event;

State transition_table[STATE_COUNT][STATE_COUNT] = {
    {STATE_START, STATE_READING, STATE_START, STATE_START, STATE_END},
    {STATE_READING, STATE_READING, STATE_PROCESSING, STATE_READING, STATE_END},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_OUTPUT, STATE_END},
    {STATE_OUTPUT, STATE_OUTPUT, STATE_OUTPUT, STATE_OUTPUT, STATE_END},
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}
};

Event get_next_event(State current_state, const char *input_buffer, size_t *position) {
    static char input[MAX_INPUT_LEN] = {0};
    static size_t input_len = 0;
    
    if (current_state == STATE_START) {
        printf("Enter text to process (max %d chars): ", MAX_INPUT_LEN - 1);
        if (fgets(input, MAX_INPUT_LEN, stdin) != NULL) {
            input_len = strnlen(input, MAX_INPUT_LEN);
            if (input_len > 0 && input[input_len - 1] == '\n') {
                input[input_len - 1] = '\0';
                input_len--;
            }
            *position = 0;
            return EVENT_INPUT_RECEIVED;
        }
        return EVENT_EXIT;
    }
    
    if (current_state == STATE_READING) {
        if (*position < input_len) {
            return EVENT_INPUT_RECEIVED;
        }
        return EVENT_PROCESS_COMPLETE;
    }
    
    if (current_state == STATE_PROCESSING) {
        if (*position < input_len) {
            return EVENT_PROCESS_COMPLETE;
        }
        return EVENT_OUTPUT_DONE;
    }
    
    if (current_state == STATE_OUTPUT) {
        return EVENT_EXIT;
    }
    
    return EVENT_NONE;
}

void process_character(char *ch, size_t pos) {
    if (ch == NULL) return;
    
    if (pos % 2 == 0) {
        if (*ch >= 'a' && *ch <= 'z') {
            *ch = *ch - 'a' + 'A';
        }
    } else {
        if (*ch >= 'A' && *ch <= 'Z') {
            *ch = *ch - 'A' + 'a';
        }
    }
}

int main(void) {
    State current_state = STATE_START;
    char input_buffer[MAX_INPUT_LEN] = {0};
    char output_buffer[MAX_INPUT_LEN] = {0};
    size_t position = 0;
    Event current_event;
    
    while (current_state != STATE_END) {
        current_event = get_next_event(current_state, input_buffer, &position);
        
        if (current_event == EVENT_EXIT) {
            current_state = STATE_END;
            continue;
        }
        
        State next_state = transition_table[current_state][current_event];
        
        if (next_state == STATE_READING && current_state == STATE_START) {
            char *src = input_buffer;
            char *dst = output_buffer;
            size_t len = strnlen(input_buffer, MAX_INPUT_LEN);
            
            if (len > 0 && len < MAX_INPUT_LEN) {
                while (src < input_buffer + len) {
                    *dst = *src;
                    src++;
                    dst++;
                }
                *dst = '\0';
            }
        }
        
        if (next_state == STATE_PROCESSING && current_state == STATE_READING) {
            char *ptr = output_buffer;
            size_t len = strnlen(output_buffer, MAX_INPUT_LEN);
            
            if (len > 0 && len < MAX_INPUT_LEN) {
                while (ptr < output_buffer + len) {
                    process_character(ptr, (size_t)(ptr - output_buffer));
                    ptr++;
                }
            }
        }
        
        if (next_state == STATE_OUTPUT && current_state == STATE_PROCESSING) {
            size_t len = strnlen(output_buffer, MAX_INPUT_LEN);
            if (len > 0 && len < MAX_INPUT_LEN) {
                printf("Processed result: ");
                char *ptr = output_buffer;
                while (ptr < output_buffer + len) {
                    putchar(*ptr);
                    ptr++;
                }
                putchar('\n');
            }
        }
        
        current_state = next_state;
        
        if (current_state == STATE_READING && position < MAX_INPUT_LEN - 1) {
            position++;
        }
    }
    
    return EXIT_SUCCESS;
}