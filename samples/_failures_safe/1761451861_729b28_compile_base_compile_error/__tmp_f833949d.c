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

static void process_state_a(int cycles) {
    for (int i = 0; i < cycles; i++) {
        volatile double x = 3.14159 * i;
        x = x * x / (i + 1);
    }
    process_count[STATE_A]++;
}

static void process_state_b(int cycles) {
    for (int i = 0; i < cycles; i++) {
        volatile long y = i * i;
        y = y % 10007;
        y = y * y;
    }
    process_count[STATE_B]++;
}

static void process_state_c(int cycles) {
    for (int i = 0; i < cycles; i++) {
        volatile unsigned int z = i + 12345;
        z = z ^ (z << 13);
        z = z ^ (z >> 17);
        z = z ^ (z << 5);
    }
    process_count[STATE_C]++;
}

static void process_state_d(int cycles) {
    for (int i = 0; i < cycles; i++) {
        volatile double w = 2.71828 * i;
        for (int j = 0; j < 10; j++) {
            w = w * 1.1;
        }
    }
    process_count[STATE_D]++;
}

static void process_state_e(int cycles) {
    for (int i = 0; i < cycles; i++) {
        volatile unsigned long v = i * 7919;
        v = (v * 1103515245 + 12345) % 2147483647;
    }
    process_count[STATE_E]++;
}

static void execute_state(state_t state, int cycles) {
    switch (state) {
        case STATE_A: process_state_a(cycles); break;
        case STATE_B: process_state_b(cycles); break;
        case STATE_C: process_state_c(cycles); break;
        case STATE_D: process_state_d(cycles); break;
        case STATE_E: process_state_e(cycles); break;
    }
}

static input_t parse_input(char c) {
    if (c == 'x' || c == 'X') return INPUT_X;
    if (c == 'y' || c == 'Y') return INPUT_Y;
    if (c == 'z' || c == 'Z') return INPUT_Z;
    return INPUT_X;
}

int main(void) {
    char input_buffer[1024];
    input_t inputs[MAX_INPUTS];
    int input_count = 0;
    
    printf("Enter state machine inputs (x/y/z, max %d): ", MAX_INPUTS);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len-1] == '\n') {
        input_buffer[len-1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    
    for (size_t i = 0; i < len && input_count < MAX_INPUTS; i++) {
        char c = input_buffer[i];
        if (c == ' ' || c == '\t' || c == ',') continue;
        inputs[input_count++] = parse_input(c);
    }
    
    if (input_count == 0) {
        fprintf(stderr, "No valid inputs found\n");
        return 1;
    }
    
    state_t current_state = STATE_A;
    int total_cycles = 0;
    
    for (int i = 0; i < input_count; i++) {
        input_t input = inputs[i];
        if (input < 0 || input > 2) input = INPUT_X;
        
        state_t next_state = transition_table[current_state][input];
        if (next_state < 0 || next_state >= STATE_COUNT) {
            next_state = STATE_A;
        }
        
        int cycles = 10000 + (i * 123) % 5000;
        if (cycles < 1000) cycles = 1000;
        if (cycles > 50000) cycles = 50000;
        
        execute_state(current_state, cycles);
        total_cycles += cycles;
        current_state = next_state;
    }
    
    execute_state(current_state, 15000);
    total_cycles += 15000;
    
    printf("State machine execution completed\n");
    printf("Total computation cycles: %d\n", total_cycles);
    printf("State visit counts:\n");
    printf("State A: %d\n", process_count[STATE_A]);
    printf("State B: %d\n", process_count[STATE_B]);
    printf("