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
    uint32_t iterations;
    uint8_t data_buffer[64];
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->iterations = 0;
    memset(machine->data_buffer, 0, sizeof(machine->data_buffer));
}

static system_state_t state_transition(system_state_t current, uint64_t counter) {
    switch (current) {
        case STATE_IDLE:
            return STATE_PROCESSING;
        case STATE_PROCESSING:
            if (counter % 3 == 0) return STATE_WAITING;
            return STATE_VALIDATE;
        case STATE_WAITING:
            return STATE_VALIDATE;
        case STATE_VALIDATE:
            if (counter % 7 == 0) return STATE_ERROR;
            return STATE_COMPLETE;
        case STATE_COMPLETE:
            return STATE_FINALIZE;
        case STATE_ERROR:
            return STATE_RESET;
        case STATE_RESET:
            return STATE_IDLE;
        case STATE_FINALIZE:
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

static void process_state(state_machine_t *machine) {
    uint32_t i;
    uint64_t temp;
    
    switch (machine->current_state) {
        case STATE_PROCESSING:
            for (i = 0; i < sizeof(machine->data_buffer); i++) {
                machine->data_buffer[i] = (uint8_t)((machine->counter + i) % 256);
            }
            break;
        case STATE_VALIDATE:
            temp = 0;
            for (i = 0; i < sizeof(machine->data_buffer); i++) {
                temp += machine->data_buffer[i];
            }
            if (temp % 11 == 0) {
                machine->counter += temp;
            }
            break;
        case STATE_COMPLETE:
            for (i = 0; i < sizeof(machine->data_buffer); i++) {
                machine->data_buffer[i] ^= 0xFF;
            }
            break;
        case STATE_FINALIZE:
            machine->counter = (machine->counter * 6364136223846793005ULL) + 1;
            break;
        default:
            break;
    }
}

int main(int argc, char *argv[]) {
    state_machine_t machine;
    uint32_t max_iterations;
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <iterations>\n", argv[0]);
        return 1;
    }
    
    max_iterations = (uint32_t)atoi(argv[1]);
    if (max_iterations == 0 || max_iterations > MAX_STEPS) {
        fprintf(stderr, "Invalid iterations. Must be 1-%d\n", MAX_STEPS);
        return 1;
    }
    
    initialize_machine(&machine);
    
    printf("Starting state machine with %u iterations\n", max_iterations);
    
    for (machine.iterations = 0; machine.iterations < max_iterations; machine.iterations++) {
        process_state(&machine);
        machine.current_state = state_transition(machine.current_state, machine.counter);
        
        if (machine.iterations % 100000 == 0) {
            printf("Iteration: %u, State: %d, Counter: %lu\n", 
                   machine.iterations, machine.current_state, machine.counter);
        }
    }
    
    printf("Final state: %d, Final counter: %lu\n", machine.current_state, machine.counter);
    
    return 0;
}