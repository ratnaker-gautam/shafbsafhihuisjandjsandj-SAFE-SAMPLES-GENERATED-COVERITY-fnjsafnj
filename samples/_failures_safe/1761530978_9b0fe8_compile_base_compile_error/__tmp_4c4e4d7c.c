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
    EVENT_ERROR,
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
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR},
    {STATE_COMPLETE, EVENT_RESET, STATE_IDLE},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void initialize_machine(StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_IDLE;
    machine->buffer[0] = '\0';
    machine->buffer_len = 0;
    machine->data_count = 0;
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
        if (!isprint(input[i])) return 0;
    }
    return 1;
}

int handle_idle_state(StateMachine *machine, const char *input) {
    if (strcmp(input, "start") == 0) {
        return EVENT_START;
    }
    return -1;
}

int handle_reading_state(StateMachine *machine, const char *input) {
    if (strcmp(input, "process") == 0) {
        if (machine->data_count > 0) {
            return EVENT_PROCESS;
        }
        return -1;
    }
    
    if (is_valid_input(input)) {
        size_t input_len = strlen(input);
        if (machine->buffer_len + input_len + 1 < MAX_INPUT_LEN) {
            strcat(machine->buffer, input);
            strcat(machine->buffer, " ");
            machine->buffer_len += input_len + 1;
            machine->data_count++;
            return EVENT_DATA;
        }
    }
    return -1;
}

int handle_processing_state(StateMachine *machine, const char *input) {
    if (strcmp(input, "success") == 0) {
        return EVENT_SUCCESS;
    } else if (strcmp(input, "error") == 0) {
        return EVENT_ERROR;
    }
    return -1;
}

int handle_complete_state(StateMachine *machine, const char *input) {
    if (strcmp(input, "reset") == 0) {
        return EVENT_RESET;
    }
    return -1;
}

int handle_error_state(StateMachine *machine, const char *input) {
    if (strcmp(input, "reset") == 0) {
        return EVENT_RESET;
    }
    return -1;
}

int process_input(StateMachine *machine, const char *input) {
    if (machine == NULL || input == NULL) return -1;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            return handle_idle_state(machine, input);
        case STATE_READING:
            return handle_reading_state(machine, input);
        case STATE_PROCESSING:
            return handle_processing_state(machine, input);
        case STATE_COMPLETE:
            return handle_complete_state(machine, input);
        case STATE_ERROR:
            return handle_error_state(machine, input);
        default:
            return -1;
    }
}

void print_state_info(StateMachine *machine) {
    printf("Current state: ");
    switch (machine->current_state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READING: printf("READING"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_COMPLETE: printf("COMPLETE"); break;
        case STATE_ERROR: printf("ERROR"); break;
        default: printf("UNKNOWN"); break;
    }
    printf("\n");
    
    if (machine->current_state == STATE_READING || machine->current_state == STATE_PROCESSING) {
        printf("Data collected: %s\n", machine->buffer);
        printf("Data items: %d\n", machine->data_count);
    }
}

void reset_machine(StateMachine *machine) {
    if (