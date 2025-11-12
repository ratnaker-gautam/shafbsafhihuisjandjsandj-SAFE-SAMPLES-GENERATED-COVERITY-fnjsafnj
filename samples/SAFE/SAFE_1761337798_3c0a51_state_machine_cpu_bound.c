//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
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
} State;

typedef struct {
    State current;
    int input;
    State next;
} Transition;

typedef struct {
    State states[MAX_STATES];
    Transition transitions[MAX_TRANSITIONS];
    int state_count;
    int transition_count;
    State current_state;
} StateMachine;

void init_state_machine(StateMachine* sm) {
    sm->state_count = 8;
    sm->states[0] = STATE_A;
    sm->states[1] = STATE_B;
    sm->states[2] = STATE_C;
    sm->states[3] = STATE_D;
    sm->states[4] = STATE_E;
    sm->states[5] = STATE_F;
    sm->states[6] = STATE_G;
    sm->states[7] = STATE_H;
    
    sm->transition_count = 0;
    
    Transition ts[] = {
        {STATE_A, 0, STATE_B}, {STATE_A, 1, STATE_C},
        {STATE_B, 0, STATE_D}, {STATE_B, 1, STATE_E},
        {STATE_C, 0, STATE_F}, {STATE_C, 1, STATE_G},
        {STATE_D, 0, STATE_H}, {STATE_D, 1, STATE_A},
        {STATE_E, 0, STATE_B}, {STATE_E, 1, STATE_C},
        {STATE_F, 0, STATE_D}, {STATE_F, 1, STATE_E},
        {STATE_G, 0, STATE_F}, {STATE_G, 1, STATE_G},
        {STATE_H, 0, STATE_H}, {STATE_H, 1, STATE_A}
    };
    
    for (int i = 0; i < 16; i++) {
        if (sm->transition_count < MAX_TRANSITIONS) {
            sm->transitions[sm->transition_count++] = ts[i];
        }
    }
    
    sm->current_state = STATE_A;
}

int process_input(StateMachine* sm, int input) {
    if (input != 0 && input != 1) {
        return 0;
    }
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            return 1;
        }
    }
    
    return 0;
}

void simulate_state_machine(StateMachine* sm, int* inputs, int input_count) {
    if (inputs == NULL || input_count <= 0 || input_count > MAX_INPUTS) {
        return;
    }
    
    for (int i = 0; i < input_count; i++) {
        if (!process_input(sm, inputs[i])) {
            break;
        }
    }
}

int validate_input_sequence(const char* str, int* inputs, int max_inputs) {
    if (str == NULL || inputs == NULL || max_inputs <= 0) {
        return 0;
    }
    
    int len = strlen(str);
    if (len == 0 || len > max_inputs) {
        return 0;
    }
    
    for (int i = 0; i < len; i++) {
        if (str[i] == '0') {
            inputs[i] = 0;
        } else if (str[i] == '1') {
            inputs[i] = 1;
        } else {
            return 0;
        }
    }
    
    return len;
}

int main(void) {
    StateMachine sm;
    init_state_machine(&sm);
    
    char input_buffer[64];
    int inputs[MAX_INPUTS];
    
    printf("Enter binary input sequence (max %d bits): ", MAX_INPUTS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    int input_count = validate_input_sequence(input_buffer, inputs, MAX_INPUTS);
    if (input_count == 0) {
        printf("Invalid input sequence\n");
        return 1;
    }
    
    printf("Initial state: %d\n", sm.current_state);
    printf("Processing %d inputs...\n", input_count);
    
    simulate_state_machine(&sm, inputs, input_count);
    
    printf("Final state: %d\n", sm.current_state);
    
    int cycle_count = 1000000;
    StateMachine test_sm;
    init_state_machine(&test_sm);
    
    for (int i = 0; i < cycle_count; i++) {
        for (int j = 0; j < input_count; j++) {
            process_input(&test_sm, inputs[j]);
        }
    }
    
    printf("Completed %d simulation cycles\n", cycle_count);
    
    return 0;
}