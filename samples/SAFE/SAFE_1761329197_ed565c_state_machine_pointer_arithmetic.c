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

State process_state(State current, Event event, char *input, size_t *pos) {
    switch (current) {
        case STATE_START:
            if (event == EVENT_INPUT_RECEIVED) return STATE_READING;
            break;
        case STATE_READING:
            if (event == EVENT_PROCESS_COMPLETE) return STATE_PROCESSING;
            break;
        case STATE_PROCESSING:
            if (event == EVENT_OUTPUT_DONE) return STATE_OUTPUT;
            break;
        case STATE_OUTPUT:
            if (event == EVENT_EXIT) return STATE_END;
            if (event == EVENT_INPUT_RECEIVED) return STATE_READING;
            break;
        default:
            break;
    }
    return current;
}

void process_input(char *input, size_t len) {
    if (input == NULL || len == 0) return;
    
    char *ptr = input;
    char *end = input + len;
    
    while (ptr < end) {
        if (*ptr >= 'a' && *ptr <= 'z') {
            *ptr = *ptr - 'a' + 'A';
        }
        ptr++;
    }
}

int main(void) {
    State current_state = STATE_START;
    char input_buffer[MAX_INPUT_LEN];
    size_t input_length = 0;
    
    printf("State Machine Processor - Type 'quit' to exit\n");
    
    while (current_state != STATE_END) {
        Event current_event = EVENT_NONE;
        
        switch (current_state) {
            case STATE_START:
                printf("Enter text: ");
                current_event = EVENT_INPUT_RECEIVED;
                break;
                
            case STATE_READING:
                if (fgets(input_buffer, MAX_INPUT_LEN, stdin) != NULL) {
                    char *newline = strchr(input_buffer, '\n');
                    if (newline != NULL) *newline = '\0';
                    
                    input_length = strlen(input_buffer);
                    
                    if (strcmp(input_buffer, "quit") == 0) {
                        current_event = EVENT_EXIT;
                    } else {
                        current_event = EVENT_PROCESS_COMPLETE;
                    }
                } else {
                    current_event = EVENT_EXIT;
                }
                break;
                
            case STATE_PROCESSING:
                process_input(input_buffer, input_length);
                current_event = EVENT_OUTPUT_DONE;
                break;
                
            case STATE_OUTPUT:
                printf("Processed: %s\n", input_buffer);
                current_event = EVENT_INPUT_RECEIVED;
                break;
                
            default:
                current_event = EVENT_EXIT;
                break;
        }
        
        current_state = process_state(current_state, current_event, input_buffer, &input_length);
    }
    
    printf("Goodbye!\n");
    return 0;
}