//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

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
    INPUT_0,
    INPUT_1,
    INPUT_TERMINATE
} Input;

State transition_table[STATE_COUNT][3] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_D, STATE_A, STATE_B},
    {STATE_E, STATE_B, STATE_C},
    {STATE_A, STATE_D, STATE_D},
    {STATE_C, STATE_E, STATE_E}
};

Input get_next_input(uint32_t* counter) {
    if (*counter >= MAX_INPUTS) {
        return INPUT_TERMINATE;
    }
    uint32_t value = (*counter)++;
    return (value % 2 == 0) ? INPUT_0 : INPUT_1;
}

int main(void) {
    State current_state = STATE_A;
    uint32_t step_counter = 0;
    uint32_t transition_count = 0;
    
    printf("Starting state machine simulation\n");
    
    while (1) {
        Input input = get_next_input(&step_counter);
        
        if (input == INPUT_TERMINATE) {
            break;
        }
        
        if (current_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid current state\n");
            return 1;
        }
        
        if (input >= 3) {
            fprintf(stderr, "Invalid input value\n");
            return 1;
        }
        
        State next_state = transition_table[current_state][input];
        
        if (next_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid transition result\n");
            return 1;
        }
        
        printf("Step %u: State %d -> Input %d -> State %d\n",
               transition_count, current_state, input, next_state);
        
        current_state = next_state;
        transition_count++;
        
        if (transition_count > 1000000) {
            fprintf(stderr, "Transition limit exceeded\n");
            return 1;
        }
    }
    
    printf("Final state: %d\n", current_state);
    printf("Total transitions: %u\n", transition_count);
    
    return 0;
}