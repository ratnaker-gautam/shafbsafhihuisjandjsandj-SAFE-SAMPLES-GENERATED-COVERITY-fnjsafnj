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

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isalnum((unsigned char)input[i]) && input[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

int handle_idle_state(StateMachine *machine, const char *input) {
    if (machine == NULL || input == NULL) return 0;
    if (strcmp(input, "start") == 0) {
        machine->current_state = get_next_state(machine->current_state, EVENT_START);
        printf("State transition: IDLE -> READING\n");
        return 1;
    }
    return 0;
}

int handle_reading_state(StateMachine *machine, const char *input) {
    if (machine == NULL || input == NULL) return 0;
    
    if (strcmp(input, "process") == 0) {
        if (machine->data_count > 0) {
            machine->current_state = get_next_state(machine->current_state, EVENT_PROCESS);
            printf("State transition: READING -> PROCESSING\n");
            return 1;
        }
        return 0;
    }
    
    if (is_valid_input(input)) {
        size_t input_len = strlen(input);
        if (machine->buffer_len + input_len < MAX_INPUT_LEN - 1) {
            strcat(machine->buffer, input);
            strcat(machine->buffer, " ");
            machine->buffer_len += input_len + 1;
            machine->data_count++;
            printf("Data added: %s (Total data items: %d)\n", input, machine->data_count);
            return 1;
        }
    }
    return 0;
}

int handle_processing_state(StateMachine *machine, const char *input) {
    if (machine == NULL || input == NULL) return 0;
    
    if (strcmp(input, "success") == 0) {
        machine->current_state = get_next_state(machine->current_state, EVENT_SUCCESS);
        printf("State transition: PROCESSING -> COMPLETE\n");
        printf("Processed data: %s\n", machine->buffer);
        return 1;
    } else if (strcmp(input, "failure") == 0) {
        machine->current_state = get_next_state(machine->current_state, EVENT_FAILURE);
        printf("State transition: PROCESSING -> ERROR\n");
        return 1;
    }
    return 0;
}

int handle_complete_state(StateMachine *machine, const char *input) {
    if (machine == NULL || input == NULL) return 0;
    
    if (strcmp(input, "reset") == 0) {
        machine->current_state = get_next_state(machine->current_state, EVENT_RESET);
        printf("State transition: COMPLETE -> IDLE\n");
        initialize_machine(machine);
        return 1;
    }
    return 0;
}

int handle_error_state(StateMachine *machine, const char *input) {
    if (machine == NULL || input == NULL) return 0;
    
    if (strcmp(input, "reset") == 0) {
        machine->current_state = get_next_state(machine->current_state, EVENT_RESET);
        printf("State transition: ERROR -> IDLE\n");
        initialize_m