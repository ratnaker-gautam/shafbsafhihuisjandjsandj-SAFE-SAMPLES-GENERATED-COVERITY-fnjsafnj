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
    return isprint((unsigned char)c) && c != '\n' && c != '\r';
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
            break;
        case STATE_READING:
            if (machine->buffer_pos < MAX_INPUT_LEN - 1) {
                machine->buffer[machine->buffer_pos++] = input;
                machine->buffer[machine->buffer_pos] = '\0';
            }
            break;
        case STATE_PROCESSING:
            machine->processed_count++;
            break;
        case STATE_COMPLETE:
            printf("Processing complete. Count: %d\n", machine->processed_count);
            break;
        case STATE_ERROR:
            printf("Error state encountered. Buffer: %s\n", machine->buffer);
            break;
    }
}

void process_input(StateMachine *machine, char input) {
    if (machine == NULL || !is_valid_input(input)) return;
    
    State next_state = get_next_state(machine->current_state, input);
    
    if (next_state != machine->current_state) {
        machine->current_state = next_state;
        handle_state_action(machine, input);
    }
}

void print_state_info(State state) {
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "COMPLETE", "ERROR"
    };
    if (state >= STATE_IDLE && state <= STATE_ERROR) {
        printf("Current state: %s\n", state_names[state]);
    }
}

int main(void) {
    StateMachine machine;
    char input[MAX_INPUT_LEN];
    
    initialize_machine(&machine);
    
    printf("State Machine Controller\n");
    printf("Commands: S=Start, E=End Input, C=Complete, X=Error, R=Reset, Q=Quit\n");
    
    while (1) {
        print_state_info(machine.current_state);
        printf("Enter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (strlen(input) == 0 || input[0] == '\n') {
            continue;
        }
        
        char command = input[0];
        
        if (command == 'Q' || command == 'q') {
            break;
        }
        
        process_input(&machine, command);
        
        if (machine.current_state == STATE_COMPLETE) {
            printf("Buffer contents: %s\n", machine.buffer);
            printf("Processed items: %d\n", machine.processed_count);
        }
    }
    
    printf("Exiting state machine controller.\n");
    return 0;
}