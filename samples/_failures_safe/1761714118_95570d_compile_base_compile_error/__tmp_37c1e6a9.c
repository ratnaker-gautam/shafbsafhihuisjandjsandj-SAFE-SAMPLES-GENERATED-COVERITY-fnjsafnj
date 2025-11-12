//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LENGTH 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 20

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_WRITE,
    EVENT_ERROR,
    EVENT_COMPLETE
} Event;

typedef struct {
    State current_state;
    Event event;
    State next_state;
} Transition;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LENGTH];
    size_t buffer_length;
    int error_code;
} StateMachine;

const Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING},
    {STATE_READING, EVENT_DATA, STATE_PROCESSING},
    {STATE_READING, EVENT_ERROR, STATE_ERROR},
    {STATE_PROCESSING, EVENT_PROCESS, STATE_WRITING},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR},
    {STATE_WRITING, EVENT_WRITE, STATE_DONE},
    {STATE_WRITING, EVENT_ERROR, STATE_ERROR},
    {STATE_ERROR, EVENT_COMPLETE, STATE_IDLE},
    {STATE_DONE, EVENT_COMPLETE, STATE_IDLE}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void initialize_state_machine(StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->buffer_length = 0;
    sm->error_code = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

State get_next_state(State current_state, Event event) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == current_state && 
            transitions[i].event == event) {
            return transitions[i].next_state;
        }
    }
    return current_state;
}

int validate_input(const char* input, size_t length) {
    if (input == NULL) return 0;
    if (length == 0 || length >= MAX_INPUT_LENGTH) return 0;
    for (size_t i = 0; i < length; i++) {
        if (input[i] == '\0') return 0;
    }
    return 1;
}

int read_input(StateMachine* sm) {
    if (sm == NULL) return 0;
    
    printf("Enter data (max %d characters): ", MAX_INPUT_LENGTH - 1);
    
    char input[MAX_INPUT_LENGTH];
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input, len)) {
        return 0;
    }
    
    if (len < sizeof(sm->buffer)) {
        memcpy(sm->buffer, input, len);
        sm->buffer_length = len;
        sm->buffer[len] = '\0';
        return 1;
    }
    
    return 0;
}

void process_data(StateMachine* sm) {
    if (sm == NULL || sm->buffer_length == 0) return;
    
    for (size_t i = 0; i < sm->buffer_length; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
}

void write_output(const StateMachine* sm) {
    if (sm == NULL || sm->buffer_length == 0) return;
    printf("Processed output: %s\n", sm->buffer);
}

void handle_state_machine(StateMachine* sm) {
    if (sm == NULL) return;
    
    while (sm->current_state != STATE_DONE && sm->current_state != STATE_ERROR) {
        switch (sm->current_state) {
            case STATE_IDLE:
                printf("State: IDLE\n");
                sm->current_state = get_next_state(sm->current_state, EVENT_START);
                break;
                
            case STATE_READING:
                printf("State: READING\n");
                if (read_input(sm)) {
                    sm->current_state = get_next_state(sm->current_state, EVENT_DATA);
                } else {
                    sm->current_state = get_next_state(sm->current_state, EVENT_ERROR);
                    sm->error_code = 1;
                }
                break;
                
            case STATE_PROCESSING:
                printf("State: PROCESSING\n");
                process_data(sm);
                sm->current_state = get_next_state(sm->current_state, EVENT_PROCESS);
                break;
                
            case STATE_WRITING:
                printf("State: WRITING\n");
                write_output(sm);
                sm->current_state = get_next_state(sm->current_state, EVENT_WRITE);
                break;
                
            default:
                sm->current_state = STATE_ERROR;
                break;
        }
    }
    
    if (sm->current_state == STATE_ERROR) {
        printf("State: ERROR (code: %d)\n", sm->