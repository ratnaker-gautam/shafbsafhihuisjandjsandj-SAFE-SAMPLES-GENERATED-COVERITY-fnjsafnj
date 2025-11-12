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
            if (counter % 19 == 0) return STATE_IDLE;
            return STATE_RESET;
            
        case STATE_FINAL:
            return STATE_FINAL;
            
        default:
            *error = 255;
            return STATE_ERROR;
    }
}

const char* state_name(State state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_READY: return "READY";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_WAITING: return "WAITING";
        case STATE_COMPLETING: return "COMPLETING";
        case STATE_ERROR: return "ERROR";
        case STATE_RESET: return "RESET";
        case STATE_FINAL: return "FINAL";
        default: return "UNKNOWN";
    }
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    printf("Starting state machine simulation\n");
    printf("Initial state: %s\n", state_name(sm.current_state));
    
    while (sm.iterations < MAX_ITERATIONS) {
        if (sm.iterations == UINT32_MAX) {
            fprintf(stderr, "Iteration counter overflow\n");
            return 1;
        }
        
        State next_state = state_transition(sm.current_state, sm.counter, &sm.error_code);
        
        if (next_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid state transition\n");
            return 1;
        }
        
        if (sm.current_state != next_state) {
            printf("State transition: %s -> %s (counter: %lu)\n", 
                   state_name(sm.current_state), state_name(next_state), sm.counter);
        }
        
        sm.current_state = next_state;
        sm.iterations++;
        
        if (sm.counter == UINT64_MAX) {
            fprintf(stderr, "Counter overflow\n");
            return 1;
        }
        sm.counter++;
        
        if (sm.current_state == STATE_FINAL) {
            printf("Reached final state after %u iterations\n", sm.iterations);
            break;
        }
        
        if (sm.current_state == STATE_ERROR) {
            printf("Error state encountered with code: %d\n", sm.error_code);
        }
    }
    
    if (sm.iterations >= MAX_ITERATIONS) {
        printf("Reached maximum iterations (%u) without reaching final state\n", MAX_ITERATIONS);
        printf("Final state: %s, Counter: %lu\n", state_name(sm.current_state), sm.counter);
    }
    
    return 0;
}