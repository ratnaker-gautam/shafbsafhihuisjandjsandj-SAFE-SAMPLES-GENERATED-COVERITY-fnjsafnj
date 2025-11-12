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
    State current;
    State next;
    Input trigger;
} Transition;

static const Transition transitions[] = {
    {STATE_A, STATE_B, INPUT_X},
    {STATE_A, STATE_C, INPUT_Y},
    {STATE_B, STATE_C, INPUT_X},
    {STATE_B, STATE_D, INPUT_Y},
    {STATE_C, STATE_D, INPUT_X},
    {STATE_C, STATE_E, INPUT_Y},
    {STATE_D, STATE_E, INPUT_X},
    {STATE_D, STATE_A, INPUT_Y},
    {STATE_E, STATE_A, INPUT_X},
    {STATE_E, STATE_B, INPUT_Y}
};

static const char* state_names[] = {
    "STATE_A", "STATE_B", "STATE_C", "STATE_D", "STATE_E"
};

static const char* input_names[] = {
    "INPUT_X", "INPUT_Y", "INPUT_Z"
};

State process_transition(State current, Input input) {
    for (size_t i = 0; i < sizeof(transitions)/sizeof(transitions[0]); i++) {
        if (transitions[i].current == current && transitions[i].trigger == input) {
            return transitions[i].next;
        }
    }
    return current;
}

Input parse_input(const char* str) {
    if (str == NULL) return INPUT_Z;
    if (strcmp(str, "X") == 0) return INPUT_X;
    if (strcmp(str, "Y") == 0) return INPUT_Y;
    if (strcmp(str, "Z") == 0) return INPUT_Z;
    return INPUT_Z;
}

void simulate_state_machine(Input* inputs, size_t count) {
    if (inputs == NULL || count == 0 || count > MAX_INPUTS) return;
    
    State current = STATE_A;
    unsigned long long computation = 1;
    
    for (size_t i = 0; i < count; i++) {
        printf("Step %zu: %s -> Input: %s -> ", 
               i + 1, state_names[current], input_names[inputs[i]]);
        
        State next = process_transition(current, inputs[i]);
        printf("%s\n", state_names[next]);
        
        for (int j = 0; j < 1000; j++) {
            computation = (computation * 3) % 1000000007;
        }
        
        current = next;
    }
    
    printf("Final state: %s\n", state_names[current]);
    printf("Computation result: %llu\n", computation);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input_sequence>\n", argv[0]);
        printf("Example: %s X Y Z X Y\n", argv[0]);
        return 1;
    }
    
    Input inputs[MAX_INPUTS];
    size_t input_count = 0;
    
    for (int i = 1; i < argc && input_count < MAX_INPUTS; i++) {
        if (argv[i] == NULL) continue;
        if (strlen(argv[i]) != 1) continue;
        
        inputs[input_count] = parse_input(argv[i]);
        input_count++;
    }
    
    if (input_count == 0) {
        printf("Error: No valid inputs provided\n");
        return 1;
    }
    
    simulate_state_machine(inputs, input_count);
    
    return 0;
}