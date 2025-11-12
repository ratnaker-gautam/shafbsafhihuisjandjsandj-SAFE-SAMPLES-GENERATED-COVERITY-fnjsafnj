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

const Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING},
    {STATE_READING, EVENT_DATA, STATE_READING},
    {STATE_READING, EVENT_PROCESS, STATE_PROCESSING},
    {STATE_PROCESSING, EVENT_SUCCESS, STATE_COMPLETE},
    {STATE_PROCESSING, EVENT_FAILURE, STATE_ERROR},
    {STATE_COMPLETE, EVENT_RESET, STATE_IDLE},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE}
};

const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void initialize_machine(StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_IDLE;
    machine->buffer_len = 0;
    machine->data_count = 0;
    memset(machine->buffer, 0, sizeof(machine->buffer));
}

State handle_transition(State current_state, Event event) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == current_state && 
            transitions[i].event == event) {
            return transitions[i].next_state;
        }
    }
    return current_state;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isalnum((unsigned char)input[i]) && input[i] != ' ') return 0;
    }
    return 1;
}

void process_state_idle(StateMachine *machine) {
    printf("State: IDLE - Enter 'start' to begin: ");
}

void process_state_reading(StateMachine *machine) {
    printf("State: READING - Enter data or 'process' to continue: ");
}

void process_state_processing(StateMachine *machine) {
    printf("State: PROCESSING - Processing %d data items\n", machine->data_count);
    if (machine->data_count > 0 && machine->buffer_len > 0) {
        printf("Processed: '%.*s'\n", (int)machine->buffer_len, machine->buffer);
        machine->current_state = handle_transition(machine->current_state, EVENT_SUCCESS);
    } else {
        machine->current_state = handle_transition(machine->current_state, EVENT_FAILURE);
    }
}

void process_state_complete(StateMachine *machine) {
    printf("State: COMPLETE - Processing successful. Enter 'reset' to continue: ");
}

void process_state_error(StateMachine *machine) {
    printf("State: ERROR - Processing failed. Enter 'reset' to continue: ");
}

void process_input(StateMachine *machine, const char *input) {
    if (machine == NULL || input == NULL) return;
    
    size_t input_len = strlen(input);
    if (!validate_input(input, input_len)) {
        printf("Invalid input\n");
        return;
    }

    Event current_event = EVENT_RESET;
    
    if (strcmp(input, "start") == 0) {
        current_event = EVENT_START;
    } else if (strcmp(input, "process") == 0) {
        current_event = EVENT_PROCESS;
    } else if (strcmp(input, "reset") == 0) {
        current_event = EVENT_RESET;
    } else {
        current_event = EVENT_DATA;
    }

    State new_state = handle_transition(machine->current_state, current_event);
    
    if (current_event == EVENT_DATA && machine->current_state == STATE_READING) {
        if (machine->buffer_len + input_len < MAX_INPUT_LEN) {
            strncpy(machine->buffer + machine->buffer_len, input, input_len);
            machine->buffer_len += input_len;
            machine->data_count++;
            printf("Added data: %s (Total: %d)\n", input, machine->data_count);
        } else {
            printf("Buffer full\n");
        }
    } else if (current_event == EVENT_RESET) {
        initialize_machine(machine);
        new_state = STATE_IDLE;
    }
    
    machine->current_state = new_state;
}

void run_state_machine(StateMachine *machine) {
    if (machine == NULL) return;
    
    char input[MAX_INPUT_LEN];
    
    while (1) {
        switch (machine->current_state) {
            case STATE_IDLE:
                process_state_idle(machine);
                break;
            case STATE_READING:
                process_state_reading(machine);
                break;
            case STATE_PROCESSING:
                process_state_processing(machine);
                break;
            case