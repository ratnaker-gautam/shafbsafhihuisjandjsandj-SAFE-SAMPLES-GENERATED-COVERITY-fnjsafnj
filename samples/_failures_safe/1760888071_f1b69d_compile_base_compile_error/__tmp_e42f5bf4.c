//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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

typedef struct {
    State current;
    char input;
    State next;
} Transition;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    uint64_t computation;
    int error_code;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_READING, 'E', STATE_PROCESSING},
    {STATE_READING, '\0', STATE_ERROR},
    {STATE_PROCESSING, '\0', STATE_COMPLETE},
    {STATE_PROCESSING, 'R', STATE_READING},
    {STATE_ERROR, 'R', STATE_IDLE}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

static State get_next_state(State current, char input) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return current;
}

static void state_idle(StateMachine *sm, char input) {
    if (input == 'S') {
        memset(sm->buffer, 0, MAX_INPUT_LEN);
        sm->buffer_pos = 0;
        sm->computation = 0;
        sm->error_code = 0;
    }
}

static void state_reading(StateMachine *sm, char input) {
    if (sm->buffer_pos < MAX_INPUT_LEN - 1) {
        if (isdigit(input)) {
            sm->buffer[sm->buffer_pos++] = input;
        } else if (input == 'E') {
            sm->buffer[sm->buffer_pos] = '\0';
        } else {
            sm->error_code = 1;
        }
    } else {
        sm->error_code = 2;
    }
}

static void state_processing(StateMachine *sm, char input) {
    if (sm->buffer_pos > 0) {
        char *endptr;
        long value = strtol(sm->buffer, &endptr, 10);
        if (endptr != sm->buffer && *endptr == '\0' && value >= 0) {
            for (uint64_t i = 0; i < (uint64_t)value; i++) {
                if (sm->computation > UINT64_MAX - (i * i)) {
                    sm->error_code = 3;
                    break;
                }
                sm->computation += i * i;
            }
        } else {
            sm->error_code = 4;
        }
    }
    if (input == 'R') {
        sm->buffer_pos = 0;
        memset(sm->buffer, 0, MAX_INPUT_LEN);
    }
}

static void state_complete(StateMachine *sm) {
    printf("Result: %lu\n", sm->computation);
}

static void state_error(StateMachine *sm) {
    printf("Error: %d\n", sm->error_code);
}

static void process_state(StateMachine *sm, char input) {
    switch (sm->current_state) {
        case STATE_IDLE:
            state_idle(sm, input);
            break;
        case STATE_READING:
            state_reading(sm, input);
            break;
        case STATE_PROCESSING:
            state_processing(sm, input);
            break;
        case STATE_COMPLETE:
            break;
        case STATE_ERROR:
            break;
    }
}

int main(void) {
    StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.buffer_pos = 0;
    sm.computation = 0;
    sm.error_code = 0;
    memset(sm.buffer, 0, MAX_INPUT_LEN);

    char input_buffer[MAX_INPUT_LEN];
    
    printf("State Machine Controller\n");
    printf("Commands: S (start), E (end), R (reset), digits (0-9), Q (quit)\n");
    
    while (sm.current_state != STATE_COMPLETE && sm.current_state != STATE_ERROR) {
        printf("State: %d > ", sm.current_state);
        
        if (fgets(input_buffer, MAX_INPUT_LEN, stdin) == NULL) {
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
        
        if (input_buffer[0] == 'Q') {
            break;
        }
        
        for (size_t i = 0; i < len; i++) {
            char input_char = input_buffer[i];
            process_state(&sm, input_char);
            State next_state = get_next_state(sm.current_state, input_char);
            sm.current_state = next_state;
            
            if (sm.current_state == STATE_COMPLETE) {
                state_complete(&sm);
                break;
            } else if (sm.current_state == STATE_ERROR) {