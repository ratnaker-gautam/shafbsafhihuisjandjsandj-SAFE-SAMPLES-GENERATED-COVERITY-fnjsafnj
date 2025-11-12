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

static state_t process_state(state_machine_t *machine) {
    switch (machine->current_state) {
        case STATE_IDLE:
            if (machine->counter % 2 == 0) {
                return STATE_PROCESSING;
            } else {
                return STATE_WAITING;
            }
            
        case STATE_PROCESSING:
            machine->counter += 3;
            if (machine->counter > 1000) {
                return STATE_COMPLETING;
            } else if (machine->counter % 7 == 0) {
                return STATE_ERROR;
            } else {
                return STATE_WAITING;
            }
            
        case STATE_WAITING:
            machine->counter += 1;
            if (machine->counter % 11 == 0) {
                return STATE_PROCESSING;
            } else if (machine->counter > 5000) {
                return STATE_COMPLETING;
            } else {
                return STATE_IDLE;
            }
            
        case STATE_COMPLETING:
            if (machine->counter < 100) {
                return STATE_PROCESSING;
            } else {
                return STATE_IDLE;
            }
            
        case STATE_ERROR:
            machine->counter = 0;
            return STATE_IDLE;
            
        default:
            return STATE_ERROR;
    }
}

static void run_state_machine(state_machine_t *machine) {
    while (machine->iterations < MAX_ITERATIONS) {
        state_t next_state = process_state(machine);
        
        if (next_state >= STATE_COUNT || next_state < 0) {
            machine->current_state = STATE_ERROR;
        } else {
            machine->current_state = next_state;
        }
        
        machine->iterations++;
        
        if (machine->current_state == STATE_COMPLETING && machine->counter > 10000) {
            break;
        }
        
        if (machine->counter > UINT64_MAX - 1000) {
            machine->current_state = STATE_ERROR;
            break;
        }
    }
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    run_state_machine(&machine);
    
    printf("Final state: %d\n", machine.current_state);
    printf("Counter value: %lu\n", machine.counter);
    printf("Iterations: %u\n", machine.iterations);
    
    return 0;
}