//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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

static system_state_t process_state(state_machine_t *machine) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            machine->counter++;
            if (machine->counter % 3 == 0) return STATE_PROCESSING;
            if (machine->counter % 7 == 0) return STATE_VALIDATE;
            return STATE_IDLE;
            
        case STATE_PROCESSING:
            for (size_t i = 0; i < sizeof(machine->data); i++) {
                machine->data[i] = (uint8_t)((machine->counter + i) % 256);
            }
            if (machine->counter % 5 == 0) return STATE_WAITING;
            return STATE_COMPLETE;
            
        case STATE_WAITING:
            machine->counter += 2;
            if (machine->counter % 11 == 0) return STATE_RESET;
            return STATE_PROCESSING;
            
        case STATE_COMPLETE:
            if (machine->counter > 1000) return STATE_FINALIZE;
            return STATE_IDLE;
            
        case STATE_ERROR:
            return STATE_RESET;
            
        case STATE_RESET:
            machine->counter = 0;
            memset(machine->data, 0, sizeof(machine->data));
            return STATE_IDLE;
            
        case STATE_VALIDATE:
            for (size_t i = 0; i < sizeof(machine->data); i++) {
                if (machine->data[i] != (uint8_t)((machine->counter + i) % 256)) {
                    return STATE_ERROR;
                }
            }
            return STATE_COMPLETE;
            
        case STATE_FINALIZE:
            return STATE_FINALIZE;
            
        default:
            return STATE_ERROR;
    }
}

static void run_state_machine(state_machine_t *machine, uint32_t max_steps) {
    if (machine == NULL || max_steps == 0 || max_steps > MAX_STEPS) return;
    
    while (machine->steps < max_steps && machine->current_state != STATE_FINALIZE) {
        system_state_t next_state = process_state(machine);
        machine->current_state = next_state;
        machine->steps++;
        
        if (machine->steps % 10000 == 0) {
            printf("Step: %u, State: %d, Counter: %lu\n", 
                   machine->steps, machine->current_state, machine->counter);
        }
    }
}

int main(void) {
    state_machine_t machine;
    uint32_t iterations;
    
    printf("Enter number of iterations (1-1000000): ");
    if (scanf("%u", &iterations) != 1) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (iterations < 1 || iterations > MAX_STEPS) {
        fprintf(stderr, "Iterations out of range\n");
        return EXIT_FAILURE;
    }
    
    initialize_machine(&machine);
    
    clock_t start_time = clock();
    run_state_machine(&machine, iterations);
    clock_t end_time = clock();
    
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("Final state: %d\n", machine.current_state);
    printf("Total steps: %u\n", machine.steps);
    printf("Final counter: %lu\n", machine.counter);
    printf("CPU time: %.3f seconds\n", cpu_time);
    
    return EXIT_SUCCESS;
}