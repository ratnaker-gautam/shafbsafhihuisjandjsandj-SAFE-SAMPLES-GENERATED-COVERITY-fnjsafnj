//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    *error = 0;
    
    switch (current) {
        case STATE_IDLE:
            if (counter % 2 == 0) return STATE_READY;
            return STATE_PROCESSING;
            
        case STATE_READY:
            if (counter % 3 == 0) return STATE_PROCESSING;
            if (counter % 7 == 0) return STATE_ERROR;
            return STATE_WAITING;
            
        case STATE_PROCESSING:
            if (counter % 5 == 0) return STATE_COMPLETING;
            if (counter % 11 == 0) return STATE_ERROR;
            return STATE_WAITING;
            
        case STATE_WAITING:
            if (counter % 13 == 0) return STATE_PROCESSING;
            if (counter % 17 == 0) return STATE_COMPLETING;
            return STATE_READY;
            
        case STATE_COMPLETING:
            if (counter % 19 == 0) return STATE_FINAL;
            return STATE_RESET;
            
        case STATE_ERROR:
            return STATE_RESET;
            
        case STATE_RESET:
            if (counter % 23 == 0) return STATE_IDLE;
            return STATE_READY;
            
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

static int validate_iterations(uint32_t iterations) {
    if (iterations == 0 || iterations > MAX_ITERATIONS) {
        return 0;
    }
    return 1;
}

static void run_state_machine(state_machine_t *machine, uint32_t max_iterations) {
    if (machine == NULL || !validate_iterations(max_iterations)) {
        return;
    }
    
    while (machine->iterations < max_iterations && machine->current_state != STATE_FINAL) {
        state_t next_state = state_transition(machine->current_state, machine->counter, &machine->error_code);
        
        if (machine->error_code != 0) {
            machine->current_state = STATE_ERROR;
            break;
        }
        
        machine->current_state = next_state;
        machine->counter++;
        
        if (machine->counter == UINT64_MAX) {
            machine->error_code = 2;
            machine->current_state = STATE_ERROR;
            break;
        }
        
        machine->iterations++;
        
        for (int i = 0; i < 100; i++) {
            machine->counter = (machine->counter * 1103515245 + 12345) % 2147483647;
        }
    }
}

static const char* state_to_string(state_t state) {
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
    uint32_t iterations;
    
    printf("Enter number of iterations (1-%u): ", MAX_ITERATIONS);
    if (scanf("%u", &iterations) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!validate_iterations(iterations)) {
        printf("Invalid iteration count\n");
        return 1;
    }
    
    initialize_machine(&machine);
    run_state_machine(&machine, iterations);
    
    printf("Final state: %s\n", state_to_string(machine.current_state));
    printf("Iterations completed: %u\n", machine.iterations);
    printf("Counter value: %lu\n", machine.counter);
    printf("Error code: %d\n", machine.error_code);
    
    return 0;
}