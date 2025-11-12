//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define STATE_COUNT 8
#define MAX_STEPS 1000000

typedef enum {
    STATE_IDLE,
    STATE_PROCESSING,
    STATE_WAITING,
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_RESET,
    STATE_VALIDATE,
    STATE_FINALIZE
} system_state_t;

typedef struct {
    system_state_t current_state;
    uint64_t counter;
    uint32_t iterations;
    int error_code;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->iterations = 0;
    machine->error_code = 0;
}

static system_state_t state_transition(state_machine_t *machine) {
    switch (machine->current_state) {
        case STATE_IDLE:
            if (machine->counter < 1000) {
                return STATE_PROCESSING;
            }
            return STATE_ERROR;
            
        case STATE_PROCESSING:
            if (machine->counter % 2 == 0) {
                return STATE_WAITING;
            } else if (machine->counter > 5000) {
                return STATE_COMPLETE;
            }
            return STATE_PROCESSING;
            
        case STATE_WAITING:
            if (machine->counter % 3 == 0) {
                return STATE_VALIDATE;
            }
            return STATE_WAITING;
            
        case STATE_VALIDATE:
            if (machine->counter < 10000) {
                return STATE_PROCESSING;
            }
            return STATE_FINALIZE;
            
        case STATE_FINALIZE:
            return STATE_COMPLETE;
            
        case STATE_COMPLETE:
            return STATE_RESET;
            
        case STATE_RESET:
            return STATE_IDLE;
            
        case STATE_ERROR:
            return STATE_RESET;
            
        default:
            return STATE_ERROR;
    }
}

static void execute_state_action(state_machine_t *machine) {
    switch (machine->current_state) {
        case STATE_IDLE:
            machine->counter = 0;
            break;
            
        case STATE_PROCESSING:
            for (int i = 0; i < 100; i++) {
                machine->counter += (i * 3) % 7;
            }
            break;
            
        case STATE_WAITING:
            machine->counter += (machine->counter * 2) % 1000;
            break;
            
        case STATE_VALIDATE:
            if (machine->counter > 20000) {
                machine->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_FINALIZE:
            machine->counter = machine->counter * 3 + 1;
            break;
            
        case STATE_COMPLETE:
            break;
            
        case STATE_RESET:
            machine->counter = 0;
            machine->error_code = 0;
            break;
            
        case STATE_ERROR:
            machine->error_code = 1;
            break;
    }
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Starting state machine simulation\n");
    
    for (uint32_t step = 0; step < MAX_STEPS; step++) {
        machine.iterations++;
        
        execute_state_action(&machine);
        
        system_state_t next_state = state_transition(&machine);
        
        if (next_state >= STATE_COUNT || next_state < 0) {
            printf("Invalid state transition detected\n");
            return 1;
        }
        
        machine.current_state = next_state;
        
        if (machine.current_state == STATE_COMPLETE) {
            printf("Simulation completed successfully after %u iterations\n", 
                   machine.iterations);
            printf("Final counter value: %lu\n", machine.counter);
            break;
        }
        
        if (machine.error_code != 0) {
            printf("Error detected at iteration %u\n", machine.iterations);
            printf("Counter value: %lu\n", machine.counter);
            break;
        }
        
        if (step == MAX_STEPS - 1) {
            printf("Maximum steps reached without completion\n");
            printf("Final state: %d, Counter: %lu\n", 
                   machine.current_state, machine.counter);
        }
    }
    
    return 0;
}