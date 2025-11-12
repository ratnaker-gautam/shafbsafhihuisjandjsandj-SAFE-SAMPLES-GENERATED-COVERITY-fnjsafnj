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
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->iterations = 0;
}

static state_t transition_state(state_machine_t *machine) {
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
    switch (machine->current_state) {
        case STATE_IDLE:
            machine->counter += 1;
            break;
            
        case STATE_PROCESSING:
            machine->counter = machine->counter * 3 + 1;
            break;
            
        case STATE_WAITING:
            machine->counter = machine->counter * 2 + 1;
            break;
            
        case STATE_COMPLETING:
            machine->counter = machine->counter / 2;
            break;
            
        case STATE_ERROR:
            machine->counter = 0;
            break;
            
        default:
            machine->counter = 0;
            break;
    }
}

static int validate_transition(state_machine_t *machine) {
    if (machine->iterations >= MAX_ITERATIONS) {
        return 0;
    }
    
    if (machine->current_state >= STATE_COUNT) {
        return 0;
    }
    
    return 1;
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Starting state machine simulation\n");
    
    while (machine.iterations < MAX_ITERATIONS) {
        if (!validate_transition(&machine)) {
            break;
        }
        
        state_t next_state = transition_state(&machine);
        
        if (next_state >= STATE_COUNT) {
            machine.current_state = STATE_ERROR;
        } else {
            machine.current_state = next_state;
        }
        
        process_state(&machine);
        machine.iterations++;
        
        if (machine.iterations % 100000 == 0) {
            printf("Iteration: %u, State: %d, Counter: %lu\n", 
                   machine.iterations, machine.current_state, machine.counter);
        }
        
        if (machine.counter > UINT64_MAX / 4) {
            machine.counter = machine.counter / 2;
        }
    }
    
    printf("Final state: %d, Iterations: %u, Counter: %lu\n", 
           machine.current_state, machine.iterations, machine.counter);
    
    return 0;
}