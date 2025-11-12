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
} state_t;

typedef enum {
    INPUT_X,
    INPUT_Y,
    INPUT_Z
} input_t;

typedef struct {
    state_t current;
    state_t next;
    input_t trigger;
} transition_t;

static const transition_t transitions[] = {
    {STATE_A, STATE_B, INPUT_X},
    {STATE_A, STATE_C, INPUT_Y},
    {STATE_B, STATE_C, INPUT_X},
    {STATE_B, STATE_D, INPUT_Z},
    {STATE_C, STATE_D, INPUT_Y},
    {STATE_C, STATE_E, INPUT_Z},
    {STATE_D, STATE_A, INPUT_X},
    {STATE_D, STATE_E, INPUT_Y},
    {STATE_E, STATE_A, INPUT_Z},
    {STATE_E, STATE_B, INPUT_X}
};

static const char* state_names[] = {
    "STATE_A", "STATE_B", "STATE_C", "STATE_D", "STATE_E"
};

static const char* input_names[] = {
    "INPUT_X", "INPUT_Y", "INPUT_Z"
};

state_t process_input(state_t current, input_t input) {
    for (size_t i = 0; i < sizeof(transitions) / sizeof(transitions[0]); i++) {
        if (transitions[i].current == current && transitions[i].trigger == input) {
            return transitions[i].next;
        }
    }
    return current;
}

int parse_input(const char* str, input_t* result) {
    if (str == NULL || result == NULL) return 0;
    
    if (strcmp(str, "X") == 0 || strcmp(str, "x") == 0) {
        *result = INPUT_X;
        return 1;
    } else if (strcmp(str, "Y") == 0 || strcmp(str, "y") == 0) {
        *result = INPUT_Y;
        return 1;
    } else if (strcmp(str, "Z") == 0 || strcmp(str, "z") == 0) {
        *result = INPUT_Z;
        return 1;
    }
    return 0;
}

int main(void) {
    state_t current_state = STATE_A;
    char input_buffer[32];
    input_t inputs[MAX_INPUTS];
    size_t input_count = 0;
    
    printf("Finite State Machine Simulation\n");
    printf("Available inputs: X, Y, Z\n");
    printf("Enter inputs one per line (empty line to finish):\n");
    
    while (input_count < MAX_INPUTS) {
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input_buffer);
        if (len > 0 && input_buffer[len - 1] == '\n') {
            input_buffer[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) {
            break;
        }
        
        if (len >= sizeof(input_buffer) - 1) {
            printf("Input too long. Skipping.\n");
            continue;
        }
        
        input_t parsed_input;
        if (parse_input(input_buffer, &parsed_input)) {
            inputs[input_count++] = parsed_input;
        } else {
            printf("Invalid input '%s'. Use X, Y, or Z.\n", input_buffer);
        }
    }
    
    if (input_count == 0) {
        printf("No valid inputs provided.\n");
        return 1;
    }
    
    printf("\nInitial state: %s\n", state_names[current_state]);
    
    for (size_t i = 0; i < input_count; i++) {
        state_t next_state = process_input(current_state, inputs[i]);
        printf("Input: %s -> State: %s -> %s\n", 
               input_names[inputs[i]], 
               state_names[current_state], 
               state_names[next_state]);
        current_state = next_state;
        
        for (int j = 0; j < 1000000; j++) {
            volatile int computation = j * j;
            (void)computation;
        }
    }
    
    printf("\nFinal state: %s\n", state_names[current_state]);
    
    return 0;
}