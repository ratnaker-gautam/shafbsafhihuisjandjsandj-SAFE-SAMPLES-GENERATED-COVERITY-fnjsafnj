//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#define MAX_STATES 8
#define MAX_INPUTS 16
#define MAX_TRANSITIONS 128

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

typedef enum {
    INPUT_0,
    INPUT_1,
    INPUT_2,
    INPUT_3,
    INPUT_RESET
} input_t;

typedef struct {
    state_t current_state;
    input_t input;
    state_t next_state;
    uint32_t computation;
} transition_t;

static transition_t transitions[MAX_TRANSITIONS];
static size_t transition_count = 0;

static void initialize_state_machine(void) {
    transition_count = 0;
    
    transitions[transition_count++] = (transition_t){STATE_A, INPUT_0, STATE_B, 1};
    transitions[transition_count++] = (transition_t){STATE_A, INPUT_1, STATE_C, 2};
    transitions[transition_count++] = (transition_t){STATE_B, INPUT_0, STATE_C, 3};
    transitions[transition_count++] = (transition_t){STATE_B, INPUT_1, STATE_D, 5};
    transitions[transition_count++] = (transition_t){STATE_C, INPUT_0, STATE_D, 7};
    transitions[transition_count++] = (transition_t){STATE_C, INPUT_1, STATE_E, 11};
    transitions[transition_count++] = (transition_t){STATE_D, INPUT_0, STATE_E, 13};
    transitions[transition_count++] = (transition_t){STATE_D, INPUT_1, STATE_F, 17};
    transitions[transition_count++] = (transition_t){STATE_E, INPUT_0, STATE_F, 19};
    transitions[transition_count++] = (transition_t){STATE_E, INPUT_1, STATE_G, 23};
    transitions[transition_count++] = (transition_t){STATE_F, INPUT_0, STATE_G, 29};
    transitions[transition_count++] = (transition_t){STATE_F, INPUT_1, STATE_H, 31};
    transitions[transition_count++] = (transition_t){STATE_G, INPUT_0, STATE_H, 37};
    transitions[transition_count++] = (transition_t){STATE_G, INPUT_1, STATE_A, 41};
    transitions[transition_count++] = (transition_t){STATE_H, INPUT_0, STATE_A, 43};
    transitions[transition_count++] = (transition_t){STATE_H, INPUT_1, STATE_B, 47};
    
    for (state_t s = STATE_A; s <= STATE_H; s++) {
        if (transition_count < MAX_TRANSITIONS) {
            transitions[transition_count++] = (transition_t){s, INPUT_RESET, STATE_A, 0};
        }
    }
}

static uint64_t cpu_intensive_computation(uint32_t base) {
    uint64_t result = 0;
    if (base == 0) {
        return 0;
    }
    uint64_t limit = (uint64_t)base * 1000;
    if (limit / 1000 != base) {
        return UINT64_MAX;
    }
    for (uint32_t i = 1; i <= limit; i++) {
        if (i % base == 0) {
            if (result > UINT64_MAX - i) {
                return UINT64_MAX;
            }
            result += i;
        }
    }
    return result;
}

static state_t process_transition(state_t current_state, input_t input, uint64_t *computation_result) {
    for (size_t i = 0; i < transition_count; i++) {
        if (transitions[i].current_state == current_state && transitions[i].input == input) {
            *computation_result = cpu_intensive_computation(transitions[i].computation);
            return transitions[i].next_state;
        }
    }
    return current_state;
}

static void print_state_info(state_t state) {
    const char *state_names[] = {
        "STATE_A", "STATE_B", "STATE_C", "STATE_D",
        "STATE_E", "STATE_F", "STATE_G", "STATE_H"
    };
    if (state >= STATE_A && state <= STATE_H) {
        printf("Current state: %s\n", state_names[state]);
    } else {
        printf("Current state: UNKNOWN\n");
    }
}

static input_t get_valid_input(void) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("Enter input (0-3, r for reset, q to quit): ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return INPUT_RESET;
        }
        
        if (buffer[0] == 'q' || buffer[0] == 'Q') {
            return INPUT_RESET;
        }
        
        if (buffer[0] == 'r' || buffer[0] == 'R') {
            return INPUT_RESET;
        }
        
        if (sscanf(buffer, "%d", &value) == 1) {
            if (value >= 0 && value <= 3) {
                return (input_t)value;
            }
        }
        
        printf("Invalid input. Please enter 0, 1, 2, 3, r, or q.\n");
    }
}

int main(void) {
    initialize_state_machine();
    
    state_t current_state = STATE_A;