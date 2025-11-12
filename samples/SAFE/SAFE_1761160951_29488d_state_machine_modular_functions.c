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
    STATE_DONE,
    STATE_ERROR
} State;

typedef struct {
    State current_state;
    char input_char;
    State next_state;
} Transition;

typedef struct {
    State current_state;
    char input_buffer[MAX_INPUT_LEN + 1];
    size_t input_pos;
    int processed_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_IDLE;
    machine->input_pos = 0;
    machine->processed_count = 0;
    memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
}

int is_valid_input(char c) {
    return isalnum(c) || c == ' ' || c == '\n';
}

State get_next_state(State current, char input) {
    static const Transition transitions[] = {
        {STATE_IDLE, 'S', STATE_READING},
        {STATE_READING, 'E', STATE_PROCESSING},
        {STATE_READING, 'X', STATE_ERROR},
        {STATE_PROCESSING, 'C', STATE_DONE},
        {STATE_PROCESSING, 'X', STATE_ERROR},
        {STATE_DONE, 'R', STATE_IDLE},
        {STATE_ERROR, 'R', STATE_IDLE}
    };
    
    static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);
    
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == current && transitions[i].input_char == input) {
            return transitions[i].next_state;
        }
    }
    
    if (current == STATE_READING && is_valid_input(input)) {
        return STATE_READING;
    }
    
    return current;
}

int process_state_action(StateMachine *machine, char input) {
    if (machine == NULL) return 0;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            if (input == 'S') {
                printf("Starting data collection...\n");
            }
            break;
            
        case STATE_READING:
            if (is_valid_input(input) && machine->input_pos < MAX_INPUT_LEN) {
                machine->input_buffer[machine->input_pos++] = input;
                printf("Collected: %c\n", input);
            }
            break;
            
        case STATE_PROCESSING:
            if (input == 'C') {
                machine->processed_count++;
                printf("Processing complete. Count: %d\n", machine->processed_count);
            }
            break;
            
        case STATE_DONE:
            if (input == 'R') {
                printf("Resetting machine.\n");
                initialize_machine(machine);
            }
            break;
            
        case STATE_ERROR:
            printf("Error state detected. Press 'R' to reset.\n");
            if (input == 'R') {
                initialize_machine(machine);
            }
            break;
    }
    
    return 1;
}

void print_state_info(State state) {
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "DONE", "ERROR"
    };
    
    if (state >= STATE_IDLE && state <= STATE_ERROR) {
        printf("Current state: %s\n", state_names[state]);
    }
}

int main(void) {
    StateMachine machine;
    char input;
    int running = 1;
    
    initialize_machine(&machine);
    printf("State Machine Controller\n");
    printf("Commands: S=Start, E=End input, C=Complete, R=Reset, X=Error, Q=Quit\n");
    printf("Other alphanumeric characters are collected as data.\n");
    
    while (running) {
        print_state_info(machine.current_state);
        printf("Enter command: ");
        
        int result = scanf(" %c", &input);
        if (result != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a single character.\n");
            continue;
        }
        
        if (input == 'Q') {
            running = 0;
            continue;
        }
        
        State next_state = get_next_state(machine.current_state, input);
        
        if (next_state != machine.current_state) {
            machine.current_state = next_state;
        }
        
        if (!process_state_action(&machine, input)) {
            printf("Action processing failed.\n");
        }
        
        printf("\n");
    }
    
    printf("Final processed count: %d\n", machine.processed_count);
    printf("Goodbye!\n");
    
    return 0;
}