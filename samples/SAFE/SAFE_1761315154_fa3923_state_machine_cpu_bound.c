//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define STATE_COUNT 8
#define MAX_ITERATIONS 1000000

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_WAITING,
    STATE_COMPLETING,
    STATE_ERROR,
    STATE_RESET,
    STATE_FINAL
} state_t;

typedef struct {
    state_t current_state;
    uint64_t counter;
    uint32_t iterations;
    int error_code;
} state_machine_t;

static state_t state_transition(state_t current, uint64_t counter, int *error) {
    switch (current) {
        case STATE_IDLE:
            if (counter % 2 == 0) return STATE_READY;
            return STATE_IDLE;
        
        case STATE_READY:
            if (counter % 3 == 0) return STATE_PROCESSING;
            if (counter % 7 == 0) return STATE_ERROR;
            return STATE_READY;
        
        case STATE_PROCESSING:
            if (counter % 5 == 0) return STATE_WAITING;
            if (counter % 11 == 0) return STATE_ERROR;
            return STATE_PROCESSING;
        
        case STATE_WAITING:
            if (counter % 13 == 0) return STATE_COMPLETING;
            return STATE_WAITING;
        
        case STATE_COMPLETING:
            if (counter % 17 == 0) return STATE_FINAL;
            return STATE_COMPLETING;
        
        case STATE_ERROR:
            return STATE_RESET;
        
        case STATE_RESET:
            return STATE_IDLE;
        
        case STATE_FINAL:
            return STATE_FINAL;
        
        default:
            *error = 1;
            return STATE_ERROR;
    }
}

static void initialize_machine(state_machine_t *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->iterations = 0;
    machine->error_code = 0;
}

static const char* state_name(state_t state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_READY: return "READY";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_WAITING: return "WAITING";
        case STATE_COMPLETING: return "COMPLETING";
        case STATE_ERROR: return "ERROR";
        case STATE_RESET: return "RESET";
        case STATE_FINAL: return "FINAL";
        default: return "UNKNOWN";
    }
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Starting state machine simulation\n");
    
    while (machine.iterations < MAX_ITERATIONS) {
        state_t next_state = state_transition(machine.current_state, machine.counter, &machine.error_code);
        
        if (machine.error_code != 0) {
            printf("State transition error detected\n");
            break;
        }
        
        if (next_state != machine.current_state) {
            printf("Transition: %s -> %s (counter: %lu)\n", 
                   state_name(machine.current_state), 
                   state_name(next_state),
                   machine.counter);
        }
        
        machine.current_state = next_state;
        machine.counter++;
        machine.iterations++;
        
        if (machine.current_state == STATE_FINAL) {
            printf("Reached final state after %u iterations\n", machine.iterations);
            break;
        }
        
        if (machine.counter == UINT64_MAX) {
            printf("Counter overflow protection triggered\n");
            break;
        }
    }
    
    if (machine.iterations >= MAX_ITERATIONS) {
        printf("Reached maximum iterations (%u)\n", MAX_ITERATIONS);
    }
    
    printf("Final state: %s\n", state_name(machine.current_state));
    printf("Total iterations: %u\n", machine.iterations);
    printf("Final counter value: %lu\n", machine.counter);
    
    return machine.error_code;
}