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
    {STATE_B, STATE_A, STATE_C},
    {STATE_C, STATE_B, STATE_D},
    {STATE_D, STATE_C, STATE_E},
    {STATE_E, STATE_D, STATE_A},
    {STATE_A, STATE_E, STATE_B}
};

static int process_count[STATE_COUNT] = {0};

static void process_state_a(void) {
    unsigned long sum = 0;
    for (int i = 0; i < 10000; i++) {
        sum += i * i;
    }
    process_count[STATE_A]++;
}

static void process_state_b(void) {
    double result = 1.0;
    for (int i = 1; i < 5000; i++) {
        result += 1.0 / (i * 1.5);
    }
    process_count[STATE_B]++;
}

static void process_state_c(void) {
    unsigned long factorial = 1;
    for (int i = 1; i <= 12; i++) {
        if (factorial > ULONG_MAX / i) break;
        factorial *= i;
    }
    process_count[STATE_C]++;
}

static void process_state_d(void) {
    long primes = 0;
    for (int i = 2; i < 1000; i++) {
        int is_prime = 1;
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) primes++;
    }
    process_count[STATE_D]++;
}

static void process_state_e(void) {
    unsigned long fib[50];
    fib[0] = 0;
    fib[1] = 1;
    for (int i = 2; i < 50; i++) {
        if (fib[i-1] > ULONG_MAX - fib[i-2]) break;
        fib[i] = fib[i-1] + fib[i-2];
    }
    process_count[STATE_E]++;
}

static void execute_state(state_t state) {
    switch (state) {
        case STATE_A: process_state_a(); break;
        case STATE_B: process_state_b(); break;
        case STATE_C: process_state_c(); break;
        case STATE_D: process_state_d(); break;
        case STATE_E: process_state_e(); break;
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
    int step_count = 0;
    
    printf("Enter state machine inputs (x/y/z characters, max %d): ", MAX_INPUTS);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_buffer);
    if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
        input_buffer[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    
    if (input_len > MAX_INPUTS) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    printf("Starting state machine simulation...\n");
    
    for (size_t i = 0; i < input_len; i++) {
        input_t input = parse_input(input_buffer[i]);
        state_t next_state = transition_table[current_state][input];
        
        printf("Step %d: State %d -> Input %c -> State %d\n", 
               step_count + 1, current_state, input_buffer[i], next_state);
        
        execute_state(current_state);
        current_state = next_state;
        step_count++;
        
        if (step_count >= MAX_INPUTS) break;
    }
    
    printf("\nState machine completed after %d steps\n", step_count);
    printf("State processing counts:\n");
    printf("State A: %d\n", process_count[STATE_A]);
    printf("State B: %d\n", process_count[STATE_B]);
    printf("State C: %d\n", process_count[STATE_C]);
    printf("State D: %d\n", process_count[STATE_D]);
    printf("State E: %d\n", process_count[STATE_E]);
    
    return 0;
}