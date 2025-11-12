//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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
    uint32_t step_limit;
    int error_code;
} state_machine_t;

static void initialize_machine(state_machine_t *machine) {
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->step_limit = MAX_STEPS;
    machine->error_code = 0;
}

static int validate_transition(system_state_t current, system_state_t next) {
    const int valid_transitions[STATE_COUNT][STATE_COUNT] = {
        {0, 1, 0, 0, 0, 1, 0, 0},
        {0, 0, 1, 1, 1, 0, 0, 0},
        {0, 1, 0, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 1},
        {0, 0, 0, 0, 0, 1, 0, 0},
        {1, 0, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };
    
    if (current >= STATE_COUNT || next >= STATE_COUNT) {
        return 0;
    }
    
    return valid_transitions[current][next];
}

static system_state_t compute_next_state(state_machine_t *machine) {
    uint64_t computation = machine->counter;
    
    for (int i = 0; i < 100; i++) {
        computation = (computation * 6364136223846793005ULL + 1ULL);
    }
    
    switch (machine->current_state) {
        case STATE_IDLE:
            return (computation & 1) ? STATE_PROCESSING : STATE_RESET;
        case STATE_PROCESSING:
            return (computation % 3 == 0) ? STATE_WAITING : 
                   (computation % 7 == 0) ? STATE_COMPLETE : STATE_PROCESSING;
        case STATE_WAITING:
            return (computation % 5 == 0) ? STATE_PROCESSING : 
                   (computation % 11 == 0) ? STATE_COMPLETE : STATE_WAITING;
        case STATE_COMPLETE:
            return STATE_FINALIZE;
        case STATE_ERROR:
            return STATE_RESET;
        case STATE_RESET:
            return STATE_VALIDATE;
        case STATE_VALIDATE:
            return (computation % 13 == 0) ? STATE_ERROR : STATE_IDLE;
        case STATE_FINALIZE:
            return STATE_FINALIZE;
        default:
            return STATE_ERROR;
    }
}

static void execute_state_action(state_machine_t *machine) {
    uint64_t local_counter = machine->counter;
    
    switch (machine->current_state) {
        case STATE_PROCESSING:
            for (int i = 0; i < 1000; i++) {
                local_counter = local_counter * 1103515245ULL + 12345ULL;
            }
            break;
        case STATE_WAITING:
            for (int i = 0; i < 500; i++) {
                local_counter = (local_counter >> 1) ^ (local_counter & 1 ? 0x8000000000000000ULL : 0);
            }
            break;
        case STATE_VALIDATE:
            for (int i = 0; i < 800; i++) {
                local_counter = (local_counter * 2862933555777941757ULL) + 1ULL;
            }
            break;
        case STATE_FINALIZE:
            for (int i = 0; i < 2000; i++) {
                local_counter = local_counter ^ (local_counter >> 33);
                local_counter = local_counter * 0xff51afd7ed558ccdULL;
                local_counter = local_counter ^ (local_counter >> 33);
            }
            break;
        default:
            break;
    }
    
    machine->counter = local_counter;
}

int main(void) {
    state_machine_t machine;
    initialize_machine(&machine);
    
    printf("Starting state machine simulation\n");
    
    uint32_t steps = 0;
    while (steps < machine.step_limit) {
        system_state_t next_state = compute_next_state(&machine);
        
        if (!validate_transition(machine.current_state, next_state)) {
            machine.current_state = STATE_ERROR;
            machine.error_code = 1;
        } else {
            machine.current_state = next_state;
        }
        
        execute_state_action(&machine);
        
        steps++;
        machine.counter++;
        
        if (machine.current_state == STATE_FINALIZE) {
            break;
        }
        
        if (machine.current_state == STATE_ERROR && machine.error_code != 0) {