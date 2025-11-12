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
    if (input == NULL || pos == NULL) return EVENT_ERROR;
    
    const char *current = input + *pos;
    if (*current == '\0') return EVENT_NONE;
    
    if (*current == 'P') return EVENT_PROCESS_COMPLETE;
    if (*current == 'F') return EVENT_FINALIZE_COMPLETE;
    if (*current == 'E') return EVENT_ERROR;
    
    (*pos)++;
    return EVENT_INPUT_RECEIVED;
}

void process_state_action(State current, char *buffer, size_t *buf_pos, char input_char) {
    if (buffer == NULL || buf_pos == NULL) return;
    
    char *buf_ptr = buffer + *buf_pos;
    
    switch (current) {
        case STATE_READING:
            if (*buf_pos < MAX_INPUT_LEN - 1 && input_char != '\0') {
                *buf_ptr = input_char;
                (*buf_pos)++;
            }
            break;
        case STATE_PROCESSING:
            if (*buf_pos > 0) {
                char *start = buffer;
                char *end = buffer + (*buf_pos - 1);
                while (start < end) {
                    char temp = *start;
                    *start = *end;
                    *end = temp;
                    start++;
                    end--;
                }
            }
            break;
        case STATE_FINALIZING:
            if (*buf_pos < MAX_INPUT_LEN - 1) {
                *buf_ptr = '!';
                (*buf_pos)++;
            }
            break;
        default:
            break;
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN];
    char output_buffer[MAX_INPUT_LEN];
    
    printf("Enter input sequence (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    State current_state = STATE_START;
    size_t input_pos = 0;
    size_t output_pos = 0;
    
    memset(output_buffer, 0, sizeof(output_buffer));
    
    while (current_state != STATE_DONE && input_pos < input_len) {
        Event event = get_next_event(input_buffer, &input_pos);
        
        if (event == EVENT_ERROR) {
            fprintf(stderr, "Error event encountered\n");
            return EXIT_FAILURE;
        }
        
        State next_state = transition_table[current_state][event];
        
        if (next_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid state transition\n");
            return EXIT_FAILURE;
        }
        
        if (event == EVENT_INPUT_RECEIVED) {
            char current_char = *(input_buffer + input_pos - 1);
            process_state_action(current_state, output_buffer, &output_pos, current_char);
        } else {
            process_state_action(current_state, output_buffer, &output_pos, '\0');
        }
        
        current_state = next_state;
    }
    
    if (output_pos < sizeof(output_buffer)) {
        *(output_buffer + output_pos) = '\0';
    }
    
    printf("Result: %s\n", output_buffer);
    
    return EXIT_SUCCESS;
}