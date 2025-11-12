//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINAL,
    STATE_ERROR
} state_t;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_END,
    EVENT_INVALID
} event_t;

typedef struct {
    state_t current_state;
    event_t event;
    state_t next_state;
} transition_t;

static const transition_t transitions[] = {
    {STATE_START, EVENT_START, STATE_READING},
    {STATE_READING, EVENT_DATA, STATE_PROCESSING},
    {STATE_READING, EVENT_END, STATE_FINAL},
    {STATE_PROCESSING, EVENT_DATA, STATE_PROCESSING},
    {STATE_PROCESSING, EVENT_END, STATE_FINAL},
    {STATE_START, EVENT_INVALID, STATE_ERROR},
    {STATE_READING, EVENT_INVALID, STATE_ERROR},
    {STATE_PROCESSING, EVENT_INVALID, STATE_ERROR},
    {STATE_FINAL, EVENT_INVALID, STATE_ERROR},
    {STATE_ERROR, EVENT_INVALID, STATE_ERROR}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

state_t get_next_state(state_t current, event_t event) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == current && transitions[i].event == event) {
            return transitions[i].next_state;
        }
    }
    return STATE_ERROR;
}

event_t classify_input(const char* input) {
    if (input == NULL) {
        return EVENT_INVALID;
    }
    
    size_t len = strlen(input);
    if (len == 0) {
        return EVENT_INVALID;
    }
    
    if (len == 1 && input[0] == 'S') {
        return EVENT_START;
    }
    
    if (len == 1 && input[0] == 'E') {
        return EVENT_END;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') {
            return EVENT_INVALID;
        }
    }
    
    return EVENT_DATA;
}

void process_state_machine(void) {
    state_t current_state = STATE_START;
    char input_buffer[MAX_INPUT_LEN + 1];
    
    printf("State Machine Controller\n");
    printf("Commands: 'S'=Start, 'E'=End, numbers=data\n");
    printf("Enter commands:\n");
    
    while (current_state != STATE_FINAL && current_state != STATE_ERROR) {
        printf("Current state: %d > ", current_state);
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
        }
        
        if (strlen(input_buffer) >= MAX_INPUT_LEN) {
            printf("Input too long\n");
            current_state = STATE_ERROR;
            continue;
        }
        
        event_t event = classify_input(input_buffer);
        state_t next_state = get_next_state(current_state, event);
        
        printf("Event: %d, Next state: %d\n", event, next_state);
        
        if (next_state == STATE_PROCESSING) {
            long value = strtol(input_buffer, NULL, 10);
            printf("Processing data: %ld\n", value);
        }
        
        current_state = next_state;
    }
    
    if (current_state == STATE_FINAL) {
        printf("State machine completed successfully\n");
    } else {
        printf("State machine terminated with error\n");
    }
}

int main(void) {
    process_state_machine();
    return 0;
}