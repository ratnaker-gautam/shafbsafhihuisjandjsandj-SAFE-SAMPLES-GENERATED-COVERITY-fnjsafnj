//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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
} state_machine_t;

static state_t state_transition(state_t current, uint64_t counter) {
    switch (current) {
        case STATE_IDLE:
            return (counter % 2 == 0) ? STATE_READY : STATE_ERROR;
        case STATE_READY:
            return (counter % 3 == 0) ? STATE_PROCESSING : STATE_WAITING;
        case STATE_PROCESSING:
            return (counter % 5 == 0) ? STATE_COMPLETING : STATE_PROCESSING;
        case STATE_WAITING:
            return (counter % 7 == 0) ? STATE_READY : STATE_PROCESSING;
        case STATE_COMPLETING:
            return (counter % 11 == 0) ? STATE_FINAL : STATE_RESET;
        case STATE_ERROR:
            return (counter % 13 == 0) ? STATE_RESET : STATE_ERROR;
        case STATE_RESET:
            return STATE_IDLE;
        case STATE_FINAL:
            return STATE_FINAL;
        default:
            return STATE_ERROR;
    }
}

static void state_machine_init(state_machine_t *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->counter = 0;
    sm->iterations = 0;
}

static int state_machine_step(state_machine_t *sm) {
    if (sm == NULL) return -1;
    if (sm->iterations >= MAX_ITERATIONS) return -2;
    
    state_t next_state = state_transition(sm->current_state, sm->counter);
    sm->current_state = next_state;
    sm->counter++;
    sm->iterations++;
    
    return 0;
}

static void perform_cpu_work(uint64_t value) {
    volatile uint64_t result = 0;
    for (uint32_t i = 0; i < 100; i++) {
        result += value * i;
        result ^= (value + i);
        result = (result << 3) | (result >> 61);
    }
}

int main(void) {
    state_machine_t sm;
    state_machine_init(&sm);
    
    printf("Starting state machine simulation\n");
    
    while (sm.current_state != STATE_FINAL && sm.iterations < MAX_ITERATIONS) {
        int result = state_machine_step(&sm);
        if (result != 0) {
            printf("State machine error: %d\n", result);
            return 1;
        }
        
        perform_cpu_work(sm.counter);
        
        if (sm.iterations % 100000 == 0) {
            printf("Iteration: %u, State: %d, Counter: %lu\n", 
                   sm.iterations, sm.current_state, sm.counter);
        }
    }
    
    printf("Final state: %d\n", sm.current_state);
    printf("Total iterations: %u\n", sm.iterations);
    printf("Final counter: %lu\n", sm.counter);
    
    return 0;
}