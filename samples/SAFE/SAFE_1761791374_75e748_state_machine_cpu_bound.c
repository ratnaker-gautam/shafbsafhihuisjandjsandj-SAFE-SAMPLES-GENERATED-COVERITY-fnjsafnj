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

State state_machine_transition(StateMachine *sm, int input) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (input == 1) return STATE_READY;
            break;
        case STATE_READY:
            if (input == 2) return STATE_PROCESSING;
            if (input == 0) return STATE_IDLE;
            break;
        case STATE_PROCESSING:
            if (input == 3) return STATE_WAITING;
            if (input == 4) return STATE_COMPLETING;
            if (input == -1) return STATE_ERROR;
            break;
        case STATE_WAITING:
            if (input == 5) return STATE_PROCESSING;
            if (input == 6) return STATE_COMPLETING;
            break;
        case STATE_COMPLETING:
            if (input == 7) return STATE_FINAL;
            if (input == 0) return STATE_RESET;
            break;
        case STATE_ERROR:
            if (input == 0) return STATE_RESET;
            break;
        case STATE_RESET:
            if (input == 1) return STATE_READY;
            if (input == 0) return STATE_IDLE;
            break;
        case STATE_FINAL:
            return STATE_FINAL;
        default:
            return STATE_ERROR;
    }
    return sm->current_state;
}

void state_machine_process(StateMachine *sm, int input) {
    if (sm == NULL) return;
    
    State new_state = state_machine_transition(sm, input);
    sm->current_state = new_state;
    
    switch (new_state) {
        case STATE_PROCESSING:
            for (int i = 0; i < 1000; i++) {
                sm->counter += (i * 3) % 97;
            }
            break;
        case STATE_COMPLETING:
            for (int i = 0; i < 500; i++) {
                sm->counter += (i * 7) % 113;
            }
            break;
        case STATE_ERROR:
            sm->error_code = input;
            break;
        case STATE_RESET:
            sm->counter = 0;
            sm->error_code = 0;
            break;
        default:
            break;
    }
    
    sm->iterations++;
}

int get_valid_input(void) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return INT_MIN;
    }
    
    char *endptr;
    long value = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return INT_MIN;
    }
    
    if (value < -10 || value > 10) {
        return INT_MIN;
    }
    
    return (int)value;
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    printf("State Machine Simulation\n");
    printf("Input range: -10 to 10 (invalid inputs will be ignored)\n");
    printf("Special inputs: 0=reset/idle, -1=error, 7=finalize\n");
    printf("Current state: %d, Counter: %lu\n", sm.current_state, sm.counter);
    
    while (sm.iterations < MAX_ITERATIONS && sm.current_state != STATE_FINAL) {
        printf("Enter input: ");
        fflush(stdout);
        
        int input = get_valid_input();
        if (input == INT_MIN) {
            printf("Invalid input. Please enter a number between -10 and 10.\n");
            continue;
        }
        
        state_machine_process(&sm, input);
        
        printf("State: %d, Counter: %lu, Iterations: %u", 
               sm.current_state, sm.counter, sm.iterations);
        if (sm.current_state == STATE_ERROR) {
            printf(", Error: %d", sm.error_code);
        }
        printf("\n");
        
        if (sm.current_state == STATE_FINAL) {
            printf("Reached final state. Simulation complete.\n");
            break;
        }
    }
    
    if (sm.iterations >= MAX_ITERATIONS) {
        printf("Maximum iterations reached.\n");
    }
    
    printf("Final counter value: %lu\n", sm.counter);
    return 0;
}