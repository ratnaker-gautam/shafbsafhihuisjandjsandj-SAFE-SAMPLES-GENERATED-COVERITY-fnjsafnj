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

static int validate_iterations(int iterations) {
    if (iterations <= 0 || iterations > MAX_ITERATIONS) {
        return 0;
    }
    return 1;
}

static void run_state_machine(state_machine_t *machine, int max_iterations) {
    if (machine == NULL || max_iterations <= 0) return;
    
    while (machine->iterations < (uint32_t)max_iterations && machine->current_state != STATE_FINAL) {
        state_t next_state = state_transition(machine->current_state, machine->counter, &machine->error_code);
        
        if (machine->error_code != 0) {
            machine->current_state = STATE_ERROR;
            machine->error_code = 1;
            break;
        }
        
        machine->current_state = next_state;
        machine->counter++;
        machine->iterations++;
        
        if (machine->counter == UINT64_MAX) {
            machine->error_code = 2;
            break;
        }
    }
}

int main(void) {
    state_machine_t machine;
    int user_iterations;
    
    printf("Enter number of iterations (1-%d): ", MAX_ITERATIONS);
    if (scanf("%d", &user_iterations) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_iterations(user_iterations)) {
        printf("Invalid iteration count\n");
        return 1;
    }
    
    initialize_machine(&machine);
    run_state_machine(&machine, user_iterations);
    
    printf("Final state: %d\n", machine.current_state);
    printf("Iterations completed: %u\n", machine.iterations);
    printf("Counter value: %lu\n", machine.counter);
    printf("Error code: %d\n", machine.error_code);
    
    return 0;
}