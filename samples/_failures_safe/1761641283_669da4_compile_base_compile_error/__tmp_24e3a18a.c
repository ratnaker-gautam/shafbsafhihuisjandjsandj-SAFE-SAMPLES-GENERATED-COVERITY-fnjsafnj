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

Event process_input(StateMachine *machine, const char *input) {
    if (machine == NULL || input == NULL) return EVENT_FAILURE;
    
    size_t input_len = strlen(input);
    if (!validate_input(input, input_len)) return EVENT_FAILURE;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            if (strcmp(input, "start") == 0) return EVENT_START;
            break;
            
        case STATE_READING:
            if (strcmp(input, "process") == 0) {
                if (machine->data_count > 0) return EVENT_PROCESS;
            } else {
                if (machine->buffer_len + input_len < MAX_INPUT_LEN - 1) {
                    strcat(machine->buffer, input);
                    strcat(machine->buffer, " ");
                    machine->buffer_len += input_len + 1;
                    machine->data_count++;
                    return EVENT_DATA;
                }
            }
            break;
            
        case STATE_PROCESSING:
            if (strcmp(input, "success") == 0) return EVENT_SUCCESS;
            if (strcmp(input, "failure") == 0) return EVENT_FAILURE;
            break;
            
        case STATE_COMPLETE:
        case STATE_ERROR:
            if (strcmp(input, "reset") == 0) return EVENT_RESET;
            break;
            
        default:
            break;
    }
    
    return EVENT_FAILURE;
}

void execute_state_action(StateMachine *machine) {
    if (machine == NULL) return;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            printf("State: IDLE - Enter 'start' to begin\n");
            break;
            
        case STATE_READING:
            printf("State: READING - Enter data or 'process' to continue\n");
            printf("Current data: %s\n", machine->buffer);
            break;
            
        case STATE_PROCESSING:
            printf("State: PROCESSING - Processing data...\n");
            printf("Processing %d data items: %s\n", machine->data_count, machine->buffer);
            printf("Enter 'success' or 'failure'\n");
            break;
            
        case STATE_COMPLETE:
            printf("State: COMPLETE - Processing successful!\n");
            printf("Final result: Processed %d items\n", machine->data_count);
            printf("Enter 'reset' to start over\n");
            break;
            
        case STATE_ERROR:
            printf("State: ERROR - Processing failed\n");
            printf("Enter 'reset' to start over\n");
            break;
    }
}

int main(void) {
    StateMachine machine;
    char input[MAX_INPUT_LEN];
    
    initialize_machine(&machine);
    
    printf("State Machine Demo - Data Processor\n");
    printf("Valid inputs: 'start', alphanumeric data, 'process', 'success', 'failure', 'reset'\n\n");
    
    while (1) {
        execute_state_action(&machine);
        
        printf("> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t