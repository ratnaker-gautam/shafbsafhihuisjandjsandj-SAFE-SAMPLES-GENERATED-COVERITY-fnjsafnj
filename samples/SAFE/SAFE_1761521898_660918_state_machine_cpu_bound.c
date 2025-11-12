//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

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
    uint32_t error_count;
} state_machine_t;

static state_t handle_idle(state_machine_t *sm) {
    if (sm->counter % 2 == 0) {
        return STATE_READY;
    }
    return STATE_IDLE;
}

static state_t handle_ready(state_machine_t *sm) {
    if (sm->counter % 3 == 0) {
        return STATE_PROCESSING;
    }
    return STATE_READY;
}

static state_t handle_processing(state_machine_t *sm) {
    uint64_t result = sm->counter * 3;
    if (result < sm->counter) {
        return STATE_ERROR;
    }
    if (sm->counter % 7 == 0) {
        return STATE_WAITING;
    }
    return STATE_PROCESSING;
}

static state_t handle_waiting(state_machine_t *sm) {
    if (sm->counter % 11 == 0) {
        return STATE_COMPLETING;
    }
    return STATE_WAITING;
}

static state_t handle_completing(state_machine_t *sm) {
    if (sm->counter % 13 == 0) {
        return STATE_FINAL;
    }
    return STATE_COMPLETING;
}

static state_t handle_error(state_machine_t *sm) {
    sm->error_count++;
    if (sm->error_count > 100) {
        return STATE_RESET;
    }
    return STATE_ERROR;
}

static state_t handle_reset(state_machine_t *sm) {
    sm->counter = 0;
    sm->error_count = 0;
    return STATE_IDLE;
}

static state_t handle_final(state_machine_t *sm) {
    return STATE_FINAL;
}

static state_t (*state_handlers[STATE_COUNT])(state_machine_t *) = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_waiting,
    handle_completing,
    handle_error,
    handle_reset,
    handle_final
};

static void run_state_machine(state_machine_t *sm) {
    while (sm->iterations < MAX_ITERATIONS) {
        if (sm->current_state >= STATE_COUNT) {
            sm->current_state = STATE_ERROR;
        }
        
        state_t next_state = state_handlers[sm->current_state](sm);
        
        if (next_state >= STATE_COUNT) {
            sm->current_state = STATE_ERROR;
        } else {
            sm->current_state = next_state;
        }
        
        sm->counter++;
        sm->iterations++;
        
        if (sm->current_state == STATE_FINAL) {
            break;
        }
        
        if (sm->counter == UINT64_MAX) {
            sm->current_state = STATE_RESET;
        }
    }
}

int main(void) {
    state_machine_t sm;
    sm.current_state = STATE_IDLE;
    sm.counter = 0;
    sm.iterations = 0;
    sm.error_count = 0;
    
    run_state_machine(&sm);
    
    printf("Final state: %u\n", sm.current_state);
    printf("Iterations: %u\n", sm.iterations);
    printf("Counter: %lu\n", sm.counter);
    printf("Error count: %u\n", sm.error_count);
    
    return 0;
}