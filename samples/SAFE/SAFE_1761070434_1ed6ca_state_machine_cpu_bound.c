//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    int error_code;
} state_machine_t;

void initialize_machine(state_machine_t *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->iterations = 0;
    machine->error_code = 0;
}

state_t transition_state(state_machine_t *machine) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            if (machine->counter % 3 == 0) {
                return STATE_PROCESSING;
            }
            return STATE_WAITING;
            
        case STATE_PROCESSING:
            if (machine->counter % 7 == 0) {
                return STATE_COMPLETING;
            }
            if (machine->counter % 13 == 0) {
                return STATE_ERROR;
            }
            return STATE_WAITING;
            
        case STATE_WAITING:
            if (machine->counter % 5 == 0) {
                return STATE_PROCESSING;
            }
            return STATE_IDLE;
            
        case STATE_COMPLETING:
            return STATE_IDLE;
            
        case STATE_ERROR:
            return STATE_IDLE;
            
        default:
            return STATE_ERROR;
    }
}

void execute_state_action(state_machine_t *machine) {
    if (machine == NULL) return;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            machine->counter += 1;
            break;
            
        case STATE_PROCESSING:
            if (machine->counter < UINT64_MAX - 17) {
                machine->counter += 17;
            }
            break;
            
        case STATE_WAITING:
            if (machine->counter < UINT64_MAX - 3) {
                machine->counter += 3;
            }
            break;
            
        case STATE_COMPLETING:
            if (machine->counter > 100) {
                machine->counter -= 100;
            }
            break;
            
        case STATE_ERROR:
            machine->error_code = (int)(machine->counter % 256);
            machine->counter = 0;
            break;
            
        default:
            machine->error_code = 255;
            break;
    }
}

int run_state_machine(state_machine_t *machine, uint32_t max_iterations) {
    if (machine == NULL || max_iterations == 0 || max_iterations > MAX_ITERATIONS) {
        return -1;
    }
    
    initialize_machine(machine);
    
    for (machine->iterations = 0; machine->iterations < max_iterations; machine->iterations++) {
        state_t next_state = transition_state(machine);
        
        if (next_state >= STATE_COUNT) {
            machine->error_code = 254;
            return -1;
        }
        
        machine->current_state = next_state;
        execute_state_action(machine);
        
        if (machine->current_state == STATE_ERROR && machine->error_code != 0) {
            break;
        }
    }
    
    return 0;
}

int main() {
    state_machine_t machine;
    uint32_t iterations;
    
    printf("Enter number of iterations (1-%u): ", MAX_ITERATIONS);
    if (scanf("%u", &iterations) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (iterations == 0 || iterations > MAX_ITERATIONS) {
        printf("Invalid iteration count\n");
        return 1;
    }
    
    int result = run_state_machine(&machine, iterations);
    
    if (result != 0) {
        printf("State machine execution failed with error code: %d\n", machine.error_code);
        return 1;
    }
    
    printf("Final state: %d\n", machine.current_state);
    printf("Final counter: %lu\n", machine.counter);
    printf("Iterations completed: %u\n", machine.iterations);
    printf("Error code: %d\n", machine.error_code);
    
    return 0;
}