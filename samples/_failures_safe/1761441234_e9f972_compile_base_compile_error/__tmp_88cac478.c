//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_STATES 8
#define MAX_SYMBOLS 16

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H
} State;

typedef struct {
    State current;
    char symbol;
    State next;
} Transition;

typedef struct {
    State states[MAX_STATES];
    char symbols[MAX_SYMBOLS];
    Transition transitions[MAX_STATES * MAX_SYMBOLS];
    size_t state_count;
    size_t symbol_count;
    size_t transition_count;
    State start_state;
    State current_state;
} StateMachine;

void init_state_machine(StateMachine *sm) {
    sm->state_count = 8;
    sm->states[0] = STATE_A;
    sm->states[1] = STATE_B;
    sm->states[2] = STATE_C;
    sm->states[3] = STATE_D;
    sm->states[4] = STATE_E;
    sm->states[5] = STATE_F;
    sm->states[6] = STATE_G;
    sm->states[7] = STATE_H;
    
    sm->symbol_count = 4;
    sm->symbols[0] = '0';
    sm->symbols[1] = '1';
    sm->symbols[2] = '2';
    sm->symbols[3] = '3';
    
    sm->transition_count = 0;
    for (size_t i = 0; i < sm->state_count; i++) {
        for (size_t j = 0; j < sm->symbol_count; j++) {
            if (sm->transition_count < MAX_STATES * MAX_SYMBOLS) {
                sm->transitions[sm->transition_count].current = sm->states[i];
                sm->transitions[sm->transition_count].symbol = sm->symbols[j];
                sm->transitions[sm->transition_count].next = sm->states[(i + j + 1) % sm->state_count];
                sm->transition_count++;
            }
        }
    }
    
    sm->start_state = STATE_A;
    sm->current_state = STATE_A;
}

int is_valid_symbol(StateMachine *sm, char symbol) {
    for (size_t i = 0; i < sm->symbol_count; i++) {
        if (sm->symbols[i] == symbol) {
            return 1;
        }
    }
    return 0;
}

State process_symbol(StateMachine *sm, char symbol) {
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current == sm->current_state && 
            sm->transitions[i].symbol == symbol) {
            return sm->transitions[i].next;
        }
    }
    return sm->current_state;
}

void process_input_string(StateMachine *sm, const char *input) {
    size_t len = strlen(input);
    if (len > MAX_INPUT_LEN) {
        printf("Input too long\n");
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_symbol(sm, input[i])) {
            printf("Invalid symbol '%c' at position %zu\n", input[i], i);
            return;
        }
    }
    
    sm->current_state = sm->start_state;
    printf("Start state: %d\n", sm->current_state);
    
    for (size_t i = 0; i < len; i++) {
        State next_state = process_symbol(sm, input[i]);
        printf("Input '%c': %d -> %d\n", input[i], sm->current_state, next_state);
        sm->current_state = next_state;
    }
    
    printf("Final state: %d\n", sm->current_state);
}

void benchmark_state_machine(StateMachine *sm) {
    const char *test_input = "0123012301230123";
    size_t iterations = 1000000;
    
    printf("Benchmarking with %zu iterations...\n", iterations);
    
    for (size_t iter = 0; iter < iterations; iter++) {
        sm->current_state = sm->start_state;
        size_t len = strlen(test_input);
        
        for (size_t i = 0; i < len; i++) {
            sm->current_state = process_symbol(sm, test_input[i]);
        }
    }
    
    printf("Benchmark completed. Final state: %d\n", sm->current_state);
}

int main(void) {
    StateMachine sm;
    char input[MAX_INPUT_LEN + 1];
    
    init_state_machine(&sm);
    
    printf("State Machine Simulator\n");
    printf("Valid symbols: ");
    for (size_t i = 0; i < sm.symbol_count; i++) {
        printf("%c ", sm.symbols[i]);
    }
    printf("\n");
    
    printf("Enter input string (max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }