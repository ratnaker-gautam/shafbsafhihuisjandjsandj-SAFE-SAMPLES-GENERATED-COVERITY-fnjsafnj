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
    uint32_t max_iterations;
} state_machine_t;

static state_t state_transition(state_t current, uint64_t counter) {
    switch (current) {
        case STATE_IDLE:
            return (counter % 2 == 0) ? STATE_READY : STATE_ERROR;
        case STATE_READY:
            return (counter % 3 == 0) ? STATE_PROCESSING : STATE_READY;
        case STATE_PROCESSING:
            return (counter % 5 == 0) ? STATE_WAITING : STATE_PROCESSING;
        case STATE_WAITING:
            return (counter % 7 == 0) ? STATE_COMPLETING : STATE_WAITING;
        case STATE_COMPLETING:
            return (counter % 11 == 0) ? STATE_FINAL : STATE_COMPLETING;
        case STATE_ERROR:
            return (counter % 13 == 0) ? STATE_RESET : STATE_ERROR;
        case STATE_RESET:
            return (counter % 17 == 0) ? STATE_IDLE : STATE_RESET;
        case STATE_FINAL:
            return STATE_FINAL;
        default:
            return STATE_ERROR;
    }
}

static const char* state_name(state_t state) {
    static const char* names[] = {
        "IDLE", "READY", "PROCESSING", "WAITING",
        "COMPLETING", "ERROR", "RESET", "FINAL"
    };
    if (state >= STATE_IDLE && state <= STATE_FINAL) {
        return names[state];
    }
    return "UNKNOWN";
}

static void initialize_machine(state_machine_t* machine, uint32_t max_iter) {
    if (machine == NULL) return;
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->iterations = 0;
    machine->max_iterations = (max_iter > 0 && max_iter <= MAX_ITERATIONS) ? max_iter : 1000;
}

static int run_state_machine(state_machine_t* machine) {
    if (machine == NULL) return -1;
    
    printf("Starting state machine with %u iterations\n", machine->max_iterations);
    
    while (machine->iterations < machine->max_iterations) {
        state_t next_state = state_transition(machine->current_state, machine->counter);
        
        printf("Iteration %u: %s -> %s (counter: %lu)\n",
               machine->iterations + 1,
               state_name(machine->current_state),
               state_name(next_state),
               machine->counter);
        
        machine->current_state = next_state;
        machine->counter++;
        machine->iterations++;
        
        if (machine->current_state == STATE_FINAL) {
            printf("Reached final state after %u iterations\n", machine->iterations);
            return 0;
        }
        
        if (machine->counter == UINT64_MAX) {
            printf("Counter overflow detected\n");
            return -1;
        }
    }
    
    printf("Completed %u iterations without reaching final state\n", machine->iterations);
    return 0;
}

int main(void) {
    state_machine_t machine;
    
    initialize_machine(&machine, 500);
    
    int result = run_state_machine(&machine);
    
    if (result != 0) {
        fprintf(stderr, "State machine execution failed\n");
        return EXIT_FAILURE;
    }
    
    printf("State machine completed successfully\n");
    return EXIT_SUCCESS;
}