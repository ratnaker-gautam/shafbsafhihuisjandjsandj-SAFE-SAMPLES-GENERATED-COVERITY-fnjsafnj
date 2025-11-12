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

int add_transition(Transition transitions[], int *count, State current, char input, State next) {
    if (*count >= MAX_TRANSITIONS) {
        return 0;
    }
    transitions[*count].current = current;
    transitions[*count].input_char = input;
    transitions[*count].next = next;
    (*count)++;
    return 1;
}

State get_next_state(const Transition transitions[], int count, State current, char input) {
    for (int i = 0; i < count; i++) {
        if (transitions[i].current == current && transitions[i].input_char == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

void process_character(StateMachine *machine, char c) {
    static Transition transitions[MAX_TRANSITIONS];
    static int transition_count = 0;
    
    if (transition_count == 0) {
        add_transition(transitions, &transition_count, STATE_IDLE, 'S', STATE_READING);
        add_transition(transitions, &transition_count, STATE_READING, 'A', STATE_READING);
        add_transition(transitions, &transition_count, STATE_READING, 'B', STATE_READING);
        add_transition(transitions, &transition_count, STATE_READING, 'C', STATE_READING);
        add_transition(transitions, &transition_count, STATE_READING, 'E', STATE_PROCESSING);
        add_transition(transitions, &transition_count, STATE_PROCESSING, 'P', STATE_COMPLETE);
        add_transition(transitions, &transition_count, STATE_COMPLETE, 'R', STATE_IDLE);
    }
    
    State next_state = get_next_state(transitions, transition_count, machine->current_state, c);
    
    if (next_state == STATE_ERROR) {
        machine->error_count++;
        machine->current_state = STATE_IDLE;
        machine->input_len = 0;
        memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
        return;
    }
    
    machine->current_state = next_state;
    
    if (machine->current_state == STATE_READING) {
        if (machine->input_len < MAX_INPUT_LEN) {
            machine->input_buffer[machine->input_len] = c;
            machine->input_len++;
        }
    }
    
    if (machine->current_state == STATE_PROCESSING) {
        int sum = 0;
        for (size_t i = 0; i < machine->input_len; i++) {
            sum += (int)machine->input_buffer[i];
        }
        machine->processed_count += sum;
        machine->input_len = 0;
        memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    char input_sequence[] = "SAAABCEPCBBEPSAABCEPR";
    size_t seq_len = strlen(input_sequence);
    
    if (seq_len == 0) {
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < seq_len; i++) {
        char current_char = input_sequence[i];
        if (!isalpha(current_char)) {
            continue;
        }
        process_character(&machine, current_char);
    }
    
    int iterations = 10000;
    for (int iter = 0; iter < iterations; iter++) {
        StateMachine temp_machine;
        initialize_machine(&temp_machine);
        
        for (size_t i = 0; i < seq_len; i++) {
            char current_char = input_sequence[i];
            if (!isalpha(current_char)) {
                continue;
            }
            process_character(&temp_machine, current_char);
        }
    }
    
    printf("Final state: %d\n", machine.current_state);
    printf("Processed count: %d\n", machine.processed_count);
    printf("Error count: %d\n", machine.error_count);
    
    return EXIT_SUCCESS;
}