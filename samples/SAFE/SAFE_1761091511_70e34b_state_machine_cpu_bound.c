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
        result += 1.0 / (i * 1.5);
    }
    process_count[STATE_B]++;
}

static void process_state_c(void) {
    long factorial = 1;
    for (int i = 1; i <= 12; i++) {
        factorial *= i;
    }
    process_count[STATE_C]++;
}

static void process_state_d(void) {
    unsigned int primes = 0;
    for (int i = 2; i < 1000; i++) {
        int is_prime = 1;
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0) {
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

static void (*state_processors[STATE_COUNT])(void) = {
    process_state_a,
    process_state_b,
    process_state_c,
    process_state_d,
    process_state_e
};

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
        if (input_buffer[i] == 'x' || input_buffer[i] == 'X' ||
            input_buffer[i] == 'y' || input_buffer[i] == 'Y' ||
            input_buffer[i] == 'z' || input_buffer[i] == 'Z') {
            inputs[input_count++] = parse_input(input_buffer[i]);
        }
    }
    
    if (input_count == 0) {
        fprintf(stderr, "No valid inputs found\n");
        return 1;
    }
    
    state_t current_state = STATE_A;
    
    printf("Starting state machine with %d inputs\n", input_count);
    
    for (int i = 0; i < input_count; i++) {
        input_t current_input = inputs[i];
        
        if (current_input < 0 || current_input >= 3) {
            fprintf(stderr, "Invalid input value\n");
            return 1;
        }
        
        state_t next_state = transition_table[current_state][current_input];
        
        if (next_state < 0 || next_state >= STATE_COUNT) {
            fprintf(stderr, "Invalid state transition\n");
            return 1;
        }
        
        state_processors[current_state]();
        
        printf("Step %d: State %d -> Input %d -> State %d\n", 
               i + 1, current_state, current_input, next_state);
        
        current_state = next_state;
    }
    
    state_processors[current_state]();
    printf("Final state: %d\n", current_state);
    
    printf("State processing counts:\n");
    for (int i = 0; i < STATE_COUNT; i++) {
        printf("State %d: %d times\n", i, process_count[i]);
    }
    
    return 0;
}