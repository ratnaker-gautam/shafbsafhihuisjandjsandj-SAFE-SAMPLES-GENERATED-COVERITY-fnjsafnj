//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define STATE_COUNT 8
#define MAX_STEPS 1000000

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
    uint32_t steps;
    uint32_t max_iterations;
} state_machine_t;

static state_t state_transition(state_t current, uint64_t counter) {
    switch (current) {
        case STATE_IDLE:
            return (counter % 2 == 0) ? STATE_READY : STATE_ERROR;
        case STATE_READY:
            return (counter % 3 == 0) ? STATE_PROCESSING : STATE_READY;
        case STATE_PROCESSING:
            return (counter % 5 == 0) ? STATE_WAITING : STATE_PROCESSING;
        case STATE_WAITING:
            return (counter % 7 == 0) ? STATE_COMPLETING : STATE_WAITING;
        case STATE_COMPLETING:
            return (counter % 11 == 0) ? STATE_FINAL : STATE_COMPLETING;
        case STATE_ERROR:
            return (counter % 13 == 0) ? STATE_RESET : STATE_ERROR;
        case STATE_RESET:
            return STATE_IDLE;
        case STATE_FINAL:
            return STATE_FINAL;
        default:
            return STATE_ERROR;
    }
}

static void state_machine_init(state_machine_t *sm, uint32_t max_iter) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->counter = 0;
    sm->steps = 0;
    sm->max_iterations = (max_iter > 0 && max_iter <= MAX_STEPS) ? max_iter : 1000;
}

static int state_machine_step(state_machine_t *sm) {
    if (sm == NULL) return 0;
    if (sm->steps >= sm->max_iterations) return 0;
    if (sm->current_state == STATE_FINAL) return 0;
    
    sm->counter++;
    if (sm->counter == UINT64_MAX) sm->counter = 1;
    
    state_t next_state = state_transition(sm->current_state, sm->counter);
    sm->current_state = next_state;
    sm->steps++;
    
    return 1;
}

static void print_state_name(state_t state) {
    const char *names[] = {
        "IDLE", "READY", "PROCESSING", "WAITING",
        "COMPLETING", "ERROR", "RESET", "FINAL"
    };
    if (state < STATE_COUNT) {
        printf("%s", names[state]);
    } else {
        printf("UNKNOWN");
    }
}

int main(int argc, char *argv[]) {
    uint32_t iterations = 1000;
    
    if (argc > 1) {
        long temp = strtol(argv[1], NULL, 10);
        if (temp > 0 && temp <= MAX_STEPS) {
            iterations = (uint32_t)temp;
        } else {
            fprintf(stderr, "Invalid iteration count. Using default: 1000\n");
        }
    }
    
    state_machine_t sm;
    state_machine_init(&sm, iterations);
    
    clock_t start = clock();
    
    while (state_machine_step(&sm)) {
        if (sm.steps % 100000 == 0) {
            printf("Step: %u, State: ", sm.steps);
            print_state_name(sm.current_state);
            printf(", Counter: %lu\n", sm.counter);
        }
    }
    
    clock_t end = clock();
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("Final state: ");
    print_state_name(sm.current_state);
    printf("\nTotal steps: %u\n", sm.steps);
    printf("CPU time: %.3f seconds\n", cpu_time);
    
    return 0;
}