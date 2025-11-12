//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define STATE_COUNT 5
#define MAX_INPUTS 1000

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E
} State;

typedef enum {
    INPUT_X,
    INPUT_Y,
    INPUT_Z
} Input;

typedef struct {
    State current_state;
    uint64_t counter;
    uint32_t transitions;
} StateMachine;

State get_next_state(State current, Input input) {
    static const State transition_table[STATE_COUNT][3] = {
        {STATE_B, STATE_C, STATE_A},
        {STATE_C, STATE_D, STATE_B},
        {STATE_D, STATE_E, STATE_C},
        {STATE_E, STATE_A, STATE_D},
        {STATE_A, STATE_B, STATE_E}
    };
    
    if (current >= STATE_COUNT || input >= 3) {
        return STATE_A;
    }
    
    return transition_table[current][input];
}

void process_state(StateMachine *sm, Input input) {
    if (sm == NULL) return;
    
    State next_state = get_next_state(sm->current_state, input);
    
    if (next_state != sm->current_state) {
        sm->transitions++;
    }
    
    sm->current_state = next_state;
    
    switch (sm->current_state) {
        case STATE_A:
            sm->counter += 1;
            break;
        case STATE_B:
            sm->counter += 2;
            break;
        case STATE_C:
            sm->counter += 3;
            break;
        case STATE_D:
            sm->counter += 5;
            break;
        case STATE_E:
            sm->counter += 7;
            break;
    }
    
    if (sm->counter > UINT64_MAX - 100) {
        sm->counter = 0;
    }
}

Input parse_input(char c) {
    switch (c) {
        case 'x': case 'X':
            return INPUT_X;
        case 'y': case 'Y':
            return INPUT_Y;
        case 'z': case 'Z':
            return INPUT_Z;
        default:
            return INPUT_X;
    }
}

int main(void) {
    StateMachine sm;
    sm.current_state = STATE_A;
    sm.counter = 0;
    sm.transitions = 0;
    
    char input_buffer[1024];
    printf("Enter sequence of inputs (x/y/z, max %d): ", MAX_INPUTS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len > MAX_INPUTS) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        Input input = parse_input(input_buffer[i]);
        process_state(&sm, input);
    }
    
    for (int i = 0; i < 1000000; i++) {
        Input random_input = (Input)(i % 3);
        process_state(&sm, random_input);
    }
    
    printf("Final state: %d\n", sm.current_state);
    printf("Counter value: %lu\n", sm.counter);
    printf("Total transitions: %u\n", sm.transitions);
    
    return 0;
}