//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define STATE_COUNT 8
#define MAX_ITERATIONS 1000000

typedef enum {
    STATE_START,
    STATE_PROCESSING,
    STATE_VALIDATING,
    STATE_COMPUTING,
    STATE_AGGREGATING,
    STATE_FINALIZING,
    STATE_ERROR,
    STATE_DONE
} state_t;

typedef struct {
    state_t current_state;
    uint64_t counter;
    uint32_t iterations;
    int error_code;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_START;
    machine->counter = 0;
    machine->iterations = 0;
    machine->error_code = 0;
}

static state_t transition_state(state_machine_t *machine) {
    switch (machine->current_state) {
        case STATE_START:
            return STATE_PROCESSING;
        case STATE_PROCESSING:
            if (machine->counter % 2 == 0) {
                return STATE_VALIDATING;
            } else {
                return STATE_COMPUTING;
            }
        case STATE_VALIDATING:
            if (machine->counter % 3 == 0) {
                return STATE_AGGREGATING;
            } else {
                return STATE_PROCESSING;
            }
        case STATE_COMPUTING:
            return STATE_AGGREGATING;
        case STATE_AGGREGATING:
            if (machine->counter > 1000) {
                return STATE_FINALIZING;
            } else {
                return STATE_PROCESSING;
            }
        case STATE_FINALIZING:
            return STATE_DONE;
        case STATE_ERROR:
            return STATE_ERROR;
        case STATE_DONE:
            return STATE_DONE;
        default:
            return STATE_ERROR;
    }
}

static void execute_state(state_machine_t *machine) {
    uint64_t temp;
    
    switch (machine->current_state) {
        case STATE_START:
            machine->counter = 1;
            break;
        case STATE_PROCESSING:
            if (machine->counter > UINT64_MAX - 7) {
                machine->error_code = 1;
                machine->current_state = STATE_ERROR;
                return;
            }
            machine->counter += 7;
            break;
        case STATE_VALIDATING:
            temp = machine->counter * 3;
            if (temp / 3 != machine->counter) {
                machine->error_code = 2;
                machine->current_state = STATE_ERROR;
                return;
            }
            machine->counter = temp;
            break;
        case STATE_COMPUTING:
            if (machine->counter > UINT64_MAX / 2) {
                machine->error_code = 3;
                machine->current_state = STATE_ERROR;
                return;
            }
            machine->counter *= 2;
            break;
        case STATE_AGGREGATING:
            if (machine->counter > UINT64_MAX - machine->iterations) {
                machine->error_code = 4;
                machine->current_state = STATE_ERROR;
                return;
            }
            machine->counter += machine->iterations;
            break;
        case STATE_FINALIZING:
            machine->counter = machine->counter % 1000000;
            break;
        case STATE_ERROR:
            break;
        case STATE_DONE:
            break;
    }
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Starting state machine simulation\n");
    
    while (machine.current_state != STATE_DONE && 
           machine.current_state != STATE_ERROR &&
           machine.iterations < MAX_ITERATIONS) {
        
        execute_state(&machine);
        
        if (machine.current_state == STATE_ERROR) {
            printf("Error occurred: code %d\n", machine.error_code);
            return 1;
        }
        
        state_t next_state = transition_state(&machine);
        machine.current_state = next_state;
        machine.iterations++;
        
        if (machine.iterations % 100000 == 0) {
            printf("Iteration: %u, State: %d, Counter: %lu\n", 
                   machine.iterations, machine.current_state, machine.counter);
        }
    }
    
    if (machine.iterations >= MAX_ITERATIONS) {
        printf("Maximum iterations reached\n");
        return 1;
    }
    
    printf("Final counter value: %lu\n", machine.counter);
    printf("Total iterations: %u\n", machine.iterations);
    
    return 0;
}