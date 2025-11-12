//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 50

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_END,
    EVENT_INVALID,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    Event event;
    State next_state;
} Transition;

static const Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING},
    {STATE_READING, EVENT_DATA, STATE_READING},
    {STATE_READING, EVENT_END, STATE_PROCESSING},
    {STATE_PROCESSING, EVENT_DATA, STATE_COMPLETE},
    {STATE_COMPLETE, EVENT_RESET, STATE_IDLE},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE},
    {STATE_READING, EVENT_INVALID, STATE_ERROR},
    {STATE_PROCESSING, EVENT_INVALID, STATE_ERROR}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

State get_next_state(State current, Event event) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == current && transitions[i].event == event) {
            return transitions[i].next_state;
        }
    }
    return current;
}

Event parse_input(const char* input) {
    if (input == NULL) {
        return EVENT_INVALID;
    }
    
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) {
        return EVENT_INVALID;
    }
    
    if (strcmp(input, "START") == 0) {
        return EVENT_START;
    } else if (strcmp(input, "END") == 0) {
        return EVENT_END;
    } else if (strcmp(input, "RESET") == 0) {
        return EVENT_RESET;
    } else {
        for (size_t i = 0; i < len; i++) {
            if (!isalnum((unsigned char)input[i]) && input[i] != ' ') {
                return EVENT_INVALID;
            }
        }
        return EVENT_DATA;
    }
}

void print_state_info(State state) {
    switch (state) {
        case STATE_IDLE:
            printf("Current state: IDLE\n");
            break;
        case STATE_READING:
            printf("Current state: READING\n");
            break;
        case STATE_PROCESSING:
            printf("Current state: PROCESSING\n");
            break;
        case STATE_COMPLETE:
            printf("Current state: COMPLETE\n");
            break;
        case STATE_ERROR:
            printf("Current state: ERROR\n");
            break;
    }
}

int main(void) {
    State current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 2];
    
    printf("State Machine Controller\n");
    printf("Available commands: START, END, RESET, or any alphanumeric data\n");
    printf("Type 'QUIT' to exit\n\n");
    
    while (1) {
        print_state_info(current_state);
        printf("Enter command: ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            continue;
        }
        
        if (strcmp(input_buffer, "QUIT") == 0) {
            break;
        }
        
        Event event = parse_input(input_buffer);
        State next_state = get_next_state(current_state, event);
        
        if (next_state != current_state) {
            printf("State transition: ");
            print_state_info(next_state);
            current_state = next_state;
        } else {
            printf("No state transition occurred.\n");
        }
        
        printf("\n");
    }
    
    printf("Exiting state machine controller.\n");
    return 0;
}