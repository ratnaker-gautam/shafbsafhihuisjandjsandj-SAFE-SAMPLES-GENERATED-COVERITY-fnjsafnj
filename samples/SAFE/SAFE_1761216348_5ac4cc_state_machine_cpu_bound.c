//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define STATE_COUNT 5
#define MAX_ITERATIONS 1000000

typedef enum {
    STATE_IDLE,
    STATE_PROCESSING,
    STATE_WAITING,
    STATE_COMPLETING,
    STATE_ERROR
} state_t;

typedef struct {
    state_t current_state;
    uint64_t counter;
    uint32_t iterations;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->iterations = 0;
}

static state_t transition_state(state_machine_t *machine) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            if (machine->counter % 2 == 0) {
                return STATE_PROCESSING;
            } else {
                return STATE_WAITING;
            }
            
        case STATE_PROCESSING:
            if (machine->counter % 3 == 0) {
                return STATE_COMPLETING;
            } else if (machine->counter % 7 == 0) {
                return STATE_ERROR;
            } else {
                return STATE_WAITING;
            }
            
        case STATE_WAITING:
            if (machine->counter % 5 == 0) {
                return STATE_PROCESSING;
            } else {
                return STATE_COMPLETING;
            }
            
        case STATE_COMPLETING:
            return STATE_IDLE;
            
        case STATE_ERROR:
            return STATE_IDLE;
            
        default:
            return STATE_ERROR;
    }
}

static void process_state(state_machine_t *machine) {
    if (machine == NULL) return;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            machine->counter += 1;
            break;
            
        case STATE_PROCESSING:
            machine->counter = (machine->counter * 3) % UINT64_MAX;
            break;
            
        case STATE_WAITING:
            machine->counter = (machine->counter + 7) % UINT64_MAX;
            break;
            
        case STATE_COMPLETING:
            machine->counter = (machine->counter / 2) % UINT64_MAX;
            break;
            
        case STATE_ERROR:
            machine->counter = 0;
            break;
            
        default:
            machine->counter = 0;
            break;
    }
}

static void print_state_info(const state_machine_t *machine) {
    if (machine == NULL) return;
    
    const char *state_names[] = {
        "IDLE",
        "PROCESSING", 
        "WAITING",
        "COMPLETING",
        "ERROR"
    };
    
    if (machine->current_state < STATE_COUNT) {
        printf("State: %-12s Counter: %lu\n", 
               state_names[machine->current_state], 
               (unsigned long)machine->counter);
    }
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Starting state machine simulation...\n");
    
    for (machine.iterations = 0; machine.iterations < MAX_ITERATIONS; machine.iterations++) {
        print_state_info(&machine);
        
        state_t new_state = transition_state(&machine);
        if (new_state >= STATE_COUNT) {
            printf("Invalid state transition detected\n");
            return EXIT_FAILURE;
        }
        
        machine.current_state = new_state;
        process_state(&machine);
        
        if (machine.iterations > 0 && machine.iterations % 100000 == 0) {
            printf("Progress: %u/%d iterations completed\n", 
                   machine.iterations, MAX_ITERATIONS);
        }
    }
    
    printf("Final state: ");
    print_state_info(&machine);
    printf("Simulation completed after %u iterations\n", machine.iterations);
    
    return EXIT_SUCCESS;
}