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
    state_t current_state;
    uint64_t counter;
    uint32_t transitions;
} state_machine_t;

static state_t get_next_state(state_t current, uint64_t counter) {
    switch (current) {
        case STATE_A:
            return (counter % 3 == 0) ? STATE_B : STATE_C;
        case STATE_B:
            return (counter % 5 == 0) ? STATE_D : STATE_E;
        case STATE_C:
            return (counter % 7 == 0) ? STATE_F : STATE_G;
        case STATE_D:
            return (counter % 2 == 0) ? STATE_H : STATE_A;
        case STATE_E:
            return (counter % 11 == 0) ? STATE_B : STATE_C;
        case STATE_F:
            return (counter % 13 == 0) ? STATE_D : STATE_E;
        case STATE_G:
            return (counter % 17 == 0) ? STATE_F : STATE_G;
        case STATE_H:
            return (counter % 19 == 0) ? STATE_H : STATE_A;
        default:
            return STATE_A;
    }
}

static void initialize_machine(state_machine_t *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_A;
    machine->counter = 0;
    machine->transitions = 0;
}

static void run_state_machine(state_machine_t *machine, uint32_t max_steps) {
    if (machine == NULL || max_steps == 0) return;
    
    for (uint32_t i = 0; i < max_steps; i++) {
        state_t next_state = get_next_state(machine->current_state, machine->counter);
        
        if (next_state != machine->current_state) {
            machine->transitions++;
        }
        
        machine->current_state = next_state;
        machine->counter++;
        
        if (machine->counter == UINT64_MAX) {
            break;
        }
    }
}

static void print_results(const state_machine_t *machine) {
    if (machine == NULL) return;
    
    printf("Final state: ");
    switch (machine->current_state) {
        case STATE_A: printf("A"); break;
        case STATE_B: printf("B"); break;
        case STATE_C: printf("C"); break;
        case STATE_D: printf("D"); break;
        case STATE_E: printf("E"); break;
        case STATE_F: printf("F"); break;
        case STATE_G: printf("G"); break;
        case STATE_H: printf("H"); break;
    }
    
    printf("\nTotal transitions: %u\n", machine->transitions);
    printf("Counter value: %lu\n", machine->counter);
}

int main(void) {
    state_machine_t machine;
    
    initialize_machine(&machine);
    
    printf("Running state machine for %d steps...\n", MAX_STEPS);
    run_state_machine(&machine, MAX_STEPS);
    
    print_results(&machine);
    
    return 0;
}