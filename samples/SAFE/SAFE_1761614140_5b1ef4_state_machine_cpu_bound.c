//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_STATES 8
#define MAX_INPUTS 16
#define MAX_TRANSITIONS 64

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

typedef enum {
    INPUT_0,
    INPUT_1,
    INPUT_2,
    INPUT_3,
    INPUT_4,
    INPUT_5,
    INPUT_6,
    INPUT_7
} Input;

typedef struct {
    State current;
    Input input;
    State next;
} Transition;

typedef struct {
    State current_state;
    Transition transitions[MAX_TRANSITIONS];
    int transition_count;
} StateMachine;

void initialize_machine(StateMachine *sm) {
    sm->current_state = STATE_A;
    sm->transition_count = 0;
}

int add_transition(StateMachine *sm, State from, Input input, State to) {
    if (sm->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    if (from >= MAX_STATES || to >= MAX_STATES || input >= MAX_INPUTS) {
        return 0;
    }
    sm->transitions[sm->transition_count].current = from;
    sm->transitions[sm->transition_count].input = input;
    sm->transitions[sm->transition_count].next = to;
    sm->transition_count++;
    return 1;
}

State process_input(StateMachine *sm, Input input) {
    if (input >= MAX_INPUTS) {
        return sm->current_state;
    }
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].input == input) {
            sm->current_state = sm->transitions[i].next;
            break;
        }
    }
    return sm->current_state;
}

void run_state_machine(StateMachine *sm, Input *inputs, int input_count) {
    if (inputs == NULL || input_count <= 0) {
        return;
    }
    for (int i = 0; i < input_count; i++) {
        if (inputs[i] >= MAX_INPUTS) {
            continue;
        }
        process_input(sm, inputs[i]);
    }
}

int validate_input_sequence(char *str, Input *inputs, int max_inputs) {
    if (str == NULL || inputs == NULL || max_inputs <= 0) {
        return 0;
    }
    int len = strlen(str);
    if (len > max_inputs) {
        return 0;
    }
    for (int i = 0; i < len; i++) {
        if (str[i] < '0' || str[i] > '7') {
            return 0;
        }
        inputs[i] = (Input)(str[i] - '0');
    }
    return len;
}

int main(void) {
    StateMachine sm;
    initialize_machine(&sm);
    
    add_transition(&sm, STATE_A, INPUT_0, STATE_B);
    add_transition(&sm, STATE_A, INPUT_1, STATE_C);
    add_transition(&sm, STATE_B, INPUT_2, STATE_D);
    add_transition(&sm, STATE_B, INPUT_3, STATE_E);
    add_transition(&sm, STATE_C, INPUT_4, STATE_F);
    add_transition(&sm, STATE_C, INPUT_5, STATE_G);
    add_transition(&sm, STATE_D, INPUT_6, STATE_H);
    add_transition(&sm, STATE_E, INPUT_7, STATE_A);
    add_transition(&sm, STATE_F, INPUT_0, STATE_B);
    add_transition(&sm, STATE_G, INPUT_1, STATE_C);
    add_transition(&sm, STATE_H, INPUT_2, STATE_D);
    
    char input_buffer[128];
    printf("Enter input sequence (digits 0-7): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    Input inputs[64];
    int input_count = validate_input_sequence(input_buffer, inputs, 64);
    
    if (input_count == 0) {
        fprintf(stderr, "Invalid input sequence\n");
        return 1;
    }
    
    run_state_machine(&sm, inputs, input_count);
    
    printf("Final state: ");
    switch (sm.current_state) {
        case STATE_A: printf("A\n"); break;
        case STATE_B: printf("B\n"); break;
        case STATE_C: printf("C\n"); break;
        case STATE_D: printf("D\n"); break;
        case STATE_E: printf("E\n"); break;
        case STATE_F: printf("F\n"); break;
        case STATE_G: printf("G\n"); break;
        case STATE_H: printf("H\n"); break;
        default: printf("Unknown\n"); break;
    }
    
    return 0;
}