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

State transition_table[STATE_COUNT][STATE_COUNT] = {
    {STATE_START, STATE_READING, STATE_START, STATE_START, STATE_START},
    {STATE_READING, STATE_READING, STATE_PROCESSING, STATE_READING, STATE_START},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_FINALIZING, STATE_START},
    {STATE_FINALIZING, STATE_FINALIZING, STATE_FINALIZING, STATE_FINALIZING, STATE_DONE},
    {STATE_DONE, STATE_DONE, STATE_DONE, STATE_DONE, STATE_DONE}
};

Event get_next_event(State current_state, const char *input_buffer, size_t *input_len) {
    static size_t pos = 0;
    Event event = EVENT_NONE;

    switch (current_state) {
        case STATE_START:
            if (input_buffer != NULL && *input_len > 0) {
                event = EVENT_INPUT_RECEIVED;
            }
            break;
        case STATE_READING:
            if (pos < *input_len) {
                event = EVENT_PROCESS_COMPLETE;
            }
            break;
        case STATE_PROCESSING:
            if (pos >= *input_len) {
                event = EVENT_FINALIZE_COMPLETE;
            }
            break;
        case STATE_FINALIZING:
            event = EVENT_NONE;
            break;
        case STATE_DONE:
            event = EVENT_NONE;
            break;
    }

    return event;
}

void process_character(char *ch) {
    if (ch == NULL) return;
    
    if (*ch >= 'a' && *ch <= 'z') {
        *ch = *ch - 'a' + 'A';
    } else if (*ch >= 'A' && *ch <= 'Z') {
        *ch = *ch - 'A' + 'a';
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN];
    State current_state = STATE_START;
    size_t input_len = 0;
    size_t pos = 0;
    
    printf("Enter text to process: ");
    
    if (fgets(input_buffer, MAX_INPUT_LEN, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char *newline_ptr = strchr(input_buffer, '\n');
    if (newline_ptr != NULL) {
        *newline_ptr = '\0';
    }
    
    input_len = strlen(input_buffer);
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    while (current_state != STATE_DONE) {
        Event event = get_next_event(current_state, input_buffer, &input_len);
        
        if (event == EVENT_ERROR) {
            fprintf(stderr, "State machine error\n");
            return EXIT_FAILURE;
        }
        
        State next_state = transition_table[current_state][event];
        
        switch (current_state) {
            case STATE_START:
                if (event == EVENT_INPUT_RECEIVED) {
                    printf("Starting processing...\n");
                }
                break;
                
            case STATE_READING:
                if (event == EVENT_PROCESS_COMPLETE) {
                    current_state = STATE_PROCESSING;
                    continue;
                }
                break;
                
            case STATE_PROCESSING:
                if (pos < input_len) {
                    char *current_char = input_buffer + pos;
                    process_character(current_char);
                    pos++;
                    
                    if (pos >= input_len) {
                        event = EVENT_FINALIZE_COMPLETE;
                    } else {
                        event = EVENT_PROCESS_COMPLETE;
                    }
                }
                break;
                
            case STATE_FINALIZING:
                printf("Processed result: %s\n", input_buffer);
                current_state = STATE_DONE;
                continue;
                
            case STATE_DONE:
                break;
        }
        
        current_state = next_state;
    }
    
    return EXIT_SUCCESS;
}