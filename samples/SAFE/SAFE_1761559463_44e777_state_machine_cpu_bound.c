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
    INPUT_0,
    INPUT_1,
    INPUT_2
} Input;

State transition_table[STATE_COUNT][3] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_C, STATE_D, STATE_B},
    {STATE_D, STATE_E, STATE_C},
    {STATE_E, STATE_A, STATE_D},
    {STATE_A, STATE_B, STATE_E}
};

int validate_input(const char* str, Input* inputs, size_t max_len) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > max_len) return 0;
    
    for (size_t i = 0; i < len; i++) {
        switch (str[i]) {
            case '0': inputs[i] = INPUT_0; break;
            case '1': inputs[i] = INPUT_1; break;
            case '2': inputs[i] = INPUT_2; break;
            default: return 0;
        }
    }
    return 1;
}

void process_state_machine(State start_state, Input* inputs, size_t input_count) {
    State current_state = start_state;
    
    for (size_t i = 0; i < input_count; i++) {
        if (inputs[i] < 0 || inputs[i] > 2) {
            fprintf(stderr, "Invalid input value\n");
            return;
        }
        
        current_state = transition_table[current_state][inputs[i]];
        
        for (int j = 0; j < 10000; j++) {
            int temp = j * j;
            temp = temp % 1000;
        }
    }
    
    const char* state_names[] = {"A", "B", "C", "D", "E"};
    if (current_state >= 0 && current_state < STATE_COUNT) {
        printf("Final state: %s\n", state_names[current_state]);
    } else {
        fprintf(stderr, "Invalid final state\n");
    }
}

int main(void) {
    char input_buffer[1024];
    Input inputs[MAX_INPUTS];
    
    printf("Enter input sequence (0,1,2 only): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
        len--;
    }
    
    if (!validate_input(input_buffer, inputs, MAX_INPUTS)) {
        fprintf(stderr, "Invalid input format or length\n");
        return 1;
    }
    
    State start_state = STATE_A;
    process_state_machine(start_state, inputs, len);
    
    return 0;
}