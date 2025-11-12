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
    State current_state;
    char input_char;
    State next_state;
} Transition;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_pos;
    int processed_count;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_READING, 'E', STATE_PROCESSING},
    {STATE_READING, 'X', STATE_ERROR},
    {STATE_PROCESSING, 'C', STATE_COMPLETE},
    {STATE_PROCESSING, 'X', STATE_ERROR},
    {STATE_ERROR, 'R', STATE_IDLE},
    {STATE_COMPLETE, 'R', STATE_IDLE}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void initialize_machine(StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_IDLE;
    machine->buffer_pos = 0;
    machine->processed_count = 0;
    memset(machine->buffer, 0, sizeof(machine->buffer));
}

int is_valid_input(char c) {
    return isprint(c) && c != '\n' && c != '\r';
}

State get_next_state(State current, char input) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == current && 
            transitions[i].input_char == input) {
            return transitions[i].next_state;
        }
    }
    return current;
}

void handle_state_action(StateMachine *machine, char input) {
    switch (machine->current_state) {
        case STATE_IDLE:
            if (input == 'S') {
                printf("Starting data collection...\n");
            }
            break;
        case STATE_READING:
            if (machine->buffer_pos < MAX_INPUT_LEN - 1) {
                machine->buffer[machine->buffer_pos++] = input;
                printf("Collected character: %c\n", input);
            }
            break;
        case STATE_PROCESSING:
            printf("Processing data: %s\n", machine->buffer);
            machine->processed_count++;
            break;
        case STATE_COMPLETE:
            printf("Processing complete. Total processed: %d\n", machine->processed_count);
            break;
        case STATE_ERROR:
            printf("Error state encountered. Input: %c\n", input);
            break;
    }
}

void process_input(StateMachine *machine, char input) {
    if (machine == NULL || !is_valid_input(input)) {
        return;
    }
    
    State next_state = get_next_state(machine->current_state, input);
    
    if (next_state != machine->current_state) {
        machine->current_state = next_state;
        handle_state_action(machine, input);
        
        if (machine->current_state == STATE_IDLE) {
            machine->buffer_pos = 0;
            memset(machine->buffer, 0, sizeof(machine->buffer));
        }
    }
}

void print_state_info(State state) {
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "COMPLETE", "ERROR"
    };
    printf("Current state: %s\n", state_names[state]);
}

void print_instructions(void) {
    printf("State Machine Controller\n");
    printf("Commands: S=Start, E=End input, C=Complete, X=Error, R=Reset\n");
    printf("Other characters are collected as data in READING state\n");
    printf("Enter 'Q' to quit\n");
}

int main(void) {
    StateMachine machine;
    char input[MAX_INPUT_LEN];
    
    initialize_machine(&machine);
    print_instructions();
    
    while (1) {
        print_state_info(machine.current_state);
        printf("Enter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (input[0] == 'Q' && (input[1] == '\n' || input[1] == '\0')) {
            break;
        }
        
        for (char *p = input; *p != '\0' && *p != '\n'; p++) {
            if (is_valid_input(*p)) {
                process_input(&machine, *p);
            }
        }
        
        if (machine.current_state == STATE_ERROR) {
            printf("In error state. Press 'R' to reset.\n");
        }
    }
    
    printf("Final state: ");
    print_state_info(machine.current_state);
    printf("Total processing cycles: %d\n", machine.processed_count);
    
    return 0;
}