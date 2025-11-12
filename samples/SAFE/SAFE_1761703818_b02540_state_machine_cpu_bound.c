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
} State;

typedef struct {
    State current_state;
    uint64_t counter;
    uint32_t data;
    uint8_t flags;
} StateMachine;

static void initialize_machine(StateMachine *machine) {
    machine->current_state = STATE_IDLE;
    machine->counter = 0;
    machine->data = 0;
    machine->flags = 0;
}

static State transition_idle(StateMachine *machine) {
    if (machine->counter > UINT64_MAX - 100) {
        return STATE_ERROR;
    }
    machine->counter += 1;
    machine->data = (machine->data * 1103515245 + 12345) & 0x7FFFFFFF;
    if (machine->data % 3 == 0) {
        return STATE_PROCESSING;
    }
    return STATE_IDLE;
}

static State transition_processing(StateMachine *machine) {
    if (machine->counter > UINT64_MAX - 1000) {
        return STATE_ERROR;
    }
    machine->counter += 10;
    for (int i = 0; i < 100; i++) {
        machine->data = (machine->data * 1664525 + 1013904223) & 0x7FFFFFFF;
    }
    if (machine->data % 5 == 0) {
        return STATE_WAITING;
    }
    return STATE_PROCESSING;
}

static State transition_waiting(StateMachine *machine) {
    if (machine->counter > UINT64_MAX - 100) {
        return STATE_ERROR;
    }
    machine->counter += 5;
    machine->data = (machine->data * 214013 + 2531011) & 0x7FFFFFFF;
    if (machine->data % 7 == 0) {
        return STATE_VALIDATE;
    }
    return STATE_WAITING;
}

static State transition_validate(StateMachine *machine) {
    if (machine->counter > UINT64_MAX - 500) {
        return STATE_ERROR;
    }
    machine->counter += 50;
    uint32_t temp = machine->data;
    for (int i = 0; i < 50; i++) {
        temp = (temp * 1103515245 + 12345) & 0x7FFFFFFF;
    }
    if (temp % 11 == 0) {
        return STATE_FINALIZE;
    }
    return STATE_RESET;
}

static State transition_finalize(StateMachine *machine) {
    if (machine->counter > UINT64_MAX - 100) {
        return STATE_ERROR;
    }
    machine->counter += 20;
    machine->flags |= 0x01;
    return STATE_COMPLETE;
}

static State transition_reset(StateMachine *machine) {
    if (machine->counter > UINT64_MAX - 100) {
        return STATE_ERROR;
    }
    machine->counter += 1;
    machine->data = 0;
    machine->flags = 0;
    return STATE_IDLE;
}

static State transition_complete(StateMachine *machine) {
    return STATE_COMPLETE;
}

static State transition_error(StateMachine *machine) {
    return STATE_ERROR;
}

static void run_state_machine(StateMachine *machine, uint32_t max_steps) {
    typedef State (*TransitionFunc)(StateMachine*);
    static const TransitionFunc transitions[STATE_COUNT] = {
        transition_idle,
        transition_processing,
        transition_waiting,
        transition_complete,
        transition_error,
        transition_reset,
        transition_validate,
        transition_finalize
    };
    
    for (uint32_t step = 0; step < max_steps; step++) {
        if (machine->current_state >= STATE_COUNT) {
            machine->current_state = STATE_ERROR;
            break;
        }
        
        State next_state = transitions[machine->current_state](machine);
        if (next_state >= STATE_COUNT) {
            machine->current_state = STATE_ERROR;
            break;
        }
        
        if (machine->current_state == STATE_COMPLETE || 
            machine->current_state == STATE_ERROR) {
            break;
        }
        
        machine->current_state = next_state;
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("Starting state machine simulation...\n");
    
    run_state_machine(&machine, MAX_STEPS);
    
    printf("Final state: %d\n", machine.current_state);
    printf("Counter value: %lu\n", machine.counter);
    printf("Data value: %u\n", machine.data);
    printf("Flags: 0x%02X\n", machine.flags);
    
    return 0;
}