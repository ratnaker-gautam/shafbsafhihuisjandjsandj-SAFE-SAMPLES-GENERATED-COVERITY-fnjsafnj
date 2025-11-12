//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    switch(state) {
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

void validate_input_string(const char *input) {
    if (input == NULL) {
        fprintf(stderr, "Error: Input string is NULL\n");
        exit(EXIT_FAILURE);
    }
    
    size_t len = strlen(input);
    if (len == 0) {
        fprintf(stderr, "Error: Input string is empty\n");
        exit(EXIT_FAILURE);
    }
    
    if (len > MAX_INPUTS) {
        fprintf(stderr, "Error: Input string too long\n");
        exit(EXIT_FAILURE);
    }
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] != '0' && input[i] != '1') {
            fprintf(stderr, "Error: Invalid input character '%c'\n", input[i]);
            exit(EXIT_FAILURE);
        }
    }
}

void cpu_intensive_processing(state_machine_t *machine, const char *input) {
    int iterations = 1000;
    
    for (int iter = 0; iter < iterations; iter++) {
        state_machine_t temp_machine = *machine;
        
        for (size_t i = 0; input[i] != '\0'; i++) {
            temp_machine.current_state = process_input(&temp_machine, input[i]);
            
            for (int j = 0; j < 100; j++) {
                volatile int computation = i * j + iter;
                (void)computation;
            }
        }
        
        for (int i = 0; i < 50; i++) {
            volatile double result = 1.0;
            for (int j = 1; j <= 10; j++) {
                result *= (double)j;
            }
            (void)result;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <binary_string>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    validate_input_string(argv[1]);
    
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Initial state: %s\n", state_name(machine.current_state));
    printf("Processing input: %s\n", argv[1]);
    
    cpu_intensive_processing(&machine, argv[1]);
    
    for (size_t i = 0; argv[1][i] != '\0'; i++) {
        machine.current_state = process_input(&machine, argv[1][i]);
        printf("After '%c': state %s\n", argv[1][i], state_name(machine.current_state));
    }
    
    printf("Final state: