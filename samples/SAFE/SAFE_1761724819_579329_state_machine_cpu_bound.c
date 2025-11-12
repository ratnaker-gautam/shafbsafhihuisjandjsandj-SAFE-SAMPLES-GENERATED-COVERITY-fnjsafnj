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
} State;

typedef struct {
    State current_state;
    uint64_t counter;
    uint32_t iterations;
    int error_code;
} StateMachine;

void state_machine_init(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->counter = 0;
    sm->iterations = 0;
    sm->error_code = 0;
}

State state_transition(State current, uint64_t counter, int *error) {
    if (error == NULL) return STATE_ERROR;
    
    switch (current) {
        case STATE_IDLE:
            if (counter % 2 == 0) return STATE_READY;
            return STATE_IDLE;
            
        case STATE_READY:
            if (counter % 3 == 0) return STATE_PROCESSING;
            if (counter % 7 == 0) return STATE_ERROR;
            return STATE_READY;
            
        case STATE_PROCESSING:
            if (counter % 5 == 0) return STATE_WAITING;
            if (counter % 11 == 0) return STATE_ERROR;
            return STATE_PROCESSING;
            
        case STATE_WAITING:
            if (counter % 13 == 0) return STATE_COMPLETING;
            return STATE_WAITING;
            
        case STATE_COMPLETING:
            if (counter % 17 == 0) return STATE_FINAL;
            return STATE_COMPLETING;
            
        case STATE_ERROR:
            *error = (int)(counter % 256);
            return STATE_RESET;
            
        case STATE_RESET:
            return STATE_IDLE;
            
        case STATE_FINAL:
            return STATE_FINAL;
            
        default:
            *error = 255;
            return STATE_ERROR;
    }
}

void process_state(StateMachine *sm) {
    if (sm == NULL) return;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            sm->counter += 1;
            break;
            
        case STATE_READY:
            sm->counter = sm->counter * 3 + 1;
            break;
            
        case STATE_PROCESSING:
            for (int i = 0; i < 100; i++) {
                sm->counter = (sm->counter * 7 + 3) % 1000000;
            }
            break;
            
        case STATE_WAITING:
            sm->counter = (sm->counter << 3) | (sm->counter >> 61);
            break;
            
        case STATE_COMPLETING:
            sm->counter = sm->counter ^ (sm->counter >> 32);
            break;
            
        case STATE_ERROR:
            sm->counter = 0;
            break;
            
        case STATE_RESET:
            sm->counter = 1;
            break;
            
        case STATE_FINAL:
            break;
            
        default:
            sm->error_code = 1;
            break;
    }
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    printf("Starting state machine simulation\n");
    
    while (sm.iterations < MAX_ITERATIONS) {
        if (sm.iterations == UINT32_MAX) {
            break;
        }
        
        State next_state = state_transition(sm.current_state, sm.counter, &sm.error_code);
        
        if (next_state >= STATE_COUNT || next_state < 0) {
            sm.error_code = 2;
            break;
        }
        
        sm.current_state = next_state;
        process_state(&sm);
        
        sm.iterations++;
        
        if (sm.current_state == STATE_FINAL) {
            break;
        }
        
        if (sm.error_code != 0 && sm.current_state != STATE_RESET) {
            break;
        }
    }
    
    printf("Final state: %d\n", sm.current_state);
    printf("Iterations: %u\n", sm.iterations);
    printf("Final counter: %lu\n", sm.counter);
    printf("Error code: %d\n", sm.error_code);
    
    return sm.error_code;
}