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
    return isalnum((unsigned char)c) || c == ' ' || c == '\n' || c == '\0';
}

State get_next_state(State current, char input) {
    static const Transition transitions[] = {
        {STATE_IDLE, 'S', STATE_READING},
        {STATE_IDLE, '\0', STATE_IDLE},
        {STATE_READING, 'E', STATE_PROCESSING},
        {STATE_READING, '\0', STATE_READING},
        {STATE_PROCESSING, 'X', STATE_DONE},
        {STATE_PROCESSING, '\0', STATE_PROCESSING},
        {STATE_DONE, 'R', STATE_IDLE},
        {STATE_DONE, '\0', STATE_DONE}
    };
    static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == current && transitions[i].input_char == input) {
            return transitions[i].next_state;
        }
    }

    if (input == '\0') {
        return current;
    }

    return STATE_ERROR;
}

int process_state_action(StateMachine *machine, char input) {
    if (machine == NULL) return 0;

    switch (machine->current_state) {
        case STATE_IDLE:
            if (input == 'S') {
                printf("Starting sequence...\n");
            }
            break;
        case STATE_READING:
            if (machine->input_pos < MAX_INPUT_LEN) {
                machine->input_buffer[machine->input_pos++] = input;
                printf("Reading character: %c\n", input);
            }
            break;
        case STATE_PROCESSING:
            if (machine->input_pos > 0) {
                machine->processed_count++;
                printf("Processing input: %s (count: %d)\n", machine->input_buffer, machine->processed_count);
            }
            break;
        case STATE_DONE:
            printf("Sequence completed. Reset with 'R'\n");
            if (input == 'R') {
                initialize_machine(machine);
            }
            break;
        case STATE_ERROR:
            printf("Invalid state transition!\n");
            initialize_machine(machine);
            return 0;
        default:
            return 0;
    }
    return 1;
}

int execute_state_machine(StateMachine *machine, char input) {
    if (machine == NULL || !is_valid_input(input)) {
        return 0;
    }

    State next_state = get_next_state(machine->current_state, input);
    
    if (!process_state_action(machine, input)) {
        return 0;
    }

    machine->current_state = next_state;
    return 1;
}

void print_current_state(State state) {
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
    
    initialize_machine(&machine);
    
    printf("State Machine Controller\n");
    printf("Commands: S (Start), E (End input), X (Execute), R (Reset)\n");
    printf("Input alphanumeric characters during READING state\n");
    printf("Enter 'Q' to quit\n\n");

    while (1) {
        print_current_state(machine.current_state);
        printf("Enter command: ");
        
        if (scanf(" %c", &input) != 1) {
            printf("Input error\n");
            break;
        }

        if (input == 'Q' || input == 'q') {
            printf("Exiting...\n");
            break;
        }

        if (!execute_state_machine(&machine, input)) {
            printf("Invalid operation\n");
        }
        
        printf("\n");
    }

    return 0;
}