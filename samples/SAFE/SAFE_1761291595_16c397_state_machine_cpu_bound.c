//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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
    char input;
    State next;
} Transition;

typedef struct {
    State current_state;
    char input_buffer[MAX_INPUT_LEN];
    size_t input_len;
    int processed_count;
    int error_count;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_IDLE, 'Q', STATE_COMPLETE},
    {STATE_READING, 'A', STATE_PROCESSING},
    {STATE_READING, 'E', STATE_ERROR},
    {STATE_PROCESSING, 'C', STATE_IDLE},
    {STATE_PROCESSING, 'E', STATE_ERROR},
    {STATE_ERROR, 'R', STATE_IDLE},
    {STATE_ERROR, 'Q', STATE_COMPLETE}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void initialize_machine(StateMachine *machine) {
    machine->current_state = STATE_IDLE;
    machine->input_len = 0;
    machine->processed_count = 0;
    machine->error_count = 0;
    memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
}

State get_next_state(State current, char input) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return current;
}

void process_state(StateMachine *machine) {
    switch (machine->current_state) {
        case STATE_IDLE:
            break;
        case STATE_READING:
            if (machine->input_len < MAX_INPUT_LEN - 1) {
                machine->input_buffer[machine->input_len] = 'R';
                machine->input_len++;
            }
            break;
        case STATE_PROCESSING:
            if (machine->input_len > 0) {
                for (size_t i = 0; i < machine->input_len; i++) {
                    machine->input_buffer[i] = toupper(machine->input_buffer[i]);
                }
                machine->processed_count++;
            }
            break;
        case STATE_ERROR:
            machine->error_count++;
            break;
        case STATE_COMPLETE:
            break;
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    char input_sequence[] = "SAECRCQ";
    size_t seq_len = strlen(input_sequence);
    
    if (seq_len == 0) {
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < seq_len; i++) {
        char current_input = input_sequence[i];
        
        if (!isalpha(current_input)) {
            continue;
        }
        
        State next_state = get_next_state(machine.current_state, current_input);
        
        if (next_state != machine.current_state) {
            machine.current_state = next_state;
            process_state(&machine);
        }
        
        if (machine.current_state == STATE_COMPLETE) {
            break;
        }
    }
    
    int result = 0;
    for (int i = 0; i < 1000000; i++) {
        result += machine.processed_count * i;
        result -= machine.error_count * i;
        if (result < 0) {
            result = 0;
        }
    }
    
    printf("Final state: %d\n", machine.current_state);
    printf("Processed operations: %d\n", machine.processed_count);
    printf("Error count: %d\n", machine.error_count);
    printf("Computation result: %d\n", result);
    
    return EXIT_SUCCESS;
}