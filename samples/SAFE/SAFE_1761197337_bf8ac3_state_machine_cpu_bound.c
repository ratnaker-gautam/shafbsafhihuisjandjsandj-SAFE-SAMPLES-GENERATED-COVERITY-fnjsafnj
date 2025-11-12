//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

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
    int error_code;
} state_machine_t;

static state_t state_transition(state_t current, uint64_t counter, int *error) {
    *error = 0;
    
    switch (current) {
        case STATE_IDLE:
            if (counter % 2 == 0) return STATE_READY;
            return STATE_PROCESSING;
            
        case STATE_READY:
            if (counter % 3 == 0) return STATE_PROCESSING;
            if (counter % 7 == 0) return STATE_ERROR;
            return STATE_WAITING;
            
        case STATE_PROCESSING:
            if (counter % 5 == 0) return STATE_COMPLETING;
            if (counter % 11 == 0) return STATE_ERROR;
            return STATE_PROCESSING;
            
        case STATE_WAITING:
            if (counter % 13 == 0) return STATE_PROCESSING;
            return STATE_WAITING;
            
        case STATE_COMPLETING:
            if (counter % 17 == 0) return STATE_FINAL;
            return STATE_RESET;
            
        case STATE_ERROR:
            return STATE_RESET;
            
        case STATE_RESET:
            return STATE_IDLE;
            
        case STATE_FINAL:
            return STATE_FINAL;
            
        default:
            *error = 1;
            return STATE_ERROR;
    }
}

static const char* state_name(state_t state) {
    static const char* names[] = {
        "IDLE", "READY", "PROCESSING", "WAITING",
        "COMPLETING", "ERROR", "RESET", "FINAL"
    };
    
    if (state >= STATE_IDLE && state <= STATE_FINAL) {
        return names[state];
    }
    return "UNKNOWN";
}

static void state_machine_init(state_machine_t *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->counter = 0;
    sm->iterations = 0;
    sm->error_code = 0;
}

static int state_machine_step(state_machine_t *sm) {
    if (sm == NULL) return -1;
    if (sm->iterations >= MAX_ITERATIONS) return -2;
    
    state_t next_state = state_transition(sm->current_state, sm->counter, &sm->error_code);
    
    if (sm->error_code != 0) {
        sm->current_state = STATE_ERROR;
        return -3;
    }
    
    sm->current_state = next_state;
    sm->counter++;
    sm->iterations++;
    
    return 0;
}

static void perform_cpu_work(uint64_t value) {
    volatile uint64_t result = 0;
    for (uint32_t i = 0; i < 100; i++) {
        result += (value * i) % 9973;
    }
}

int main(void) {
    state_machine_t sm;
    state_machine_init(&sm);
    
    printf("Starting state machine simulation\n");
    printf("Initial state: %s\n", state_name(sm.current_state));
    
    while (sm.current_state != STATE_FINAL && sm.iterations < MAX_ITERATIONS) {
        int result = state_machine_step(&sm);
        if (result != 0) {
            printf("Error in state machine step: %d\n", result);
            break;
        }
        
        perform_cpu_work(sm.counter);
        
        if (sm.iterations % 100000 == 0) {
            printf("Iteration: %u, State: %s, Counter: %lu\n", 
                   sm.iterations, state_name(sm.current_state), sm.counter);
        }
    }
    
    printf("Final state: %s\n", state_name(sm.current_state));
    printf("Total iterations: %u\n", sm.iterations);
    printf("Final counter: %lu\n", sm.counter);
    
    return 0;
}