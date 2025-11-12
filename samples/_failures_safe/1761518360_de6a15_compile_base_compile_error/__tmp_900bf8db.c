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

State process_input(StateMachine *sm, char input) {
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            return sm->transitions[i].next;
        }
    }
    return sm->current_state;
}

void print_state(State state) {
    switch(state) {
        case STATE_A: printf("A"); break;
        case STATE_B: printf("B"); break;
        case STATE_C: printf("C"); break;
        case STATE_D: printf("D"); break;
        case STATE_E: printf("E"); break;
        case STATE_F: printf("F"); break;
        case STATE_G: printf("G"); break;
        case STATE_H: printf("H"); break;
    }
}

int validate_input_string(const char *input) {
    if (input == NULL) return 0;
    
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUTS) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] != '0' && input[i] != '1') {
            return 0;
        }
    }
    return 1;
}

void generate_test_input(char *buffer, size_t length) {
    if (buffer == NULL || length == 0 || length > MAX_INPUTS) return;
    
    for (size_t i = 0; i < length; i++) {
        buffer[i] = (rand() % 2) ? '1' : '0';
    }
    buffer[length] = '\0';
}

int main(void) {
    StateMachine sm;
    initialize_machine(&sm);
    
    char input_buffer[MAX_INPUTS + 1];
    int test_count = 1000000;
    
    printf("State Machine Simulation\n");
    printf("Initial state: ");
    print_state(sm.current_state);
    printf("\n");
    
    for (int test = 0; test < test_count; test++) {
        size_t input_length = (rand() % 64) + 1;
        if (input_length > MAX_INPUTS) input_length = MAX_INPUTS;
        
        generate_test_input(input_buffer, input_length);
        
        if (!validate_input_string(input_buffer)) {
            continue;
        }
        
        State original_state = sm.current_state;
        
        for (size_t i = 0; i < input_length; i++) {
            sm.current_state = process_input(&sm, input_buffer[i]);
        }
        
        if (test % 100000 == 0) {
            printf("Test %d: Input '%.16s