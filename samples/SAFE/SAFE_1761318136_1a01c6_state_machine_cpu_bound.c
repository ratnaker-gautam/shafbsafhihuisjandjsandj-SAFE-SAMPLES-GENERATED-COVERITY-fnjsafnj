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
    INPUT_0,
    INPUT_1,
    INPUT_COUNT
} input_t;

static const state_t transition_table[STATE_COUNT][INPUT_COUNT] = {
    {STATE_B, STATE_C},
    {STATE_D, STATE_A},
    {STATE_E, STATE_B},
    {STATE_A, STATE_C},
    {STATE_D, STATE_E}
};

static int validate_input(const char* str, input_t* inputs, size_t max_len) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > max_len) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '0') {
            inputs[i] = INPUT_0;
        } else if (str[i] == '1') {
            inputs[i] = INPUT_1;
        } else {
            return 0;
        }
    }
    return 1;
}

static state_t process_state_machine(state_t start_state, const input_t* inputs, size_t input_count) {
    state_t current_state = start_state;
    
    for (size_t i = 0; i < input_count; i++) {
        if (inputs[i] >= INPUT_COUNT) continue;
        current_state = transition_table[current_state][inputs[i]];
        if (current_state >= STATE_COUNT) current_state = STATE_A;
    }
    
    return current_state;
}

static const char* state_to_string(state_t state) {
    switch (state) {
        case STATE_A: return "A";
        case STATE_B: return "B";
        case STATE_C: return "C";
        case STATE_D: return "D";
        case STATE_E: return "E";
        default: return "A";
    }
}

int main(void) {
    char input_buffer[1024];
    input_t inputs[MAX_INPUTS];
    
    printf("Enter binary input sequence (0s and 1s, max %d characters): ", MAX_INPUTS);
    
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
        fprintf(stderr, "Invalid input. Only 0s and 1s allowed, max %d characters.\n", MAX_INPUTS);
        return 1;
    }
    
    state_t final_state = process_state_machine(STATE_A, inputs, len);
    
    printf("Final state: %s\n", state_to_string(final_state));
    
    size_t iterations = 1000000;
    state_t test_state = STATE_A;
    for (size_t i = 0; i < iterations; i++) {
        for (size_t j = 0; j < len; j++) {
            test_state = transition_table[test_state][inputs[j]];
            if (test_state >= STATE_COUNT) test_state = STATE_A;
        }
    }
    
    printf("Validated %zu iterations\n", iterations);
    
    return 0;
}