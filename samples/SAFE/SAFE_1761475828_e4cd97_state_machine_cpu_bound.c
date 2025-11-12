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
        factorial *= i;
    }
    process_count[STATE_C]++;
}

static void process_state_d(void) {
    int primes = 0;
    for (int num = 2; num <= 1000; num++) {
        int is_prime = 1;
        for (int i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            primes++;
        }
    }
    process_count[STATE_D]++;
}

static void process_state_e(void) {
    long fib[50];
    fib[0] = 0;
    fib[1] = 1;
    for (int i = 2; i < 50; i++) {
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
        
        inputs[input_count] = parse_input(c);
        input_count++;
    }
    
    if (input_count == 0) {
        fprintf(stderr, "No valid inputs found\n");
        return 1;
    }
    
    state_t current_state = STATE_A;
    
    printf("Starting state machine...\n");
    
    for (int i = 0; i < input_count; i++) {
        input_t input = inputs[i];
        if (input < 0 || input > 2) {
            input = INPUT_X;
        }
        
        if (current_state < 0 || current_state >= STATE_COUNT) {
            current_state = STATE_A;
        }
        
        execute_state(current_state);
        
        state_t next_state = transition_table[current_state][input];
        
        if (next_state < 0 || next_state >= STATE_COUNT) {
            next_state = STATE_A;
        }
        
        printf("State %d, Input %d -> State %d\n", 
               current_state, input, next_state);
        
        current_state = next_state;
    }
    
    execute_state(current_state);
    printf("Final state: %d\n", current_state);
    
    printf("State execution counts:\n");
    for (int i = 0; i < STATE_COUNT; i++) {
        printf("State %d: %d\n", i, process_count[i]);
    }
    
    return 0;
}