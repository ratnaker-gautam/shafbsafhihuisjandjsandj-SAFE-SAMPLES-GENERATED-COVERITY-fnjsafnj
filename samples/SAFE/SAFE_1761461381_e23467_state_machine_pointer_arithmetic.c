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
    STATE_DONE
} State;

typedef enum {
    EVENT_NONE,
    EVENT_INPUT_RECEIVED,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR
} Event;

State transition_table[STATE_COUNT][5] = {
    {STATE_START, STATE_READING, STATE_START, STATE_START, STATE_START},
    {STATE_READING, STATE_READING, STATE_PROCESSING, STATE_READING, STATE_DONE},
    {STATE_PROCESSING, STATE_PROCESSING, STATE_PROCESSING, STATE_WRITING, STATE_DONE},
    {STATE_WRITING, STATE_WRITING, STATE_WRITING, STATE_WRITING, STATE_DONE},
    {STATE_DONE, STATE_DONE, STATE_DONE, STATE_DONE, STATE_DONE}
};

Event get_next_event(State current_state, char *input, size_t *pos) {
    char *current = input + *pos;
    
    if (current >= input + INPUT_SIZE) {
        return EVENT_ERROR;
    }
    
    if (*current == '\0') {
        return EVENT_ERROR;
    }
    
    switch (current_state) {
        case STATE_START:
            if (*current != '\0') {
                return EVENT_INPUT_RECEIVED;
            }
            break;
        case STATE_READING:
            if (*current == 'P') {
                return EVENT_PROCESS_COMPLETE;
            }
            break;
        case STATE_PROCESSING:
            if (*current == 'W') {
                return EVENT_WRITE_COMPLETE;
            }
            break;
        case STATE_WRITING:
            if (*current == 'D') {
                return EVENT_ERROR;
            }
            break;
        default:
            break;
    }
    
    return EVENT_NONE;
}

void process_state_action(State state, char *input, size_t *pos) {
    char *current = input + *pos;
    
    switch (state) {
        case STATE_READING:
            if (current < input + INPUT_SIZE && *current != '\0') {
                putchar(*current);
                (*pos)++;
            }
            break;
        case STATE_PROCESSING:
            if (current < input + INPUT_SIZE && *current != '\0') {
                putchar(*current - 32);
                (*pos)++;
            }
            break;
        case STATE_WRITING:
            if (current < input + INPUT_SIZE && *current != '\0') {
                putchar(*current);
                putchar(' ');
                (*pos)++;
            }
            break;
        default:
            break;
    }
}

int main(void) {
    char input_buffer[INPUT_SIZE];
    State current_state = STATE_START;
    size_t position = 0;
    
    printf("Enter input sequence: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (input_buffer[0] == '\0') {
        printf("Empty input\n");
        return EXIT_FAILURE;
    }
    
    printf("Processing: ");
    
    while (current_state != STATE_DONE) {
        Event event = get_next_event(current_state, input_buffer, &position);
        
        if (event == EVENT_ERROR) {
            break;
        }
        
        State next_state = transition_table[current_state][event];
        
        if (next_state >= STATE_COUNT) {
            break;
        }
        
        process_state_action(current_state, input_buffer, &position);
        current_state = next_state;
        
        if (position >= INPUT_SIZE) {
            break;
        }
        
        if (*(input_buffer + position) == '\0') {
            break;
        }
    }
    
    putchar('\n');
    return EXIT_SUCCESS;
}