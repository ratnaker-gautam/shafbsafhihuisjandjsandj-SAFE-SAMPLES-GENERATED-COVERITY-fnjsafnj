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
    STATE_PROCESSING,
    STATE_VALIDATING,
    STATE_COMPUTING,
    STATE_AGGREGATING,
    STATE_FINALIZING,
    STATE_ERROR,
    STATE_DONE
} state_t;

typedef struct {
    state_t current_state;
    uint64_t counter;
    uint32_t iterations;
    int error_code;
} state_machine_t;

static state_t state_transition(state_t current, uint64_t counter, int *error) {
    switch (current) {
        case STATE_IDLE:
            return STATE_PROCESSING;
        case STATE_PROCESSING:
            if (counter % 3 == 0) return STATE_VALIDATING;
            return STATE_COMPUTING;
        case STATE_VALIDATING:
            if (counter % 7 == 0) return STATE_ERROR;
            return STATE_AGGREGATING;
        case STATE_COMPUTING:
            return STATE_AGGREGATING;
        case STATE_AGGREGATING:
            if (counter % 11 == 0) return STATE_FINALIZING;
            return STATE_PROCESSING;
        case STATE_FINALIZING:
            return STATE_DONE;
        case STATE_ERROR:
            *error = 1;
            return STATE_DONE;
        case STATE_DONE:
            return STATE_DONE;
        default:
            *error = 1;
            return STATE_ERROR;
    }
}

static uint64_t perform_computation(uint64_t value) {
    uint64_t result = value;
    for (int i = 0; i < 100; i++) {
        result = (result * 1103515245 + 12345) % 2147483647;
        result ^= (result >> 16);
        result = (result * 16807) % 2147483647;
    }
    return result;
}

int main(void) {
    state_machine_t machine;
    machine.current_state = STATE_IDLE;
    machine.counter = 0;
    machine.iterations = 0;
    machine.error_code = 0;

    printf("Starting state machine simulation...\n");

    while (machine.current_state != STATE_DONE && machine.current_state != STATE_ERROR) {
        if (machine.iterations >= MAX_ITERATIONS) {
            machine.current_state = STATE_ERROR;
            machine.error_code = 2;
            break;
        }

        state_t next_state = state_transition(machine.current_state, machine.counter, &machine.error_code);
        
        if (machine.current_state == STATE_PROCESSING || machine.current_state == STATE_COMPUTING) {
            machine.counter = perform_computation(machine.counter);
        }

        if (machine.current_state == STATE_VALIDATING) {
            if (machine.counter > UINT64_MAX / 2) {
                machine.counter /= 2;
            }
        }

        if (machine.current_state == STATE_AGGREGATING) {
            machine.counter += machine.iterations;
            if (machine.counter < machine.iterations) {
                machine.current_state = STATE_ERROR;
                machine.error_code = 3;
                break;
            }
        }

        machine.current_state = next_state;
        machine.iterations++;

        if (machine.iterations % 100000 == 0) {
            printf("Progress: iteration %u, state %d\n", machine.iterations, machine.current_state);
        }
    }

    if (machine.current_state == STATE_DONE) {
        printf("State machine completed successfully after %u iterations\n", machine.iterations);
        printf("Final counter value: %lu\n", machine.counter);
    } else {
        printf("State machine failed with error code %d after %u iterations\n", machine.error_code, machine.iterations);
        return 1;
    }

    return 0;
}