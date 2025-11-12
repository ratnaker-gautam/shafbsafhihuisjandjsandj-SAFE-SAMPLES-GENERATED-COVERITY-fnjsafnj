//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_LENGTH 100
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
    char buffer[MAX_INPUT_LENGTH];
    size_t buffer_index;
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
    machine->buffer_index = 0;
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
    
    if (strlen(input) == 1 && input[0] == 's') {
        printf("Starting data collection...\n");
        return 1;
    }
    return 0;
}

int handle_reading_state(StateMachine *machine, const char *input) {
    if (machine == NULL || input == NULL) return 0;
    
    size_t input_len = strlen(input);
    if (input_len == 0) return 0;
    
    if (input_len == 1 && input[0] == 'p') {
        if (machine->buffer_index > 0) {
            printf("Processing %zu characters...\n", machine->buffer_index);
            return 1;
        }
        return 0;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        if (!is_valid_input_char(input[i])) {
            printf("Invalid character detected: %c\n", input[i]);
            return 0;
        }
        
        if (machine->buffer_index < MAX_INPUT_LENGTH - 1) {
            machine->buffer[machine->buffer_index++] = input[i];
        } else {
            printf("Buffer full, cannot accept more data\n");
            return 0;
        }
    }
    
    machine->data_count++;
    printf("Added %zu characters, total data entries: %d\n", input_len, machine->data_count);
    return 1;
}

int handle_processing_state(StateMachine *machine) {
    if (machine == NULL) return 0;
    
    if (machine->buffer_index == 0) {
        printf("No data to process\n");
        return 0;
    }
    
    int success = 1;
    for (size_t i = 0; i < machine->buffer_index; i++) {
        if (!is_valid_input_char(machine->buffer[i])) {
            success = 0;
            break;
        }
    }
    
    if (success) {
        printf("Processing successful! Processed: %s\n", machine->buffer);
        return 1;
    } else {
        printf("Processing failed: invalid data detected\n");
        return 0;
    }
}

void print_state_info(State state) {
    switch (state) {
        case STATE_IDLE:
            printf("Current state: IDLE (Enter 's' to start, 'q' to quit)\n");
            break;
        case STATE_READING:
            printf("Current state: READING (Enter data or 'p' to process)\n");
            break;
        case STATE_PROCESSING:
            printf("Current state: PROCESSING\n");
            break;
        case STATE_COMPLETE:
            printf("Current state: COMPLETE (Enter 'r' to reset)\n");
            break;
        case STATE_ERROR:
            printf("Current state: ERROR (Enter 'r' to reset)\n");
            break;
    }
}

int main(void) {
    StateMachine machine;
    char input[MAX_INPUT_LENGTH + 2];
    
    initialize_machine(&machine);
    
    printf("State Machine Demo - Data Processor\n");
    printf("===================================\n");
    
    while (1) {
        print_state_info(machine.current_state);
        printf("> ");
        
        if (fgets(input,