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
    uint8_t error_code;
} state_machine_t;

static state_t state_transition(state_t current, uint64_t counter, uint8_t* error) {
    switch (current) {
        case STATE_IDLE:
            return (counter % 2 == 0) ? STATE_READY : STATE_IDLE;
        case STATE_READY:
            return (counter % 3 == 0) ? STATE_PROCESSING : STATE_READY;
        case STATE_PROCESSING:
            if (counter % 5 == 0) return STATE_WAITING;
            if (counter % 7 == 0) return STATE_ERROR;
            return STATE_PROCESSING;
        case STATE_WAITING:
            return (counter % 11 == 0) ? STATE_COMPLETING : STATE_WAITING;
        case STATE_COMPLETING:
            return (counter % 13 == 0) ? STATE_FINAL : STATE_COMPLETING;
        case STATE_ERROR:
            return (counter % 17 == 0) ? STATE_RESET : STATE_ERROR;
        case STATE_RESET:
            return (counter % 19 == 0) ? STATE_IDLE : STATE_RESET;
        case STATE_FINAL:
            return STATE_FINAL;
        default:
            *error = 1;
            return STATE_ERROR;
    }
}

static void process_state(state_machine_t* sm) {
    switch (sm->current_state) {
        case STATE_IDLE:
            sm->counter += 1;
            break;
        case STATE_READY:
            sm->counter *= 2;
            break;
        case STATE_PROCESSING:
            sm->counter = (sm->counter * 3) / 2;
            break;
        case STATE_WAITING:
            sm->counter += sm->counter % 100;
            break;
        case STATE_COMPLETING:
            sm->counter |= 0xFF;
            break;
        case STATE_ERROR:
            sm->error_code = (uint8_t)(sm->counter % 256);
            break;
        case STATE_RESET:
            sm->counter = 1;
            sm->error_code = 0;
            break;
        case STATE_FINAL:
            break;
        default:
            sm->error_code = 255;
            break;
    }
}

static void print_state_info(const state_machine_t* sm) {
    const char* state_names[] = {
        "IDLE", "READY", "PROCESSING", "WAITING",
        "COMPLETING", "ERROR", "RESET", "FINAL"
    };
    
    if (sm->current_state < STATE_COUNT) {
        printf("State: %-12s Counter: %10lu Error: %3u\n",
               state_names[sm->current_state],
               (unsigned long)sm->counter,
               sm->error_code);
    }
}

int main(void) {
    state_machine_t sm;
    sm.current_state = STATE_IDLE;
    sm.counter = 1;
    sm.iterations = 0;
    sm.error_code = 0;
    
    printf("State Machine Simulation\n");
    printf("========================\n");
    
    while (sm.iterations < MAX_ITERATIONS) {
        if (sm.iterations % 100000 == 0) {
            print_state_info(&sm);
        }
        
        process_state(&sm);
        
        state_t next_state = state_transition(sm.current_state, sm.counter, &sm.error_code);
        
        if (next_state >= STATE_COUNT) {
            sm.error_code = 254;
            break;
        }
        
        sm.current_state = next_state;
        sm.iterations++;
        
        if (sm.current_state == STATE_FINAL) {
            break;
        }
        
        if (sm.counter > UINT64_MAX / 100) {
            sm.counter = 1;
        }
    }
    
    printf("\nFinal State:\n");
    print_state_info(&sm);
    printf("Total iterations: %u\n", sm.iterations);
    
    return (sm.error_code == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}