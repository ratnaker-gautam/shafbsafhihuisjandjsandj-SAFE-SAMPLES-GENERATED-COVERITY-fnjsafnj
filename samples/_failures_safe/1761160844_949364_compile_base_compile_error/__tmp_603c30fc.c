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

int is_valid_input_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '.' || c == ',';
}

State get_next_state(State current, char input) {
    static const Transition transitions[] = {
        {STATE_IDLE, 's', STATE_READING},
        {STATE_READING, 'e', STATE_PROCESSING},
        {STATE_READING, 'c', STATE_IDLE},
        {STATE_PROCESSING, 'd', STATE_DONE},
        {STATE_PROCESSING, 'r', STATE_READING},
        {STATE_DONE, 's', STATE_READING},
        {STATE_DONE, 'q', STATE_IDLE}
    };
    static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == current && transitions[i].input_char == input) {
            return transitions[i].next_state;
        }
    }
    return STATE_ERROR;
}

int process_state_action(StateMachine *machine) {
    if (machine == NULL) return 0;

    switch (machine->current_state) {
        case STATE_IDLE:
            printf("Machine idle. Enter 's' to start reading.\n");
            break;
        case STATE_READING:
            printf("Reading mode. Enter characters (a-z, 0-9, space, comma, period).\n");
            printf("Enter 'e' to process, 'c' to cancel.\n");
            break;
        case STATE_PROCESSING:
            if (machine->input_pos > 0) {
                printf("Processing: '%.*s'\n", (int)machine->input_pos, machine->input_buffer);
                machine->processed_count++;
                printf("Processed %d inputs so far.\n", machine->processed_count);
            }
            printf("Enter 'd' when done, 'r' to read more.\n");
            break;
        case STATE_DONE:
            printf("Processing complete. Enter 's' for new input, 'q' to quit.\n");
            machine->input_pos = 0;
            memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
            break;
        case STATE_ERROR:
            printf("Invalid state transition. Resetting to idle.\n");
            machine->current_state = STATE_IDLE;
            machine->input_pos = 0;
            memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
            break;
        default:
            return 0;
    }
    return 1;
}

int handle_input(StateMachine *machine, char input) {
    if (machine == NULL) return 0;

    if (machine->current_state == STATE_READING) {
        if (is_valid_input_char(input)) {
            if (machine->input_pos < MAX_INPUT_LEN) {
                machine->input_buffer[machine->input_pos++] = input;
                printf("Added: '%c'\n", input);
                return 1;
            } else {
                printf("Input buffer full. Cannot add more characters.\n");
                return 0;
            }
        }
    }

    State next_state = get_next_state(machine->current_state, input);
    if (next_state != STATE_ERROR) {
        machine->current_state = next_state;
        return 1;
    }

    printf("Invalid input '%c' for current state.\n", input);
    return 0;
}

int main(void) {
    StateMachine machine;
    char input[10];
    int running = 1;

    initialize_machine(&machine);

    printf("State Machine Demo\n");
    printf("Available commands: s (start), e (end reading), c (cancel), d (done), r (read more), q (quit)\n");
    printf("In reading mode, enter alphanumeric characters, spaces, commas, or periods.\n\n");

    while (running) {
        process_state_action(&machine);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        if (strlen(input) == 0 || input[0] == '\n') {
            continue;
        }

        char command = input[0];
        
        if (command == 'q' && machine.current_state == STATE_DONE) {
            running = 0;
            continue;
        }

        if (!handle_input(&machine, command)) {
            if (machine.current_state == STATE_ERROR) {
                process_state_action(&machine);
            }
        }
    }

    printf("Final processed