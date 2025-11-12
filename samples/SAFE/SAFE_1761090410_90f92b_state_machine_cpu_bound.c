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
        result *= 1.0 + (1.0 / i);
    }
    process_count[STATE_B]++;
}

static void process_state_c(void) {
    unsigned long factorial = 1;
    for (int i = 1; i <= 20; i++) {
        factorial *= i;
    }
    process_count[STATE_C]++;
}

static void process_state_d(void) {
    int primes = 0;
    for (int n = 2; n <= 1000; n++) {
        int is_prime = 1;
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) {
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
    switch (c) {
        case 'x': case 'X': return INPUT_X;
        case 'y': case 'Y': return INPUT_Y;
        case 'z': case 'Z': return INPUT_Z;
        default: return INPUT_X;
    }
}

int main(void) {
    char input_buffer[1024];
    state_t current_state = STATE_A;
    int step_count = 0;
    
    printf("Enter sequence of inputs (x/y/z, max %d): ", MAX_INPUTS);
    
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
        input_len = MAX_INPUTS;
    }
    
    printf("Starting state machine...\n");
    
    for (size_t i = 0; i < input_len; i++) {
        input_t input = parse_input(input_buffer[i]);
        
        if (input < 0 || input > 2) {
            fprintf(stderr, "Invalid input character: %c\n", input_buffer[i]);
            continue;
        }
        
        if (current_state < 0 || current_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid current state\n");
            break;
        }
        
        state_t next_state = transition_table[current_state][input];
        
        if (next_state < 0 || next_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid transition\n");
            break;
        }
        
        execute_state(current_state);
        current_state = next_state;
        step_count++;
        
        printf("Step %d: Input '%c' -> State %d\n", 
               step_count, input_buffer[i], current_state);
    }
    
    printf("\nFinal state: %d\n", current_state);
    printf("State execution counts:\n");
    for (int i = 0; i < STATE_COUNT; i++) {
        printf("State %d: %d times\n", i, process_count[i]);
    }
    printf("Total steps: %d\n", step_count);
    
    return 0;
}