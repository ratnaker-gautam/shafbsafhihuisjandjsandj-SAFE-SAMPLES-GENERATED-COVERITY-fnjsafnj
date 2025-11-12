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

typedef struct {
    State current;
    char trigger;
    State next;
} Transition;

typedef struct {
    State current_state;
    char input_buffer[MAX_INPUT_LEN + 1];
    size_t input_pos;
    Transition transitions[MAX_TRANSITIONS];
    size_t num_transitions;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    machine->current_state = STATE_IDLE;
    machine->input_pos = 0;
    machine->num_transitions = 0;
    memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
    
    Transition trans[] = {
        {STATE_IDLE, 'S', STATE_READING},
        {STATE_READING, 'E', STATE_PROCESSING},
        {STATE_READING, 'X', STATE_IDLE},
        {STATE_PROCESSING, 'C', STATE_COMPLETE},
        {STATE_PROCESSING, 'F', STATE_ERROR},
        {STATE_ERROR, 'R', STATE_IDLE},
        {STATE_COMPLETE, 'R', STATE_IDLE}
    };
    
    size_t num = sizeof(trans) / sizeof(trans[0]);
    if (num <= MAX_TRANSITIONS) {
        memcpy(machine->transitions, trans, sizeof(trans));
        machine->num_transitions = num;
    }
}

State find_next_state(StateMachine *machine, char trigger) {
    for (size_t i = 0; i < machine->num_transitions; i++) {
        if (machine->transitions[i].current == machine->current_state && 
            machine->transitions[i].trigger == trigger) {
            return machine->transitions[i].next;
        }
    }
    return machine->current_state;
}

void handle_idle_state(StateMachine *machine, char input) {
    if (input == 'S') {
        printf("Starting data collection...\n");
        machine->input_pos = 0;
        memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
    }
}

void handle_reading_state(StateMachine *machine, char input) {
    if (input == 'E') {
        printf("End of input received. Data: %s\n", machine->input_buffer);
    } else if (input == 'X') {
        printf("Input cancelled.\n");
        machine->input_pos = 0;
        memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
    } else if (isalnum((unsigned char)input) && machine->input_pos < MAX_INPUT_LEN) {
        machine->input_buffer[machine->input_pos++] = input;
        machine->input_buffer[machine->input_pos] = '\0';
        printf("Added character: %c\n", input);
    }
}

void handle_processing_state(StateMachine *machine, char input) {
    if (input == 'C') {
        printf("Processing completed successfully.\n");
    } else if (input == 'F') {
        printf("Processing failed.\n");
    }
}

void handle_complete_state(StateMachine *machine, char input) {
    if (input == 'R') {
        printf("Resetting system.\n");
        machine->input_pos = 0;
        memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
    }
}

void handle_error_state(StateMachine *machine, char input) {
    if (input == 'R') {
        printf("Recovering from error state.\n");
        machine->input_pos = 0;
        memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
    }
}

void process_input(StateMachine *machine, char input) {
    State next_state = find_next_state(machine, input);
    
    if (next_state != machine->current_state) {
        printf("Transition: %d -> %d\n", machine->current_state, next_state);
        machine->current_state = next_state;
    }
    
    switch (machine->current_state) {
        case STATE_IDLE:
            handle_idle_state(machine, input);
            break;
        case STATE_READING:
            handle_reading_state(machine, input);
            break;
        case STATE_PROCESSING:
            handle_processing_state(machine, input);
            break;
        case STATE_COMPLETE:
            handle_complete_state(machine, input);
            break;
        case STATE_ERROR:
            handle_error_state(machine, input);
            break;
    }
}

void print_instructions(void) {
    printf("State Machine Controller\n");
    printf("Commands:\n");
    printf("  S - Start data collection\n");
    printf("  E - End data collection\n");
    printf("  X - Cancel data collection\n");
    printf("  C - Complete processing\n");
    printf("  F - Fail processing\n");
    printf("  R - Reset system\n");
    printf("  Q - Quit program\n");
    printf("  alphanumeric - Add data during collection\n");
    printf("Enter commands one at a time:\n");
}

int main(void) {
    StateMachine machine;
    char input[10];
    
    initialize_machine(&machine);
    print_instructions();
    
    while (1