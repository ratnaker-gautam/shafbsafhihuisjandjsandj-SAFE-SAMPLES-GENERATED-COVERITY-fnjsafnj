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
    STATE_COMPLETING,
    STATE_ERROR,
    STATE_RESET,
    STATE_VALIDATING,
    STATE_FINALIZING
} state_t;

typedef struct {
    state_t current_state;
    uint64_t counter;
    uint32_t steps;
    uint8_t data[16];
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->steps = 0;
    memset(machine->data, 0, sizeof(machine->data));
}

static state_t transition_state(state_machine_t *machine) {
    uint32_t hash = 0;
    for (size_t i = 0; i < sizeof(machine->data); i++) {
        hash = (hash * 31) + machine->data[i];
    }
    
    uint8_t decision = hash % 7;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            return (decision < 3) ? STATE_PROCESSING : STATE_VALIDATING;
        case STATE_PROCESSING:
            return (decision < 4) ? STATE_WAITING : STATE_COMPLETING;
        case STATE_WAITING:
            return (decision < 2) ? STATE_PROCESSING : STATE_VALIDATING;
        case STATE_COMPLETING:
            return (decision < 3) ? STATE_FINALIZING : STATE_RESET;
        case STATE_ERROR:
            return STATE_RESET;
        case STATE_RESET:
            return STATE_IDLE;
        case STATE_VALIDATING:
            return (decision < 5) ? STATE_PROCESSING : STATE_ERROR;
        case STATE_FINALIZING:
            return (decision < 4) ? STATE_IDLE : STATE_RESET;
        default:
            return STATE_ERROR;
    }
}

static void update_data(state_machine_t *machine) {
    for (size_t i = 0; i < sizeof(machine->data); i++) {
        machine->data[i] = (machine->data[i] + 1) ^ (machine->counter >> (i % 8));
    }
}

static void process_state(state_machine_t *machine) {
    uint64_t computation = 0;
    
    for (size_t i = 0; i < sizeof(machine->data); i++) {
        computation += machine->data[i] * (i + 1);
        computation ^= (computation << 13);
        computation ^= (computation >> 7);
        computation ^= (computation << 17);
    }
    
    machine->counter += computation;
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Starting state machine simulation...\n");
    
    while (machine.steps < MAX_STEPS) {
        process_state(&machine);
        
        state_t next_state = transition_state(&machine);
        machine.current_state = next_state;
        
        update_data(&machine);
        machine.steps++;
        
        if (machine.steps % 100000 == 0) {
            printf("Step: %u, State: %d, Counter: %lu\n", 
                   machine.steps, machine.current_state, machine.counter);
        }
        
        if (machine.current_state == STATE_FINALIZING && machine.counter > 1000000) {
            break;
        }
    }
    
    printf("Final state: %d\n", machine.current_state);
    printf("Total steps: %u\n", machine.steps);
    printf("Final counter: %lu\n", machine.counter);
    
    return 0;
}