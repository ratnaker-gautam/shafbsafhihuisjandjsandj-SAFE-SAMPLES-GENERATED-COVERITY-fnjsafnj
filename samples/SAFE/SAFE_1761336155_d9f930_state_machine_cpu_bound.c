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

static state_t state_transition(state_t current, uint64_t counter) {
    switch (current) {
        case STATE_IDLE:
            return STATE_PROCESSING;
        case STATE_PROCESSING:
            return (counter % 3 == 0) ? STATE_VALIDATING : STATE_COMPUTING;
        case STATE_VALIDATING:
            return (counter % 7 == 0) ? STATE_ERROR : STATE_AGGREGATING;
        case STATE_COMPUTING:
            return (counter % 5 == 0) ? STATE_AGGREGATING : STATE_PROCESSING;
        case STATE_AGGREGATING:
            return (counter % 11 == 0) ? STATE_FINALIZING : STATE_COMPUTING;
        case STATE_FINALIZING:
            return (counter % 13 == 0) ? STATE_DONE : STATE_IDLE;
        case STATE_ERROR:
            return STATE_DONE;
        case STATE_DONE:
            return STATE_DONE;
        default:
            return STATE_ERROR;
    }
}

static void state_action(state_machine_t *sm) {
    switch (sm->current_state) {
        case STATE_IDLE:
            sm->counter += 1;
            break;
        case STATE_PROCESSING:
            sm->counter = sm->counter * 3 + 1;
            break;
        case STATE_VALIDATING:
            sm->counter = sm->counter ^ (sm->counter >> 1);
            break;
        case STATE_COMPUTING:
            sm->counter = sm->counter * 5 + 7;
            break;
        case STATE_AGGREGATING:
            sm->counter = sm->counter + (sm->counter >> 2);
            break;
        case STATE_FINALIZING:
            sm->counter = sm->counter | 1;
            break;
        case STATE_ERROR:
            sm->error_code = (int)(sm->counter % 256);
            break;
        case STATE_DONE:
            break;
    }
}

static int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > 10) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < '0' || input[i] > '9') return 0;
    }
    return 1;
}

static uint32_t parse_uint32(const char *input) {
    uint64_t result = 0;
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (result > UINT32_MAX / 10) return UINT32_MAX;
        result = result * 10 + (input[i] - '0');
    }
    return (result > UINT32_MAX) ? UINT32_MAX : (uint32_t)result;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <iterations>\n", argv[0]);
        return 1;
    }

    if (!validate_input(argv[1])) {
        fprintf(stderr, "Error: Invalid input format\n");
        return 1;
    }

    uint32_t max_iterations = parse_uint32(argv[1]);
    if (max_iterations == 0 || max_iterations > MAX_ITERATIONS) {
        fprintf(stderr, "Error: Iterations must be between 1 and %d\n", MAX_ITERATIONS);
        return 1;
    }

    state_machine_t sm = {
        .current_state = STATE_IDLE,
        .counter = 1,
        .iterations = 0,
        .error_code = 0
    };

    while (sm.current_state != STATE_DONE && sm.current_state != STATE_ERROR) {
        if (sm.iterations >= max_iterations) {
            fprintf(stderr, "Error: Maximum iterations reached\n");
            return 1;
        }

        state_action(&sm);
        sm.current_state = state_transition(sm.current_state, sm.counter);
        sm.iterations++;

        if (sm.counter > UINT64_MAX / 10) {
            sm.current_state = STATE_ERROR;
            sm.error_code = 1;
        }
    }

    if (sm.current_state == STATE_ERROR) {
        printf("State machine terminated with error code: %d\n", sm.error_code);
        return 1;
    }

    printf("State machine completed successfully after %u iterations\n", sm.iterations);
    printf("Final counter value: %lu\n", sm.counter);
    return 0;
}