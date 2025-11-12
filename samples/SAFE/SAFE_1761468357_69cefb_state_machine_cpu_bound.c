//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

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
            if (machine->counter % 3 == 0) {
                return STATE_PROCESSING;
            } else {
                return STATE_WAITING;
            }
        
        case STATE_PROCESSING:
            if (machine->counter % 7 == 0) {
                return STATE_COMPLETING;
            } else if (machine->counter % 13 == 0) {
                return STATE_ERROR;
            } else {
                return STATE_PROCESSING;
            }
        
        case STATE_WAITING:
            if (machine->counter % 5 == 0) {
                return STATE_PROCESSING;
            } else {
                return STATE_WAITING;
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
            machine->counter += 2;
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

static void print_state_info(const state_machine_t *machine) {
    const char *state_names[] = {
        "IDLE",
        "PROCESSING",
        "WAITING",
        "COMPLETING",
        "ERROR"
    };
    
    if (machine->current_state < STATE_COUNT) {
        printf("State: %-12s Counter: %10lu Iteration: %6u\n",
               state_names[machine->current_state],
               machine->counter,
               machine->iterations);
    }
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Starting state machine simulation...\n");
    printf("Maximum iterations: %d\n\n", MAX_ITERATIONS);
    
    while (machine.iterations < MAX_ITERATIONS) {
        print_state_info(&machine);
        
        process_state(&machine);
        machine.current_state = transition_state(&machine);
        machine.iterations++;
        
        if (machine.counter > UINT64_MAX / 3) {
            machine.counter = 1;
        }
        
        if (machine.iterations % 100000 == 0) {
            printf("Progress: %u/%d iterations completed\n", 
                   machine.iterations, MAX_ITERATIONS);
        }
    }
    
    printf("\nFinal state: ");
    print_state_info(&machine);
    printf("Simulation completed successfully.\n");
    
    return 0;
}