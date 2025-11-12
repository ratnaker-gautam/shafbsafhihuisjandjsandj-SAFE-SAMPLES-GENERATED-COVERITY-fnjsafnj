//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_STATES 8
#define MAX_INPUTS 16
#define MAX_TRANSITIONS 64

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H
} state_t;

typedef enum {
    INPUT_0,
    INPUT_1,
    INPUT_2,
    INPUT_3,
    INPUT_INVALID
} input_t;

typedef struct {
    state_t current;
    input_t input;
    state_t next;
} transition_t;

typedef struct {
    state_t current_state;
    transition_t transitions[MAX_TRANSITIONS];
    size_t transition_count;
} state_machine_t;

static input_t parse_input(char c) {
    switch (c) {
        case '0': return INPUT_0;
        case '1': return INPUT_1;
        case '2': return INPUT_2;
        case '3': return INPUT_3;
        default: return INPUT_INVALID;
    }
}

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_A;
    machine->transition_count = 0;
    
    transition_t trans[] = {
        {STATE_A, INPUT_0, STATE_B}, {STATE_A, INPUT_1, STATE_C},
        {STATE_B, INPUT_0, STATE_D}, {STATE_B, INPUT_1, STATE_E},
        {STATE_C, INPUT_0, STATE_F}, {STATE_C, INPUT_1, STATE_G},
        {STATE_D, INPUT_2, STATE_H}, {STATE_D, INPUT_3, STATE_A},
        {STATE_E, INPUT_2, STATE_B}, {STATE_E, INPUT_3, STATE_C},
        {STATE_F, INPUT_2, STATE_D}, {STATE_F, INPUT_3, STATE_E},
        {STATE_G, INPUT_2, STATE_F}, {STATE_G, INPUT_3, STATE_G},
        {STATE_H, INPUT_0, STATE_H}, {STATE_H, INPUT_1, STATE_A}
    };
    
    size_t count = sizeof(trans) / sizeof(trans[0]);
    if (count > MAX_TRANSITIONS) {
        count = MAX_TRANSITIONS;
    }
    
    for (size_t i = 0; i < count; i++) {
        if (machine->transition_count < MAX_TRANSITIONS) {
            machine->transitions[machine->transition_count] = trans[i];
            machine->transition_count++;
        }
    }
}

static state_t process_transition(state_machine_t *machine, input_t input) {
    for (size_t i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current == machine->current_state &&
            machine->transitions[i].input == input) {
            return machine->transitions[i].next;
        }
    }
    return machine->current_state;
}

static void print_state(state_t state) {
    const char *names[] = {
        "STATE_A", "STATE_B", "STATE_C", "STATE_D",
        "STATE_E", "STATE_F", "STATE_G", "STATE_H"
    };
    
    if (state < MAX_STATES) {
        printf("%s\n", names[state]);
    }
}

static void simulate_state_machine(state_machine_t *machine, const char *input_sequence) {
    size_t len = strlen(input_sequence);
    if (len > MAX_INPUTS) {
        len = MAX_INPUTS;
    }
    
    printf("Initial state: ");
    print_state(machine->current_state);
    
    for (size_t i = 0; i < len; i++) {
        input_t input = parse_input(input_sequence[i]);
        if (input == INPUT_INVALID) {
            printf("Invalid input '%c' at position %zu\n", input_sequence[i], i);
            continue;
        }
        
        state_t next_state = process_transition(machine, input);
        printf("Input '%c': ", input_sequence[i]);
        print_state(next_state);
        machine->current_state = next_state;
    }
}

int main(void) {
    state_machine_t machine;
    char input_buffer[MAX_INPUTS + 2];
    
    initialize_machine(&machine);
    
    printf("Enter input sequence (0-3, max %d characters): ", MAX_INPUTS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        fprintf(stderr, "Empty input sequence\n");
        return 1;
    }
    
    simulate_state_machine(&machine, input_buffer);
    
    return 0;
}