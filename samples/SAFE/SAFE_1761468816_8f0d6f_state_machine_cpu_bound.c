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

static state_t transition_table[STATE_COUNT][3] = {
    {STATE_B, STATE_C, STATE_A},
    {STATE_C, STATE_D, STATE_B},
    {STATE_D, STATE_E, STATE_C},
    {STATE_E, STATE_A, STATE_D},
    {STATE_A, STATE_B, STATE_E}
};

static int process_count[STATE_COUNT] = {0};

static void process_state(state_t state) {
    volatile int computation = 0;
    for (int i = 0; i < 10000; i++) {
        computation += (state + 1) * i;
        computation ^= (computation >> 3);
    }
    process_count[state]++;
}

static input_t parse_input(char c) {
    switch (c) {
        case 'x': case 'X': return INPUT_X;
        case 'y': case 'Y': return INPUT_Y;
        case 'z': case 'Z': return INPUT_Z;
        default: return INPUT_X;
    }
}

static int validate_input_string(const char* str, size_t len) {
    if (str == NULL) return 0;
    if (len > MAX_INPUTS) return 0;
    for (size_t i = 0; i < len; i++) {
        char c = str[i];
        if (c != 'x' && c != 'X' && c != 'y' && c != 'Y' && c != 'z' && c != 'Z') {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    char input_buffer[1024];
    printf("Enter state machine inputs (x/y/z, max %d): ", MAX_INPUTS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[--input_len] = '\0';
    }
    
    if (!validate_input_string(input_buffer, input_len)) {
        fprintf(stderr, "Invalid input characters or length exceeded\n");
        return EXIT_FAILURE;
    }
    
    state_t current_state = STATE_A;
    printf("Starting state: A\n");
    
    for (size_t i = 0; i < input_len; i++) {
        input_t input = parse_input(input_buffer[i]);
        state_t next_state = transition_table[current_state][input];
        
        printf("Input: %c, Transition: %d -> %d\n", 
               input_buffer[i], current_state, next_state);
        
        process_state(current_state);
        current_state = next_state;
    }
    
    process_state(current_state);
    printf("Final state: %d\n", current_state);
    
    printf("State processing counts:\n");
    for (int i = 0; i < STATE_COUNT; i++) {
        printf("State %d: %d\n", i, process_count[i]);
    }
    
    return EXIT_SUCCESS;
}