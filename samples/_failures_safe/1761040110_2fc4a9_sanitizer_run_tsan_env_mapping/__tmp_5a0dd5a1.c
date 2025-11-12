//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    Input input;
    State next;
} Transition;

static const Transition transitions[] = {
    {STATE_A, INPUT_X, STATE_B},
    {STATE_A, INPUT_Y, STATE_C},
    {STATE_B, INPUT_X, STATE_C},
    {STATE_B, INPUT_Y, STATE_D},
    {STATE_C, INPUT_X, STATE_D},
    {STATE_C, INPUT_Z, STATE_A},
    {STATE_D, INPUT_Y, STATE_E},
    {STATE_D, INPUT_Z, STATE_B},
    {STATE_E, INPUT_X, STATE_A},
    {STATE_E, INPUT_Z, STATE_C}
};

static const int transition_count = sizeof(transitions) / sizeof(transitions[0]);

State process_transition(State current, Input input) {
    for (int i = 0; i < transition_count; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return current;
}

Input char_to_input(char c) {
    switch (c) {
        case 'x': return INPUT_X;
        case 'y': return INPUT_Y;
        case 'z': return INPUT_Z;
        default: return INPUT_X;
    }
}

char state_to_char(State s) {
    switch (s) {
        case STATE_A: return 'A';
        case STATE_B: return 'B';
        case STATE_C: return 'C';
        case STATE_D: return 'D';
        case STATE_E: return 'E';
        default: return 'A';
    }
}

void simulate_state_machine(const char* input_sequence) {
    if (input_sequence == NULL) {
        printf("Error: Null input sequence\n");
        return;
    }
    
    size_t len = strlen(input_sequence);
    if (len == 0) {
        printf("Error: Empty input sequence\n");
        return;
    }
    
    if (len > MAX_INPUTS) {
        printf("Error: Input sequence too long\n");
        return;
    }
    
    State current_state = STATE_A;
    printf("Initial state: %c\n", state_to_char(current_state));
    
    for (size_t i = 0; i < len; i++) {
        Input input = char_to_input(input_sequence[i]);
        State next_state = process_transition(current_state, input);
        printf("Input: %c -> State: %c\n", input_sequence[i], state_to_char(next_state));
        current_state = next_state;
    }
    
    printf("Final state: %c\n", state_to_char(current_state));
}

int main() {
    char input_buffer[MAX_INPUTS + 1];
    
    printf("Enter input sequence (x, y, z only): ");
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Error: No input provided\n");
        return EXIT_FAILURE;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (input_buffer[i] != 'x' && input_buffer[i] != 'y' && input_buffer[i] != 'z') {
            printf("Error: Invalid character '%c' at position %zu\n", input_buffer[i], i + 1);
            return EXIT_FAILURE;
        }
    }
    
    simulate_state_machine(input_buffer);
    
    return EXIT_SUCCESS;
}