//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define STATE_COUNT 8
#define MAX_STEPS 1000000

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
    uint64_t counter;
    uint32_t transitions;
} state_machine_t;

static state_t transition_function(state_t current, uint64_t counter) {
    switch (current) {
        case STATE_A:
            return (counter % 3 == 0) ? STATE_B : STATE_C;
        case STATE_B:
            return (counter % 5 == 0) ? STATE_D : STATE_E;
        case STATE_C:
            return (counter % 7 == 0) ? STATE_F : STATE_G;
        case STATE_D:
            return (counter % 11 == 0) ? STATE_H : STATE_A;
        case STATE_E:
            return (counter % 13 == 0) ? STATE_B : STATE_C;
        case STATE_F:
            return (counter % 17 == 0) ? STATE_D : STATE_E;
        case STATE_G:
            return (counter % 19 == 0) ? STATE_F : STATE_G;
        case STATE_H:
            return (counter % 23 == 0) ? STATE_H : STATE_A;
        default:
            return STATE_A;
    }
}

static void initialize_machine(state_machine_t *machine) {
    if (machine == NULL) return;
    machine->current = STATE_A;
    machine->counter = 0;
    machine->transitions = 0;
}

static void run_simulation(state_machine_t *machine, uint32_t max_steps) {
    if (machine == NULL || max_steps == 0 || max_steps > MAX_STEPS) return;
    
    for (uint32_t i = 0; i < max_steps; i++) {
        state_t next_state = transition_function(machine->current, machine->counter);
        machine->current = next_state;
        machine->counter++;
        machine->transitions++;
        
        if (machine->counter == UINT64_MAX) {
            break;
        }
    }
}

static const char* state_to_string(state_t state) {
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

int main(void) {
    state_machine_t machine;
    char input_buffer[32];
    uint32_t steps;
    
    printf("Enter number of simulation steps (1-%u): ", MAX_STEPS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (sscanf(input_buffer, "%u", &steps) != 1) {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (steps == 0 || steps > MAX_STEPS) {
        fprintf(stderr, "Steps must be between 1 and %u\n", MAX_STEPS);
        return 1;
    }
    
    initialize_machine(&machine);
    
    printf("Starting state: %s\n", state_to_string(machine.current));
    printf("Running simulation for %u steps...\n", steps);
    
    run_simulation(&machine, steps);
    
    printf("Final state: %s\n", state_to_string(machine.current));
    printf("Total transitions: %u\n", machine.transitions);
    printf("Final counter: %lu\n", machine.counter);
    
    return 0;
}