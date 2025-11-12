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
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_RESET,
    STATE_VALIDATE,
    STATE_FINALIZE
} system_state_t;

typedef struct {
    system_state_t current_state;
    uint64_t counter;
    uint32_t steps;
    uint8_t data[256];
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->steps = 0;
    memset(machine->data, 0, sizeof(machine->data));
}

static system_state_t transition_function(system_state_t current, uint64_t counter) {
    switch (current) {
        case STATE_IDLE:
            return (counter % 2 == 0) ? STATE_PROCESSING : STATE_WAITING;
        case STATE_PROCESSING:
            return (counter % 3 == 0) ? STATE_COMPLETE : STATE_VALIDATE;
        case STATE_WAITING:
            return (counter % 5 == 0) ? STATE_PROCESSING : STATE_ERROR;
        case STATE_COMPLETE:
            return STATE_FINALIZE;
        case STATE_ERROR:
            return (counter % 7 == 0) ? STATE_RESET : STATE_ERROR;
        case STATE_RESET:
            return STATE_IDLE;
        case STATE_VALIDATE:
            return (counter % 11 == 0) ? STATE_COMPLETE : STATE_PROCESSING;
        case STATE_FINALIZE:
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

static void process_state(state_machine_t *machine) {
    machine->counter++;
    
    for (size_t i = 0; i < sizeof(machine->data); i++) {
        machine->data[i] = (uint8_t)((machine->data[i] + i + machine->counter) % 256);
    }
    
    uint64_t hash = 0;
    for (size_t i = 0; i < sizeof(machine->data); i++) {
        hash = (hash * 31 + machine->data[i]) % 1000000007;
    }
    
    machine->counter = (hash ^ machine->counter) % 1000000;
}

static void run_simulation(state_machine_t *machine, uint32_t max_iterations) {
    if (max_iterations == 0 || max_iterations > MAX_STEPS) {
        max_iterations = MAX_STEPS;
    }
    
    for (uint32_t i = 0; i < max_iterations; i++) {
        if (machine->steps >= MAX_STEPS) {
            break;
        }
        
        process_state(machine);
        machine->current_state = transition_function(machine->current_state, machine->counter);
        machine->steps++;
        
        if (machine->current_state == STATE_FINALIZE) {
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    uint32_t iterations = 10000;
    
    if (argc > 1) {
        char *endptr;
        long val = strtol(argv[1], &endptr, 10);
        if (endptr != argv[1] && *endptr == '\0' && val > 0 && val <= MAX_STEPS) {
            iterations = (uint32_t)val;
        }
    }
    
    state_machine_t machine;
    initialize_machine(&machine);
    
    clock_t start = clock();
    run_simulation(&machine, iterations);
    clock_t end = clock();
    
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("State machine simulation completed\n");
    printf("Iterations: %u\n", machine.steps);
    printf("Final state: %d\n", machine.current_state);
    printf("Final counter: %lu\n", machine.counter);
    printf("CPU time: %.6f seconds\n", cpu_time);
    
    uint64_t checksum = 0;
    for (size_t i = 0; i < sizeof(machine.data); i++) {
        checksum = (checksum * 31 + machine.data[i]) % 1000000007;
    }
    printf("Data checksum: %lu\n", checksum);
    
    return 0;
}