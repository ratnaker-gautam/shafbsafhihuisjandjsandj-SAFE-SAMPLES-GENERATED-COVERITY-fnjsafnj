//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define STATE_COUNT 8
#define MAX_STEPS 1000000

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H
} state_t;

typedef struct {
    state_t current_state;
    uint64_t counter;
    uint32_t transitions;
} state_machine_t;

static state_t get_next_state(state_t current, uint64_t counter) {
    switch (current) {
        case STATE_A:
            return (counter % 3 == 0) ? STATE_B : STATE_C;
        case STATE_B:
            return (counter % 5 == 0) ? STATE_D : STATE_E;
        case STATE_C:
            return (counter % 7 == 0) ? STATE_F : STATE_G;
        case STATE_D:
            return (counter % 11 == 0) ? STATE_H : STATE_A;
        case STATE_E:
            return (counter % 13 == 0) ? STATE_B : STATE_C;
        case STATE_F:
            return (counter % 17 == 0) ? STATE_D : STATE_E;
        case STATE_G:
            return (counter % 19 == 0) ? STATE_F : STATE_G;
        case STATE_H:
            return (counter % 23 == 0) ? STATE_H : STATE_A;
        default:
            return STATE_A;
    }
}

static void state_machine_init(state_machine_t *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_A;
    sm->counter = 0;
    sm->transitions = 0;
}

static int state_machine_step(state_machine_t *sm) {
    if (sm == NULL) return 0;
    if (sm->counter >= MAX_STEPS) return 0;
    
    state_t next_state = get_next_state(sm->current_state, sm->counter);
    if (next_state != sm->current_state) {
        sm->transitions++;
    }
    sm->current_state = next_state;
    sm->counter++;
    
    return 1;
}

static void print_state_name(state_t state) {
    const char *names[] = {
        "A", "B", "C", "D", "E", "F", "G", "H"
    };
    if (state < STATE_COUNT) {
        printf("%s", names[state]);
    }
}

int main(void) {
    state_machine_t sm;
    uint32_t target_steps;
    
    printf("Enter number of state machine steps (1-1000000): ");
    if (scanf("%u", &target_steps) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (target_steps < 1 || target_steps > MAX_STEPS) {
        printf("Steps must be between 1 and %d\n", MAX_STEPS);
        return 1;
    }
    
    state_machine_init(&sm);
    
    printf("Initial state: ");
    print_state_name(sm.current_state);
    printf("\n");
    
    for (uint32_t i = 0; i < target_steps; i++) {
        if (!state_machine_step(&sm)) {
            break;
        }
    }
    
    printf("Final state: ");
    print_state_name(sm.current_state);
    printf("\n");
    printf("Total transitions: %u\n", sm.transitions);
    printf("Total steps: %lu\n", sm.counter);
    
    return 0;
}