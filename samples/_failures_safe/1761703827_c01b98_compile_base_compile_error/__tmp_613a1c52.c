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
    STATE_VALIDATING,
    STATE_COMPUTING,
    STATE_VERIFYING,
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

static void state_init(state_machine_t *sm) {
    sm->counter = 0;
    sm->iterations = 0;
    sm->error_code = 0;
    sm->current_state = STATE_PROCESSING;
}

static void state_processing(state_machine_t *sm) {
    for (int i = 0; i < 100; i++) {
        if (sm->counter < UINT64_MAX - 100) {
            sm->counter += (i * 3) % 17;
        } else {
            sm->error_code = 1;
            sm->current_state = STATE_ERROR;
            return;
        }
    }
    sm->current_state = STATE_VALIDATING;
}

static void state_validating(state_machine_t *sm) {
    uint64_t check = 0;
    for (int i = 0; i < 50; i++) {
        if (check < UINT64_MAX - 50) {
            check += (sm->counter + i) % 13;
        } else {
            sm->error_code = 2;
            sm->current_state = STATE_ERROR;
            return;
        }
    }
    if (check % 7 == sm->counter % 7) {
        sm->current_state = STATE_COMPUTING;
    } else {
        sm->error_code = 3;
        sm->current_state = STATE_ERROR;
    }
}

static void state_computing(state_machine_t *sm) {
    uint64_t temp = sm->counter;
    for (int i = 0; i < 200; i++) {
        if (temp < UINT64_MAX / 2) {
            temp = (temp * 3 + 1) % 1000000;
        } else {
            sm->error_code = 4;
            sm->current_state = STATE_ERROR;
            return;
        }
    }
    sm->counter = temp;
    sm->current_state = STATE_VERIFYING;
}

static void state_verifying(state_machine_t *sm) {
    uint64_t verify = sm->counter;
    for (int i = 0; i < 100; i++) {
        if (verify > 0) {
            verify = (verify * 2) % 1000000;
        } else {
            sm->error_code = 5;
            sm->current_state = STATE_ERROR;
            return;
        }
    }
    if (verify == sm->counter * 1024 % 1000000) {
        sm->current_state = STATE_FINALIZING;
    } else {
        sm->error_code = 6;
        sm->current_state = STATE_ERROR;
    }
}

static void state_finalizing(state_machine_t *sm) {
    uint64_t final = sm->counter;
    for (int i = 0; i < 150; i++) {
        if (final < UINT64_MAX - 150) {
            final += (i * final) % 1000;
        } else {
            sm->error_code = 7;
            sm->current_state = STATE_ERROR;
            return;
        }
    }
    sm->counter = final;
    sm->current_state = STATE_DONE;
}

static void state_error(state_machine_t *sm) {
    sm->counter = 0;
}

static void state_done(state_machine_t *sm) {
    sm->counter += 1;
}

static void run_state_machine(state_machine_t *sm) {
    while (sm->current_state != STATE_DONE && sm->current_state != STATE_ERROR) {
        if (sm->iterations >= MAX_ITERATIONS) {
            sm->error_code = 8;
            sm->current_state = STATE_ERROR;
            break;
        }
        
        switch (sm->current_state) {
            case STATE_INIT:
                state_init(sm);
                break;
            case STATE_PROCESSING:
                state_processing(sm);
                break;
            case STATE_VALIDATING:
                state_validating(sm);
                break;
            case STATE_COMPUTING:
                state_computing(sm);
                break;
            case STATE_VERIFYING:
                state_verifying(sm);
                break;
            case STATE_FINALIZING:
                state_finalizing(sm);
                break;
            case STATE_ERROR:
                state_error(sm);
                break;
            case STATE_DONE:
                state_done(sm);
                break;
        }
        
        sm->iterations++;
    }
}

int main(void) {
    state_machine_t sm;
    
    sm.current_state = STATE_INIT;
    sm.counter = 0;
    sm.iterations = 0;
    sm.error_code = 0;
    
    run_state_machine(&sm);
    
    if (sm.current_state == STATE_DONE) {
        printf("State machine completed successfully. Final counter: %lu\n", sm.counter);
        return 0;
    } else {
        printf("State machine failed with error code: %