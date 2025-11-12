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

State transition_table[STATE_COUNT][3] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_C, STATE_D, STATE_B},
    {STATE_D, STATE_E, STATE_C},
    {STATE_E, STATE_A, STATE_D},
    {STATE_A, STATE_B, STATE_E}
};

int process_inputs(Input *inputs, int count) {
    if (inputs == NULL || count <= 0 || count > MAX_INPUTS) {
        return -1;
    }
    
    State current_state = STATE_A;
    int cycles = 0;
    
    for (int i = 0; i < count; i++) {
        if (inputs[i] < INPUT_X || inputs[i] > INPUT_Z) {
            return -1;
        }
        
        current_state = transition_table[current_state][inputs[i]];
        
        for (int j = 0; j < 1000; j++) {
            cycles += (current_state * j) % 100;
        }
    }
    
    return cycles;
}

int main(void) {
    printf("Enter number of inputs (1-%d): ", MAX_INPUTS);
    
    int count;
    if (scanf("%d", &count) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (count <= 0 || count > MAX_INPUTS) {
        printf("Invalid count\n");
        return 1;
    }
    
    Input *inputs = malloc(count * sizeof(Input));
    if (inputs == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter inputs (0=X, 1=Y, 2=Z):\n");
    for (int i = 0; i < count; i++) {
        int value;
        if (scanf("%d", &value) != 1) {
            printf("Invalid input\n");
            free(inputs);
            return 1;
        }
        
        if (value < 0 || value > 2) {
            printf("Invalid input value\n");
            free(inputs);
            return 1;
        }
        
        inputs[i] = (Input)value;
    }
    
    int result = process_inputs(inputs, count);
    if (result == -1) {
        printf("Error processing inputs\n");
        free(inputs);
        return 1;
    }
    
    printf("Processing completed. Cycles: %d\n", result);
    printf("Final state: ");
    
    State current_state = STATE_A;
    for (int i = 0; i < count; i++) {
        current_state = transition_table[current_state][inputs[i]];
    }
    
    switch (current_state) {
        case STATE_A: printf("A\n"); break;
        case STATE_B: printf("B\n"); break;
        case STATE_C: printf("C\n"); break;
        case STATE_D: printf("D\n"); break;
        case STATE_E: printf("E\n"); break;
    }
    
    free(inputs);
    return 0;
}