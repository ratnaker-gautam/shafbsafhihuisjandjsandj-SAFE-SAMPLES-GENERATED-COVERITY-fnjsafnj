//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define STATE_COUNT 5
#define MAX_ITERATIONS 1000000

typedef enum {
    STATE_IDLE,
    STATE_PROCESSING,
    STATE_WAITING,
    STATE_COMPLETING,
    STATE_ERROR
} state_t;

typedef struct {
    state_t current_state;
    uint64_t counter;
    uint32_t iterations;
} state_machine_t;

static state_t handle_idle(state_machine_t *sm) {
    if (sm->counter % 2 == 0) {
        return STATE_PROCESSING;
    } else {
        return STATE_WAITING;
    }
}

static state_t handle_processing(state_machine_t *sm) {
    uint64_t result = sm->counter * 3;
    if (result < sm->counter) {
        return STATE_ERROR;
    }
    sm->counter = result;
    return STATE_COMPLETING;
}

static state_t handle_waiting(state_machine_t *sm) {
    sm->counter += 7;
    if (sm->counter < sm->counter - 7) {
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

static state_t handle_completing(state_machine_t *sm) {
    sm->counter /= 2;
    return STATE_IDLE;
}

static state_t handle_error(state_machine_t *sm) {
    sm->counter = 1;
    return STATE_IDLE;
}

static void run_state_machine(state_machine_t *sm) {
    for (sm->iterations = 0; sm->iterations < MAX_ITERATIONS; sm->iterations++) {
        switch (sm->current_state) {
            case STATE_IDLE:
                sm->current_state = handle_idle(sm);
                break;
            case STATE_PROCESSING:
                sm->current_state = handle_processing(sm);
                break;
            case STATE_WAITING:
                sm->current_state = handle_waiting(sm);
                break;
            case STATE_COMPLETING:
                sm->current_state = handle_completing(sm);
                break;
            case STATE_ERROR:
                sm->current_state = handle_error(sm);
                break;
            default:
                sm->current_state = STATE_ERROR;
                break;
        }
        
        if (sm->counter > UINT64_MAX / 4) {
            sm->current_state = STATE_ERROR;
        }
    }
}

int main(void) {
    state_machine_t sm;
    sm.current_state = STATE_IDLE;
    sm.counter = 1;
    sm.iterations = 0;
    
    run_state_machine(&sm);
    
    printf("Final state: %d\n", sm.current_state);
    printf("Final counter: %lu\n", sm.counter);
    printf("Iterations completed: %u\n", sm.iterations);
    
    return 0;
}