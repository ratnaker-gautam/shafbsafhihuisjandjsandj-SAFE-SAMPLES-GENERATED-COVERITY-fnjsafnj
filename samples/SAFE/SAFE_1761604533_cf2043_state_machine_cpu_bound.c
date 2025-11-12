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
            return STATE_VALIDATE;
            
        case STATE_PROCESSING:
            if (hash % 3 == 0) {
                return STATE_WAITING;
            } else if (hash % 7 == 0) {
                return STATE_ERROR;
            }
            return STATE_COMPLETE;
            
        case STATE_WAITING:
            if (machine->counter % 5 == 0) {
                return STATE_PROCESSING;
            }
            return STATE_VALIDATE;
            
        case STATE_COMPLETE:
            if (machine->iterations < 100) {
                return STATE_RESET;
            }
            return STATE_FINALIZE;
            
        case STATE_ERROR:
            return STATE_RESET;
            
        case STATE_RESET:
            machine->counter = 0;
            memset(machine->data_buffer, 0, sizeof(machine->data_buffer));
            return STATE_IDLE;
            
        case STATE_VALIDATE:
            for (size_t i = 0; i < sizeof(machine->data_buffer); i++) {
                machine->data_buffer[i] = (hash + i) % 256;
            }
            return STATE_PROCESSING;
            
        case STATE_FINALIZE:
            return STATE_FINALIZE;
    }
    
    return STATE_IDLE;
}

static void process_state(state_machine_t *machine) {
    uint64_t computation = 0;
    
    for (size_t i = 0; i < sizeof(machine->data_buffer); i++) {
        computation += machine->data_buffer[i] * machine->counter;
        computation ^= (computation << 13);
        computation ^= (computation >> 7);
        computation ^= (computation << 17);
    }
    
    machine->data_buffer[computation % sizeof(machine->data_buffer)] = computation % 256;
}

int main(void) {
    state_machine_t machine;
    uint32_t step_count = 0;
    
    initialize_machine(&machine);
    
    printf("Starting state machine simulation\n");
    
    while (machine.current_state != STATE_FINALIZE && step_count < MAX_STEPS) {
        process_state(&machine);
        machine.current_state = state_transition(&machine);
        step_count++;
        
        if (step_count % 100000 == 0) {
            printf("Step: %u, State: %d, Counter: %lu\n", 
                   step_count, machine.current_state, machine.counter);
        }
    }
    
    printf("Final state: %d\n", machine.current_state);
    printf("Total steps: %u\n", step_count);
    printf("Final counter: %lu\n", machine.counter);
    
    return 0;
}