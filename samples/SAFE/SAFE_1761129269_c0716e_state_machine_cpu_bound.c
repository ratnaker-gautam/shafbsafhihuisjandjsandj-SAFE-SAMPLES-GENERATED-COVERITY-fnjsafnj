//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_SYMBOLS 10

typedef enum {
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    NUM_STATES
} State;

typedef enum {
    SYMBOL_ZERO,
    SYMBOL_ONE,
    SYMBOL_TWO,
    SYMBOL_THREE,
    NUM_SYMBOLS
} Symbol;

typedef struct {
    State next_state;
    int output;
} Transition;

typedef struct {
    State current_state;
    Transition transitions[NUM_STATES][NUM_SYMBOLS];
    int state_count;
    int symbol_count;
} StateMachine;

void initialize_machine(StateMachine *sm) {
    sm->current_state = STATE_A;
    sm->state_count = NUM_STATES;
    sm->symbol_count = NUM_SYMBOLS;
    
    for (int i = 0; i < NUM_STATES; i++) {
        for (int j = 0; j < NUM_SYMBOLS; j++) {
            sm->transitions[i][j].next_state = (State)((i + j) % NUM_STATES);
            sm->transitions[i][j].output = (i * j) % 100;
        }
    }
}

int process_symbol(StateMachine *sm, Symbol sym) {
    if (sym < 0 || sym >= NUM_SYMBOLS) {
        return -1;
    }
    if (sm->current_state < 0 || sm->current_state >= NUM_STATES) {
        return -1;
    }
    
    Transition *t = &sm->transitions[sm->current_state][sym];
    int result = t->output;
    sm->current_state = t->next_state;
    
    return result;
}

int symbol_from_char(char c) {
    if (c >= '0' && c <= '3') {
        return c - '0';
    }
    return -1;
}

void process_input_string(StateMachine *sm, const char *input) {
    if (input == NULL) {
        return;
    }
    
    size_t len = strlen(input);
    if (len > MAX_INPUT_LEN) {
        return;
    }
    
    printf("Processing: %s\n", input);
    printf("State sequence: %d", sm->current_state);
    
    for (size_t i = 0; i < len; i++) {
        int sym = symbol_from_char(input[i]);
        if (sym < 0 || sym >= NUM_SYMBOLS) {
            printf("\nInvalid symbol '%c' at position %zu\n", input[i], i);
            return;
        }
        
        int output = process_symbol(sm, (Symbol)sym);
        if (output < 0) {
            printf("\nState machine error at position %zu\n", i);
            return;
        }
        
        printf(" -> %d", sm->current_state);
    }
    
    printf("\nFinal state: %d\n", sm->current_state);
}

int main() {
    StateMachine sm;
    initialize_machine(&sm);
    
    char input[MAX_INPUT_LEN + 1];
    printf("Enter input string (digits 0-3, max %d chars): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input\n");
        return 1;
    }
    
    process_input_string(&sm, input);
    
    return 0;
}