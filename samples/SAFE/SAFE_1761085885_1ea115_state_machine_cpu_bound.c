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
    switch (current) {
        case STATE_IDLE:
            return (counter % 2 == 0) ? STATE_READY : STATE_IDLE;
        case STATE_READY:
            return (counter % 3 == 0) ? STATE_PROCESSING : STATE_READY;
        case STATE_PROCESSING:
            if (counter % 5 == 0) return STATE_WAITING;
            if (counter % 7 == 0) return STATE_ERROR;
            return STATE_PROCESSING;
        case STATE_WAITING:
            return (counter % 11 == 0) ? STATE_COMPLETING : STATE_WAITING;
        case STATE_COMPLETING:
            return (counter % 13 == 0) ? STATE_FINAL : STATE_COMPLETING;
        case STATE_ERROR:
            return (counter % 17 == 0) ? STATE_RESET : STATE_ERROR;
        case STATE_RESET:
            return STATE_IDLE;
        case STATE_FINAL:
            return STATE_FINAL;
        default:
            *error = 1;
            return STATE_ERROR;
    }
}

static const char* state_name(state_t state) {
    static const char* names[] = {
        "IDLE", "READY", "PROCESSING", "WAITING",
        "COMPLETING", "ERROR", "RESET", "FINAL"
    };
    if (state < STATE_IDLE || state > STATE_FINAL) return "UNKNOWN";
    return names[state];
}

static void initialize_machine(state_machine_t *machine) {
    if (!machine) return;
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->iterations = 0;
    machine->error_code = 0;
}

static int validate_iterations(int iterations) {
    return (iterations > 0 && iterations <= MAX_ITERATIONS);
}

int main(void) {
    state_machine_t machine;
    int user_iterations;
    
    printf("Enter number of state transitions (1-%d): ", MAX_ITERATIONS);
    if (scanf("%d", &user_iterations) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (!validate_iterations(user_iterations)) {
        fprintf(stderr, "Invalid iteration count\n");
        return EXIT_FAILURE;
    }
    
    initialize_machine(&machine);
    
    printf("Starting state machine with %d iterations\n", user_iterations);
    printf("Initial state: %s\n", state_name(machine.current_state));
    
    for (machine.iterations = 0; machine.iterations < (uint32_t)user_iterations; machine.iterations++) {
        if (machine.current_state == STATE_FINAL) {
            break;
        }
        
        state_t next_state = state_transition(machine.current_state, machine.counter, &machine.error_code);
        
        if (machine.error_code != 0) {
            fprintf(stderr, "State transition error\n");
            return EXIT_FAILURE;
        }
        
        if (next_state != machine.current_state) {
            printf("Transition: %s -> %s (counter: %lu)\n", 
                   state_name(machine.current_state), 
                   state_name(next_state),
                   machine.counter);
        }
        
        machine.current_state = next_state;
        
        if (machine.counter < UINT64_MAX - 1) {
            machine.counter++;
        } else {
            machine.counter = 0;
        }
        
        for (int i = 0; i < 1000; i++) {
            machine.counter = (machine.counter * 1103515245 + 12345) & 0x7FFFFFFF;
        }
    }
    
    printf("Final state: %s\n", state_name(machine.current_state));
    printf("Total counter value: %lu\n", machine.counter);
    printf("Iterations completed: %u\n", machine.iterations);
    
    return EXIT_SUCCESS;
}