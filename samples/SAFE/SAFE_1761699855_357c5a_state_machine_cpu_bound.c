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

static void state_idle(state_machine_t *sm) {
    if (sm->counter > UINT64_MAX - 1) {
        sm->current_state = STATE_ERROR;
        sm->error_code = 1;
        return;
    }
    sm->counter += 1;
    sm->current_state = STATE_PROCESSING;
}

static void state_processing(state_machine_t *sm) {
    if (sm->counter > UINT64_MAX - 3) {
        sm->current_state = STATE_ERROR;
        sm->error_code = 2;
        return;
    }
    sm->counter *= 3;
    sm->current_state = STATE_VALIDATING;
}

static void state_validating(state_machine_t *sm) {
    if (sm->counter % 2 == 0) {
        sm->current_state = STATE_COMPUTING;
    } else {
        sm->current_state = STATE_AGGREGATING;
    }
}

static void state_computing(state_machine_t *sm) {
    if (sm->counter > UINT64_MAX - sm->counter) {
        sm->current_state = STATE_ERROR;
        sm->error_code = 3;
        return;
    }
    sm->counter += sm->counter / 2;
    sm->current_state = STATE_FINALIZING;
}

static void state_aggregating(state_machine_t *sm) {
    if (sm->counter > UINT64_MAX - 7) {
        sm->current_state = STATE_ERROR;
        sm->error_code = 4;
        return;
    }
    sm->counter += 7;
    sm->current_state = STATE_FINALIZING;
}

static void state_finalizing(state_machine_t *sm) {
    if (sm->counter > UINT64_MAX - 1) {
        sm->current_state = STATE_ERROR;
        sm->error_code = 5;
        return;
    }
    sm->counter += 1;
    if (sm->counter > 1000000) {
        sm->current_state = STATE_DONE;
    } else {
        sm->current_state = STATE_IDLE;
    }
}

static void state_error(state_machine_t *sm) {
    sm->current_state = STATE_DONE;
}

static void state_done(state_machine_t *sm) {
}

static void (*state_handlers[STATE_COUNT])(state_machine_t *) = {
    state_idle,
    state_processing,
    state_validating,
    state_computing,
    state_aggregating,
    state_finalizing,
    state_error,
    state_done
};

static void run_state_machine(state_machine_t *sm) {
    while (sm->current_state != STATE_DONE && sm->current_state != STATE_ERROR) {
        if (sm->iterations >= MAX_ITERATIONS) {
            sm->current_state = STATE_ERROR;
            sm->error_code = 6;
            break;
        }
        
        if (sm->current_state < 0 || sm->current_state >= STATE_COUNT) {
            sm->current_state = STATE_ERROR;
            sm->error_code = 7;
            break;
        }
        
        state_handlers[sm->current_state](sm);
        sm->iterations++;
    }
    
    if (sm->current_state == STATE_ERROR) {
        state_handlers[STATE_ERROR](sm);
    }
}

int main(void) {
    state_machine_t sm;
    char input_buffer[32];
    unsigned long input_value;
    char *endptr;
    
    printf("Enter initial counter value (0-100000): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_value = strtoul(input_buffer, &endptr, 10);
    if (endptr == input_buffer || *endptr != '\n') {
        fprintf(stderr, "Invalid input format\n");
        return 1;
    }
    
    if (input_value > 100000) {
        fprintf(stderr, "Input value too large\n");
        return 1;
    }
    
    sm.current_state = STATE_IDLE;
    sm.counter = (uint64_t)input_value;
    sm.iterations = 0;
    sm.error_code = 0;
    
    run_state_machine(&sm);
    
    printf("Final counter: %lu\n", (unsigned long)sm.counter);
    printf("Iterations: %u\n", sm.iterations);
    printf("Final state: %d\n", sm.current_state);
    if (sm.error_code != 0) {
        printf("Error code: %d\n", sm.error_code);
    }
    
    return 0;
}