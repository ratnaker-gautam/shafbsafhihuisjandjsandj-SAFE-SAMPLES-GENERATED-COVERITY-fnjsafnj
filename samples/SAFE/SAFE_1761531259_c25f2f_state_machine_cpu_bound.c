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
                return STATE_COMPUTING;
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
    switch (machine->current_state) {
        case STATE_START:
            machine->counter = 1;
            break;
        case STATE_PROCESSING:
            for (int i = 0; i < 100; i++) {
                machine->counter += i * 2;
            }
            break;
        case STATE_VALIDATING:
            if (machine->counter > UINT64_MAX - 1000) {
                machine->error_code = 1;
                machine->current_state = STATE_ERROR;
                return;
            }
            machine->counter += 1000;
            break;
        case STATE_COMPUTING:
            for (int i = 0; i < 50; i++) {
                machine->counter = (machine->counter * 3 + 7) % 1000000;
            }
            break;
        case STATE_AGGREGATING:
            machine->counter += machine->counter / 2;
            break;
        case STATE_FINALIZING:
            machine->counter = machine->counter % 1000000007;
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
            break;
        }

        state_t next_state = transition_state(&machine);
        
        if (next_state == STATE_ERROR) {
            machine.error_code = 2;
            machine.current_state = STATE_ERROR;
            break;
        }
        
        machine.current_state = next_state;
        machine.iterations++;
        
        if (machine.iterations % 10000 == 0) {
            printf("Iteration: %u, State: %d, Counter: %lu\n", 
                   machine.iterations, machine.current_state, machine.counter);
        }
    }

    if (machine.current_state == STATE_ERROR) {
        printf("State machine terminated with error: %d\n", machine.error_code);
        return 1;
    } else if (machine.iterations >= MAX_ITERATIONS) {
        printf("State machine reached maximum iterations\n");
        return 2;
    } else {
        printf("State machine completed successfully\n");
        printf("Final counter value: %lu\n", machine.counter);
        printf("Total iterations: %u\n", machine.iterations);
        return 0;
    }
}