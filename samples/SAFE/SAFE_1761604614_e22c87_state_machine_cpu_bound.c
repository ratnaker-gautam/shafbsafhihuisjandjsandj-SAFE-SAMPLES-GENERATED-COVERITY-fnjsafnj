//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define STATE_COUNT 8
#define MAX_STEPS 1000000

typedef enum {
    STATE_IDLE,
    STATE_PROCESSING,
    STATE_WAITING,
    STATE_COMPLETING,
    STATE_ERROR,
    STATE_RESET,
    STATE_VALIDATING,
    STATE_FINAL
} state_t;

typedef struct {
    state_t current_state;
    uint64_t counter;
    uint32_t steps;
    uint32_t max_iterations;
} state_machine_t;

static state_t state_transition(state_t current, uint64_t counter) {
    switch (current) {
        case STATE_IDLE:
            return (counter % 7 == 0) ? STATE_PROCESSING : STATE_VALIDATING;
        case STATE_PROCESSING:
            return (counter % 11 == 0) ? STATE_WAITING : STATE_COMPLETING;
        case STATE_WAITING:
            return (counter % 13 == 0) ? STATE_COMPLETING : STATE_PROCESSING;
        case STATE_COMPLETING:
            return (counter % 17 == 0) ? STATE_FINAL : STATE_IDLE;
        case STATE_ERROR:
            return STATE_RESET;
        case STATE_RESET:
            return STATE_IDLE;
        case STATE_VALIDATING:
            return (counter % 19 == 0) ? STATE_ERROR : STATE_PROCESSING;
        case STATE_FINAL:
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

static const char* state_name(state_t state) {
    static const char* names[] = {
        "IDLE", "PROCESSING", "WAITING", "COMPLETING",
        "ERROR", "RESET", "VALIDATING", "FINAL"
    };
    if (state < STATE_IDLE || state > STATE_FINAL) return "UNKNOWN";
    return names[state];
}

static void initialize_machine(state_machine_t* machine, uint32_t iterations) {
    if (machine == NULL) return;
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->steps = 0;
    machine->max_iterations = (iterations > 0 && iterations <= MAX_STEPS) ? iterations : 1000;
}

static int run_state_machine(state_machine_t* machine) {
    if (machine == NULL) return -1;
    
    printf("Starting state machine with %u iterations\n", machine->max_iterations);
    
    while (machine->steps < machine->max_iterations) {
        state_t next_state = state_transition(machine->current_state, machine->counter);
        
        if (machine->steps % 100000 == 0) {
            printf("Step %u: %s -> %s (counter: %lu)\n",
                   machine->steps,
                   state_name(machine->current_state),
                   state_name(next_state),
                   machine->counter);
        }
        
        machine->current_state = next_state;
        machine->counter++;
        machine->steps++;
        
        if (machine->current_state == STATE_FINAL) {
            printf("Reached FINAL state at step %u\n", machine->steps);
            break;
        }
        
        if (machine->counter == UINT64_MAX) {
            printf("Counter overflow protection\n");
            break;
        }
    }
    
    printf("Completed %u steps. Final state: %s\n", machine->steps, state_name(machine->current_state));
    return 0;
}

int main(int argc, char* argv[]) {
    uint32_t iterations = 1000;
    
    if (argc > 1) {
        long input_val = strtol(argv[1], NULL, 10);
        if (input_val > 0 && input_val <= MAX_STEPS) {
            iterations = (uint32_t)input_val;
        } else {
            printf("Invalid iteration count. Using default: 1000\n");
        }
    }
    
    state_machine_t machine;
    initialize_machine(&machine, iterations);
    
    int result = run_state_machine(&machine);
    if (result != 0) {
        printf("State machine execution failed\n");
        return 1;
    }
    
    return 0;
}