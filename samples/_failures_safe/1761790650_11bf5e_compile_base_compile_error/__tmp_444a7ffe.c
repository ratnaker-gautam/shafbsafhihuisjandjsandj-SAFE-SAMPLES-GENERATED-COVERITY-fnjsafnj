//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    Event event;
    State next_state;
} Transition;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING},
    {STATE_READING, EVENT_DATA, STATE_READING},
    {STATE_READING, EVENT_PROCESS, STATE_PROCESSING},
    {STATE_PROCESSING, EVENT_SUCCESS, STATE_COMPLETE},
    {STATE_PROCESSING, EVENT_FAILURE, STATE_ERROR},
    {STATE_COMPLETE, EVENT_RESET, STATE_IDLE},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void initialize_machine(StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_IDLE;
    machine->buffer_len = 0;
    machine->data_count = 0;
    memset(machine->buffer, 0, sizeof(machine->buffer));
}

State get_next_state(State current, Event event) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == current && transitions[i].event == event) {
            return transitions[i].next_state;
        }
    }
    return current;
}

int is_valid_input_char(char c) {
    return isalnum(c) || c == ' ' || c == '.' || c == ',';
}

int handle_idle_state(StateMachine *machine, const char *input) {
    if (machine == NULL || input == NULL) return 0;
    if (strlen(input) == 0) return 0;
    
    if (strcmp(input, "start") == 0) {
        machine->current_state = get_next_state(machine->current_state, EVENT_START);
        return 1;
    }
    return 0;
}

int handle_reading_state(StateMachine *machine, const char *input) {
    if (machine == NULL || input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len == 0) return 0;
    
    if (strcmp(input, "process") == 0) {
        machine->current_state = get_next_state(machine->current_state, EVENT_PROCESS);
        return 1;
    }
    
    if (machine->buffer_len + input_len >= MAX_INPUT_LEN) {
        return 0;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        if (!is_valid_input_char(input[i])) {
            return 0;
        }
    }
    
    memcpy(machine->buffer + machine->buffer_len, input, input_len);
    machine->buffer_len += input_len;
    machine->data_count++;
    machine->current_state = get_next_state(machine->current_state, EVENT_DATA);
    return 1;
}

int handle_processing_state(StateMachine *machine) {
    if (machine == NULL) return 0;
    
    if (machine->data_count > 0 && machine->buffer_len > 0) {
        int valid_count = 0;
        for (size_t i = 0; i < machine->buffer_len; i++) {
            if (isalnum(machine->buffer[i])) {
                valid_count++;
            }
        }
        
        if (valid_count >= 3) {
            machine->current_state = get_next_state(machine->current_state, EVENT_SUCCESS);
        } else {
            machine->current_state = get_next_state(machine->current_state, EVENT_FAILURE);
        }
        return 1;
    } else {
        machine->current_state = get_next_state(machine->current_state, EVENT_FAILURE);
        return 1;
    }
}

int handle_complete_state(StateMachine *machine, const char *input) {
    if (machine == NULL || input == NULL) return 0;
    
    if (strcmp(input, "reset") == 0) {
        initialize_machine(machine);
        machine->current_state = get_next_state(machine->current_state, EVENT_RESET);
        return 1;
    }
    return 0;
}

int handle_error_state(StateMachine *machine, const char *input) {
    if (machine == NULL || input == NULL) return 0;
    
    if (strcmp(input, "reset") == 0) {
        initialize_machine(machine);
        machine->current_state = get_next_state(machine->current_state, EVENT_RESET);
        return 1;
    }
    return 0;
}

void print_state_info(const StateMachine *machine) {
    if (machine == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "COMPLETE", "ERROR"
    };