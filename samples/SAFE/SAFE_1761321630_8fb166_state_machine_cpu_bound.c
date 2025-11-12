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
    char input_char;
    State next;
} Transition;

typedef struct {
    State current_state;
    char input_buffer[MAX_INPUT_LEN + 1];
    size_t input_len;
    int processed_count;
    int error_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    machine->current_state = STATE_IDLE;
    machine->input_len = 0;
    machine->processed_count = 0;
    machine->error_count = 0;
    memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
}

State handle_transition(State current, char input, Transition *transitions, int num_transitions) {
    for (int i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input_char == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

int is_valid_input(char c) {
    return isalnum(c) || c == ' ' || c == '\n';
}

void process_character(StateMachine *machine, char input) {
    static Transition transitions[] = {
        {STATE_IDLE, 'S', STATE_READING},
        {STATE_READING, 'P', STATE_PROCESSING},
        {STATE_PROCESSING, 'C', STATE_COMPLETE},
        {STATE_READING, 'E', STATE_ERROR},
        {STATE_PROCESSING, 'E', STATE_ERROR},
        {STATE_COMPLETE, 'R', STATE_IDLE},
        {STATE_ERROR, 'R', STATE_IDLE}
    };
    static int num_transitions = sizeof(transitions) / sizeof(transitions[0]);

    if (!is_valid_input(input)) {
        machine->error_count++;
        return;
    }

    State new_state = handle_transition(machine->current_state, input, transitions, num_transitions);
    
    if (new_state == STATE_ERROR) {
        machine->error_count++;
    } else {
        machine->current_state = new_state;
    }

    if (machine->current_state == STATE_PROCESSING) {
        if (machine->input_len < MAX_INPUT_LEN) {
            machine->input_buffer[machine->input_len++] = input;
        }
    } else if (machine->current_state == STATE_COMPLETE) {
        machine->processed_count++;
        machine->input_len = 0;
        memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
    }
}

void simulate_workload(int iterations) {
    volatile uint64_t counter = 0;
    for (int i = 0; i < iterations; i++) {
        counter += i * i;
        counter ^= (counter << 5);
        counter ^= (counter >> 13);
        counter ^= (counter << 7);
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    char input_sequence[] = "SPCSPESPCSPCRSPCR";
    size_t seq_len = strlen(input_sequence);
    
    if (seq_len == 0 || seq_len > 1000) {
        fprintf(stderr, "Invalid input sequence length\n");
        return EXIT_FAILURE;
    }
    
    printf("Starting state machine simulation...\n");
    
    for (size_t i = 0; i < seq_len; i++) {
        char current_char = input_sequence[i];
        
        if (!is_valid_input(current_char)) {
            fprintf(stderr, "Invalid character in input sequence\n");
            return EXIT_FAILURE;
        }
        
        process_character(&machine, current_char);
        simulate_workload(10000);
        
        printf("Step %zu: Input '%c' -> State %d, Processed: %d, Errors: %d\n",
               i + 1, current_char, machine.current_state, 
               machine.processed_count, machine.error_count);
        
        if (machine.error_count > 5) {
            printf("Too many errors, terminating\n");
            break;
        }
    }
    
    printf("Final state: %d\n", machine.current_state);
    printf("Total processed: %d\n", machine.processed_count);
    printf("Total errors: %d\n", machine.error_count);
    
    return EXIT_SUCCESS;
}