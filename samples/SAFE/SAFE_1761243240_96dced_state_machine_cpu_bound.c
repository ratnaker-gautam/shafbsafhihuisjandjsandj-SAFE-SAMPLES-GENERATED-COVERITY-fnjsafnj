//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_STATES 8
#define MAX_TRANSITIONS 16
#define MAX_INPUTS 256

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

typedef struct {
    state_t current;
    char input;
    state_t next;
} transition_t;

typedef struct {
    state_t current_state;
    transition_t transitions[MAX_TRANSITIONS];
    int transition_count;
} state_machine_t;

void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_A;
    machine->transition_count = 0;
    
    transition_t trans[] = {
        {STATE_A, '0', STATE_B}, {STATE_A, '1', STATE_C},
        {STATE_B, '0', STATE_D}, {STATE_B, '1', STATE_E},
        {STATE_C, '0', STATE_F}, {STATE_C, '1', STATE_G},
        {STATE_D, '0', STATE_H}, {STATE_D, '1', STATE_A},
        {STATE_E, '0', STATE_B}, {STATE_E, '1', STATE_C},
        {STATE_F, '0', STATE_D}, {STATE_F, '1', STATE_E},
        {STATE_G, '0', STATE_F}, {STATE_G, '1', STATE_G},
        {STATE_H, '0', STATE_H}, {STATE_H, '1', STATE_A}
    };
    
    int count = sizeof(trans) / sizeof(trans[0]);
    if (count > MAX_TRANSITIONS) count = MAX_TRANSITIONS;
    
    for (int i = 0; i < count; i++) {
        if (machine->transition_count < MAX_TRANSITIONS) {
            machine->transitions[machine->transition_count] = trans[i];
            machine->transition_count++;
        }
    }
}

state_t process_input(state_machine_t *machine, char input) {
    for (int i = 0; i < machine->transition_count; i++) {
        if (machine->transitions[i].current == machine->current_state &&
            machine->transitions[i].input == input) {
            return machine->transitions[i].next;
        }
    }
    return machine->current_state;
}

const char* state_name(state_t state) {
    switch (state) {
        case STATE_A: return "A";
        case STATE_B: return "B";
        case STATE_C: return "C";
        case STATE_D: return "D";
        case STATE_E: return "E";
        case STATE_F: return "F";
        case STATE_G: return "G";
        case STATE_H: return "H";
        default: return "UNKNOWN";
    }
}

void simulate_machine(state_machine_t *machine, const char *input_sequence) {
    size_t len = strnlen(input_sequence, MAX_INPUTS);
    
    printf("Initial state: %s\n", state_name(machine->current_state));
    printf("Input sequence: %s\n", input_sequence);
    printf("State transitions:\n");
    
    for (size_t i = 0; i < len; i++) {
        char input = input_sequence[i];
        if (input != '0' && input != '1') {
            printf("Invalid input '%c' at position %zu, skipping\n", input, i);
            continue;
        }
        
        state_t old_state = machine->current_state;
        machine->current_state = process_input(machine, input);
        
        printf("  %s --%c--> %s\n", state_name(old_state), input, 
               state_name(machine->current_state));
    }
    
    printf("Final state: %s\n", state_name(machine->current_state));
}

int validate_binary_string(const char *str) {
    if (str == NULL) return 0;
    
    size_t len = strnlen(str, MAX_INPUTS);
    if (len == 0 || len >= MAX_INPUTS) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (str[i] != '0' && str[i] != '1') {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    char input_buffer[MAX_INPUTS + 1];
    
    printf("Enter binary input sequence (max %d characters, only 0s and 1s): ", MAX_INPUTS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strnlen(input_buffer, sizeof(input_buffer));
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!validate_binary_string(input_buffer)) {
        fprintf(stderr, "Invalid input: must contain only 0s and 1s, max %d characters\n", MAX_INPUTS);
        return EXIT_FAILURE;
    }
    
    simulate_machine(&machine, input_buffer);
    
    return EXIT_SUCCESS;
}