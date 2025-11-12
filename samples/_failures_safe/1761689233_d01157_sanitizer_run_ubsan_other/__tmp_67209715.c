//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define STATE_COUNT 8
#define MAX_ITERATIONS 1000000

typedef enum {
    STATE_INIT,
    STATE_PROCESSING,
    STATE_VALIDATION,
    STATE_TRANSFORMATION,
    STATE_OPTIMIZATION,
    STATE_FINALIZE,
    STATE_ERROR,
    STATE_COMPLETE
} state_t;

typedef struct {
    state_t current_state;
    uint64_t counter;
    uint32_t data[256];
    int data_size;
    int iterations;
} state_machine_t;

static void initialize_data(state_machine_t *sm) {
    sm->data_size = 256;
    for (int i = 0; i < sm->data_size; i++) {
        sm->data[i] = (uint32_t)(i * 1103515245 + 12345);
    }
}

static state_t state_init(state_machine_t *sm) {
    sm->counter = 0;
    sm->iterations = 0;
    initialize_data(sm);
    return STATE_PROCESSING;
}

static state_t state_processing(state_machine_t *sm) {
    for (int i = 0; i < sm->data_size; i++) {
        sm->data[i] = (sm->data[i] * 1664525 + 1013904223) % UINT32_MAX;
    }
    sm->counter += sm->data_size;
    return STATE_VALIDATION;
}

static state_t state_validation(state_machine_t *sm) {
    uint32_t checksum = 0;
    for (int i = 0; i < sm->data_size; i++) {
        checksum ^= sm->data[i];
        checksum = (checksum << 1) | (checksum >> 31);
    }
    if (checksum == 0) {
        return STATE_ERROR;
    }
    return STATE_TRANSFORMATION;
}

static state_t state_transformation(state_machine_t *sm) {
    for (int i = 0; i < sm->data_size; i++) {
        uint32_t val = sm->data[i];
        val = ((val & 0xFFFF0000) >> 16) | ((val & 0x0000FFFF) << 16);
        val = ((val & 0xFF00FF00) >> 8) | ((val & 0x00FF00FF) << 8);
        sm->data[i] = val;
    }
    return STATE_OPTIMIZATION;
}

static state_t state_optimization(state_machine_t *sm) {
    for (int i = 1; i < sm->data_size - 1; i++) {
        uint32_t prev = sm->data[i - 1];
        uint32_t curr = sm->data[i];
        uint32_t next = sm->data[i + 1];
        sm->data[i] = (prev + 2 * curr + next) / 4;
    }
    return STATE_FINALIZE;
}

static state_t state_finalize(state_machine_t *sm) {
    uint64_t sum = 0;
    for (int i = 0; i < sm->data_size; i++) {
        sum += sm->data[i];
    }
    sm->counter += sum;
    sm->iterations++;
    
    if (sm->iterations >= MAX_ITERATIONS) {
        return STATE_COMPLETE;
    }
    return STATE_PROCESSING;
}

static state_t state_error(state_machine_t *sm) {
    return STATE_COMPLETE;
}

static state_t state_complete(state_machine_t *sm) {
    return STATE_COMPLETE;
}

static state_t (*state_handlers[STATE_COUNT])(state_machine_t *) = {
    state_init,
    state_processing,
    state_validation,
    state_transformation,
    state_optimization,
    state_finalize,
    state_error,
    state_complete
};

int main(void) {
    state_machine_t sm;
    sm.current_state = STATE_INIT;
    
    while (sm.current_state != STATE_COMPLETE && sm.current_state != STATE_ERROR) {
        if (sm.current_state < 0 || sm.current_state >= STATE_COUNT) {
            break;
        }
        state_t next_state = state_handlers[sm.current_state](&sm);
        sm.current_state = next_state;
    }
    
    printf("State machine completed. Final counter: %lu\n", (unsigned long)sm.counter);
    printf("Iterations processed: %d\n", sm.iterations);
    
    return 0;
}