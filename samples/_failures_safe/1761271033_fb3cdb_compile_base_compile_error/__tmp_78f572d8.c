//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_STATES 8
#define MAX_TRANSITIONS 16
#define MAX_INPUTS 256

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H
} State;

typedef struct {
    State current;
    char input;
    State next;
} Transition;

typedef struct {
    State states[MAX_STATES];
    Transition transitions[MAX_TRANSITIONS];
    int state_count;
    int transition_count;
    State current_state;
} StateMachine;

void initialize_machine(StateMachine *sm) {
    sm->state_count = 8;
    sm->states[0] = STATE_A;
    sm->states[1] = STATE_B;
    sm->states[2] = STATE_C;
    sm->states[3] = STATE_D;
    sm->states[4] = STATE_E;
    sm->states[5] = STATE_F;
    sm->states[6] = STATE_G;
    sm->states[7] = STATE_H;
    
    sm->transition_count = 16;
    sm->transitions[0] = (Transition){STATE_A, '0', STATE_B};
    sm->transitions[1] = (Transition){STATE_A, '1', STATE_C};
    sm->transitions[2] = (Transition){STATE_B, '0', STATE_D};
    sm->transitions[3] = (Transition){STATE_B, '1', STATE_E};
    sm->transitions[4] = (Transition){STATE_C, '0', STATE_F};
    sm->transitions[5] = (Transition){STATE_C, '1', STATE_G};
    sm->transitions[6] = (Transition){STATE_D, '0', STATE_H};
    sm->transitions[7] = (Transition){STATE_D, '1', STATE_A};
    sm->transitions[8] = (Transition){STATE_E, '0', STATE_B};
    sm->transitions[9] = (Transition){STATE_E, '1', STATE_C};
    sm->transitions[10] = (Transition){STATE_F, '0', STATE_D};
    sm->transitions[11] = (Transition){STATE_F, '1', STATE_E};
    sm->transitions[12] = (Transition){STATE_G, '0', STATE_F};
    sm->transitions[13] = (Transition){STATE_G, '1', STATE_G};
    sm->transitions[14] = (Transition){STATE_H, '0', STATE_H};
    sm->transitions[15] = (Transition){STATE_H, '1', STATE_A};
    
    sm->current_state = STATE_A;
}

int process_input(StateMachine *sm, char input) {
    if (input != '0' && input != '1') {
        return -1;
    }
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            return 0;
        }
    }
    
    return -1;
}

void run_state_machine(StateMachine *sm, const char *input_sequence) {
    if (input_sequence == NULL) {
        return;
    }
    
    size_t len = strlen(input_sequence);
    if (len >= MAX_INPUTS) {
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (process_input(sm, input_sequence[i]) != 0) {
            break;
        }
    }
}

int validate_sequence(const char *sequence) {
    if (sequence == NULL) {
        return 0;
    }
    
    size_t len = strlen(sequence);
    if (len == 0 || len >= MAX_INPUTS) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (sequence[i] != '0' && sequence[i] != '1') {
            return 0;
        }
    }
    
    return 1;
}

int main() {
    StateMachine sm;
    initialize_machine(&sm);
    
    char input_buffer[MAX_INPUTS + 2];
    
    printf("Enter binary sequence (max %d characters, 0s and 1s only): ", MAX_INPUTS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!validate_sequence(input_buffer)) {
        printf("Invalid input sequence\n");
        return 1;
    }
    
    State initial_state = sm.current_state;
    run_state_machine(&sm, input_buffer);
    
    printf("Initial state: %d\n", initial_state);
    printf("Final state: %d\n", sm.current_state);
    printf("Sequence length: %zu\n", strlen(input_buffer));
    
    int computation_result = 0;
    size_t input_len = strlen(input_buffer);
    for (size_t i = 0; i < input_len; i++) {
        if (