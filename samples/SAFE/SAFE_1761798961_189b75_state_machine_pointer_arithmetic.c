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
    {STATE_START, STATE_READING, STATE_START, STATE_START, STATE_START},
    {STATE_READING, STATE_READING, STATE_PROCESSING, STATE_READING, STATE_READING},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_OUTPUT, STATE_PROCESSING},
    {STATE_OUTPUT, STATE_OUTPUT, STATE_OUTPUT, STATE_OUTPUT, STATE_END},
    {STATE_END, STATE_END, STATE_END, STATE_END, STATE_END}
};

Event get_next_event(State current_state, const char *input_buffer, size_t *position) {
    static size_t input_len = 0;
    static const char *current_input = NULL;
    
    if (current_input == NULL && input_buffer != NULL) {
        current_input = input_buffer;
        input_len = strlen(input_buffer);
        *position = 0;
    }
    
    switch (current_state) {
        case STATE_START:
            if (current_input != NULL && input_len > 0) {
                return EVENT_INPUT_RECEIVED;
            }
            break;
        case STATE_READING:
            if (*position >= input_len) {
                return EVENT_PROCESS_COMPLETE;
            }
            break;
        case STATE_PROCESSING:
            return EVENT_OUTPUT_DONE;
        case STATE_OUTPUT:
            if (*position >= input_len) {
                return EVENT_EXIT;
            }
            break;
        default:
            break;
    }
    return EVENT_NONE;
}

void process_character(char c, char *output_buffer, size_t *output_pos) {
    if (*output_pos >= MAX_INPUT_LEN - 1) {
        return;
    }
    
    if (c >= 'a' && c <= 'z') {
        *(output_buffer + *output_pos) = c - 32;
    } else if (c >= 'A' && c <= 'Z') {
        *(output_buffer + *output_pos) = c + 32;
    } else {
        *(output_buffer + *output_pos) = c;
    }
    (*output_pos)++;
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1] = {0};
    char output_buffer[MAX_INPUT_LEN + 1] = {0};
    State current_state = STATE_START;
    size_t input_pos = 0;
    size_t output_pos = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && *(input_buffer + input_len - 1) == '\n') {
        *(input_buffer + input_len - 1) = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided.\n");
        return EXIT_FAILURE;
    }
    
    while (current_state != STATE_END) {
        Event event = get_next_event(current_state, input_buffer, &input_pos);
        
        switch (current_state) {
            case STATE_START:
                if (event == EVENT_INPUT_RECEIVED) {
                    current_state = STATE_READING;
                }
                break;
                
            case STATE_READING:
                if (event == EVENT_PROCESS_COMPLETE) {
                    current_state = STATE_PROCESSING;
                } else {
                    process_character(*(input_buffer + input_pos), output_buffer, &output_pos);
                    input_pos++;
                }
                break;
                
            case STATE_PROCESSING:
                if (event == EVENT_OUTPUT_DONE) {
                    current_state = STATE_OUTPUT;
                }
                break;
                
            case STATE_OUTPUT:
                if (output_pos > 0) {
                    *(output_buffer + output_pos) = '\0';
                    printf("Processed output: %s\n", output_buffer);
                }
                if (event == EVENT_EXIT) {
                    current_state = STATE_END;
                }
                break;
                
            default:
                break;
        }
    }
    
    return EXIT_SUCCESS;
}