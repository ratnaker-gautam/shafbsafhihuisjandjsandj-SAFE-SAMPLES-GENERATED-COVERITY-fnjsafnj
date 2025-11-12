//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define STATE_COUNT 8
#define MAX_ITERATIONS 1000000

typedef enum {
    STATE_IDLE,
    STATE_PROCESSING,
    STATE_WAITING,
    STATE_COMPLETING,
    STATE_ERROR,
    STATE_RESET,
    STATE_VALIDATING,
    STATE_FINAL
} state_t;

typedef struct {
    state_t current_state;
    uint64_t counter;
    uint32_t iterations;
    int error_code;
} state_machine_t;

static state_t handle_idle(state_machine_t *sm) {
    if (sm->counter % 3 == 0) {
        return STATE_PROCESSING;
    } else if (sm->counter % 7 == 0) {
        return STATE_VALIDATING;
    }
    return STATE_IDLE;
}

static state_t handle_processing(state_machine_t *sm) {
    uint64_t temp = sm->counter * 6364136223846793005ULL;
    sm->counter = (temp >> 32) ^ temp;
    
    if (sm->counter % 5 == 0) {
        return STATE_WAITING;
    } else if (sm->counter % 11 == 0) {
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

static state_t handle_waiting(state_machine_t *sm) {
    sm->counter += 1;
    if (sm->counter % 13 == 0) {
        return STATE_COMPLETING;
    }
    return STATE_WAITING;
}

static state_t handle_completing(state_machine_t *sm) {
    sm->counter = (sm->counter * 1103515245 + 12345) & 0x7FFFFFFF;
    if (sm->counter % 17 == 0) {
        return STATE_FINAL;
    }
    return STATE_COMPLETING;
}

static state_t handle_error(state_machine_t *sm) {
    sm->error_code = (int)(sm->counter % 256);
    if (sm->counter % 19 == 0) {
        return STATE_RESET;
    }
    return STATE_ERROR;
}

static state_t handle_reset(state_machine_t *sm) {
    sm->counter = 1;
    sm->error_code = 0;
    return STATE_IDLE;
}

static state_t handle_validating(state_machine_t *sm) {
    uint64_t old = sm->counter;
    sm->counter = (sm->counter << 13) ^ (sm->counter >> 19);
    if (sm->counter < old) {
        return STATE_PROCESSING;
    }
    return STATE_VALIDATING;
}

static state_t handle_final(state_machine_t *sm) {
    return STATE_FINAL;
}

static state_t (*state_handlers[STATE_COUNT])(state_machine_t *) = {
    handle_idle,
    handle_processing,
    handle_waiting,
    handle_completing,
    handle_error,
    handle_reset,
    handle_validating,
    handle_final
};

static void run_state_machine(state_machine_t *sm, uint32_t max_iter) {
    if (sm == NULL || max_iter == 0 || max_iter > MAX_ITERATIONS) {
        return;
    }
    
    for (sm->iterations = 0; sm->iterations < max_iter; sm->iterations++) {
        if (sm->current_state >= STATE_COUNT) {
            sm->current_state = STATE_IDLE;
        }
        
        state_t next_state = state_handlers[sm->current_state](sm);
        
        if (next_state >= STATE_COUNT) {
            next_state = STATE_IDLE;
        }
        
        if (sm->current_state == STATE_FINAL) {
            break;
        }
        
        sm->current_state = next_state;
    }
}

int main(void) {
    state_machine_t sm;
    sm.current_state = STATE_IDLE;
    sm.counter = 1;
    sm.iterations = 0;
    sm.error_code = 0;
    
    uint32_t iterations;
    printf("Enter number of iterations (1-1000000): ");
    
    if (scanf("%u", &iterations) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (iterations == 0 || iterations > MAX_ITERATIONS) {
        printf("Iterations out of range\n");
        return 1;
    }
    
    run_state_machine(&sm, iterations);
    
    printf("Final state: %d\n", sm.current_state);
    printf("Counter value: %lu\n", sm.counter);
    printf("Iterations performed: %u\n", sm.iterations);
    printf("Error code: %d\n", sm.error_code);
    
    return 0;
}