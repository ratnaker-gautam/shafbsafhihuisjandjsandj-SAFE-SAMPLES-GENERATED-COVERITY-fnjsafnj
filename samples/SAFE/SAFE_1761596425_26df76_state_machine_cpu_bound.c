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
            for (int i = 0; i < 100; i++) {
                sm->counter += (input * i);
            }
            break;
        case STATE_COMPLETING:
            sm->counter *= 2;
            if (sm->counter > UINT64_MAX / 2) {
                sm->counter = UINT64_MAX;
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

int validate_input(const char *input_str) {
    if (input_str == NULL) return INT_MIN;
    
    char *endptr;
    long value = strtol(input_str, &endptr, 10);
    
    if (endptr == input_str || *endptr != '\0') {
        return INT_MIN;
    }
    
    if (value < -10 || value > 10) {
        return INT_MIN;
    }
    
    if (value < INT_MIN || value > INT_MAX) {
        return INT_MIN;
    }
    
    return (int)value;
}

int main(void) {
    StateMachine sm;
    state_machine_init(&sm);
    
    char input_buffer[32];
    int input_value;
    
    printf("State Machine Simulation (Enter numbers -10 to 10, 'q' to quit)\n");
    
    while (sm.iterations < MAX_ITERATIONS) {
        printf("Current state: %d, Counter: %lu, Iterations: %u\n", 
               sm.current_state, sm.counter, sm.iterations);
        printf("Enter input: ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        if (input_buffer[0] == 'q' && (input_buffer[1] == '\n' || input_buffer[1] == '\0')) {
            break;
        }
        
        input_value = validate_input(input_buffer);
        if (input_value == INT_MIN) {
            printf("Invalid input. Please enter a number between -10 and 10.\n");
            continue;
        }
        
        state_machine_process(&sm, input_value);
        
        if (sm.current_state == STATE_FINAL) {
            printf("Reached final state. Simulation complete.\n");
            break;
        }
        
        if (sm.current_state == STATE_ERROR) {
            printf("Error state reached with code: %d\n", sm.error_code);
        }
    }
    
    if (sm.iterations >= MAX_ITERATIONS) {
        printf("Maximum iterations reached.\n");
    }
    
    printf("Final state: %d, Final counter: %lu, Total iterations: %u\n",
           sm.current_state, sm.counter, sm.iterations);
    
    return 0;
}