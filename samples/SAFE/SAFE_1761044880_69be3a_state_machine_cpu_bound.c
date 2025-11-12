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
    uint32_t iterations;
    uint8_t data_buffer[64];
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->iterations = 0;
    memset(machine->data_buffer, 0, sizeof(machine->data_buffer));
}

static system_state_t state_transition(state_machine_t *machine) {
    uint32_t hash = 0;
    for (size_t i = 0; i < sizeof(machine->data_buffer); i++) {
        hash = (hash * 31) + machine->data_buffer[i];
    }
    
    machine->counter++;
    machine->iterations++;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            if (machine->counter % 2 == 0) {
                return STATE_PROCESSING;
            }
            return STATE_WAITING;
            
        case STATE_PROCESSING:
            if (hash % 3 == 0) {
                return STATE_COMPLETE;
            } else if (hash % 7 == 0) {
                return STATE_ERROR;
            }
            return STATE_VALIDATE;
            
        case STATE_WAITING:
            if (machine->counter % 5 == 0) {
                return STATE_PROCESSING;
            }
            return STATE_WAITING;
            
        case STATE_COMPLETE:
            return STATE_FINALIZE;
            
        case STATE_ERROR:
            if (machine->counter % 11 == 0) {
                return STATE_RESET;
            }
            return STATE_ERROR;
            
        case STATE_RESET:
            return STATE_IDLE;
            
        case STATE_VALIDATE:
            if (hash % 13 == 0) {
                return STATE_COMPLETE;
            }
            return STATE_PROCESSING;
            
        case STATE_FINALIZE:
            return STATE_IDLE;
            
        default:
            return STATE_RESET;
    }
}

static void update_buffer(state_machine_t *machine) {
    for (size_t i = 0; i < sizeof(machine->data_buffer); i++) {
        machine->data_buffer[i] = (uint8_t)((machine->counter + i * 17) % 256);
    }
}

static const char* state_name(system_state_t state) {
    static const char* names[] = {
        "IDLE", "PROCESSING", "WAITING", "COMPLETE",
        "ERROR", "RESET", "VALIDATE", "FINALIZE"
    };
    
    if (state >= 0 && state < STATE_COUNT) {
        return names[state];
    }
    return "UNKNOWN";
}

int main(void) {
    state_machine_t machine;
    uint32_t max_iterations;
    
    printf("Enter maximum iterations (1-1000000): ");
    if (scanf("%u", &max_iterations) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (max_iterations < 1 || max_iterations > MAX_STEPS) {
        fprintf(stderr, "Iterations out of range\n");
        return 1;
    }
    
    initialize_machine(&machine);
    
    printf("Starting state machine simulation\n");
    printf("Initial state: %s\n", state_name(machine.current_state));
    
    while (machine.iterations < max_iterations) {
        update_buffer(&machine);
        machine.current_state = state_transition(&machine);
        
        if (machine.iterations % 10000 == 0) {
            printf("Step %u: State=%s, Counter=%lu\n", 
                   machine.iterations, state_name(machine.current_state), 
                   machine.counter);
        }
        
        if (machine.current_state == STATE_FINALIZE) {
            printf("Reached final state at iteration %u\n", machine.iterations);
            break;
        }
    }
    
    printf("Final state: %s\n", state_name(machine.current_state));
    printf("Total iterations: %u\n", machine.iterations);
    printf("Total counter: %lu\n", machine.counter);
    
    return 0;
}