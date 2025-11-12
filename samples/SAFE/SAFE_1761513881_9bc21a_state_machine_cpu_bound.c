//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define STATE_COUNT 8
#define MAX_ITERATIONS 1000000

typedef enum {
    STATE_IDLE,
    STATE_PROCESSING,
    STATE_WAITING,
    STATE_COMPLETING,
    STATE_ERROR,
    STATE_RESET,
    STATE_CALIBRATING,
    STATE_SHUTDOWN
} system_state_t;

typedef struct {
    system_state_t current_state;
    uint64_t iteration_count;
    uint32_t error_count;
    uint32_t success_count;
    double accumulator;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_IDLE;
    machine->iteration_count = 0;
    machine->error_count = 0;
    machine->success_count = 0;
    machine->accumulator = 0.0;
}

static system_state_t process_state_transition(state_machine_t *machine) {
    double computation;
    uint32_t decision;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            computation = machine->accumulator * 1.1;
            if (computation > 1000.0) {
                return STATE_PROCESSING;
            }
            machine->accumulator += 0.5;
            return STATE_IDLE;
            
        case STATE_PROCESSING:
            for (int i = 0; i < 100; i++) {
                machine->accumulator += (i * 0.01);
            }
            decision = (uint32_t)(machine->accumulator) % 10;
            if (decision < 2) {
                return STATE_ERROR;
            } else if (decision < 6) {
                return STATE_WAITING;
            }
            return STATE_COMPLETING;
            
        case STATE_WAITING:
            machine->accumulator *= 0.95;
            if (machine->accumulator < 50.0) {
                return STATE_IDLE;
            }
            return STATE_PROCESSING;
            
        case STATE_COMPLETING:
            machine->success_count++;
            machine->accumulator = 0.0;
            return STATE_IDLE;
            
        case STATE_ERROR:
            machine->error_count++;
            if (machine->error_count > 5) {
                return STATE_RESET;
            }
            return STATE_IDLE;
            
        case STATE_RESET:
            machine->accumulator = 10.0;
            machine->error_count = 0;
            return STATE_CALIBRATING;
            
        case STATE_CALIBRATING:
            for (int i = 0; i < 50; i++) {
                machine->accumulator += (i * 0.02);
            }
            return STATE_IDLE;
            
        case STATE_SHUTDOWN:
        default:
            return STATE_SHUTDOWN;
    }
}

static void run_state_machine(state_machine_t *machine) {
    while (machine->iteration_count < MAX_ITERATIONS) {
        if (machine->current_state == STATE_SHUTDOWN) {
            break;
        }
        
        machine->current_state = process_state_transition(machine);
        machine->iteration_count++;
        
        if (machine->iteration_count % 100000 == 0) {
            printf("Iteration: %lu, State: %d, Success: %u, Errors: %u\n",
                   machine->iteration_count, machine->current_state,
                   machine->success_count, machine->error_count);
        }
    }
}

int main(void) {
    state_machine_t machine;
    
    initialize_machine(&machine);
    run_state_machine(&machine);
    
    printf("Final results - Iterations: %lu, Success: %u, Errors: %u, Final accumulator: %.2f\n",
           machine.iteration_count, machine.success_count, 
           machine.error_count, machine.accumulator);
    
    return 0;
}