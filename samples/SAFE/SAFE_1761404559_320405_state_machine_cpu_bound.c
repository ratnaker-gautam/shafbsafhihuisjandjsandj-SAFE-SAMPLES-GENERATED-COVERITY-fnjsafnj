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

static void process_state_a(void) {
    volatile int result = 0;
    for (int i = 0; i < 10000; i++) {
        result += i * i;
    }
    process_count[STATE_A]++;
}

static void process_state_b(void) {
    volatile int result = 0;
    for (int i = 0; i < 15000; i++) {
        result += i * (i + 1);
    }
    process_count[STATE_B]++;
}

static void process_state_c(void) {
    volatile int result = 0;
    for (int i = 0; i < 20000; i++) {
        result += i * (i - 1);
    }
    process_count[STATE_C]++;
}

static void process_state_d(void) {
    volatile int result = 0;
    for (int i = 0; i < 25000; i++) {
        result += i * i * i;
    }
    process_count[STATE_D]++;
}

static void process_state_e(void) {
    volatile int result = 0;
    for (int i = 0; i < 30000; i++) {
        result += i * i * (i + 1);
    }
    process_count[STATE_E]++;
}

static void execute_state_processing(state_t state) {
    switch (state) {
        case STATE_A: process_state_a(); break;
        case STATE_B: process_state_b(); break;
        case STATE_C: process_state_c(); break;
        case STATE_D: process_state_d(); break;
        case STATE_E: process_state_e(); break;
        default: break;
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
    state_t current_state = STATE_A;
    int input_count = 0;
    
    printf("Enter state machine inputs (x/y/z, max %d): ", MAX_INPUTS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    
    for (size_t i = 0; i < len && input_count < MAX_INPUTS; i++) {
        char c = input_buffer[i];
        if (c != 'x' && c != 'X' && c != 'y' && c != 'Y' && c != 'z' && c != 'Z') {
            continue;
        }
        
        input_t input = parse_input(c);
        state_t next_state = transition_table[current_state][input];
        
        if (next_state < STATE_A || next_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid state transition\n");
            return 1;
        }
        
        execute_state_processing(current_state);
        current_state = next_state;
        input_count++;
    }
    
    printf("Final state: %d\n", current_state);
    printf("State processing counts:\n");
    for (int i = 0; i < STATE_COUNT; i++) {
        printf("State %d: %d\n", i, process_count[i]);
    }
    printf("Total inputs processed: %d\n", input_count);
    
    return 0;
}