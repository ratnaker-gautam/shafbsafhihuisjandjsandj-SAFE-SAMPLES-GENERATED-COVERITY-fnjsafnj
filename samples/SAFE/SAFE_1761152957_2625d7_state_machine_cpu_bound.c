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
    State next;
    char symbol;
} Transition;

typedef struct {
    State states[MAX_STATES];
    Transition transitions[MAX_SYMBOLS];
    int state_count;
    int transition_count;
    State current_state;
    State start_state;
    State accept_state;
} StateMachine;

void init_state_machine(StateMachine *sm) {
    sm->state_count = 4;
    sm->states[0] = STATE_A;
    sm->states[1] = STATE_B;
    sm->states[2] = STATE_C;
    sm->states[3] = STATE_D;
    
    sm->transition_count = 6;
    sm->transitions[0] = (Transition){STATE_A, STATE_B, '0'};
    sm->transitions[1] = (Transition){STATE_A, STATE_C, '1'};
    sm->transitions[2] = (Transition){STATE_B, STATE_A, '1'};
    sm->transitions[3] = (Transition){STATE_B, STATE_D, '0'};
    sm->transitions[4] = (Transition){STATE_C, STATE_D, '0'};
    sm->transitions[5] = (Transition){STATE_C, STATE_A, '1'};
    
    sm->start_state = STATE_A;
    sm->accept_state = STATE_D;
    sm->current_state = sm->start_state;
}

int process_input(StateMachine *sm, const char *input) {
    if (sm == NULL || input == NULL) {
        return 0;
    }
    
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) {
        return 0;
    }
    
    sm->current_state = sm->start_state;
    
    for (size_t i = 0; i < len; i++) {
        char symbol = input[i];
        if (!isdigit((unsigned char)symbol)) {
            return 0;
        }
        
        int transition_found = 0;
        for (int j = 0; j < sm->transition_count; j++) {
            if (sm->transitions[j].current == sm->current_state && 
                sm->transitions[j].symbol == symbol) {
                sm->current_state = sm->transitions[j].next;
                transition_found = 1;
                break;
            }
        }
        
        if (!transition_found) {
            return 0;
        }
    }
    
    return sm->current_state == sm->accept_state;
}

void cpu_intensive_validation(const char *input) {
    if (input == NULL) return;
    
    size_t len = strlen(input);
    uint32_t hash = 0;
    
    for (size_t i = 0; i < len; i++) {
        for (int j = 0; j < 1000; j++) {
            hash = (hash * 31) + (unsigned char)input[i];
        }
    }
    
    volatile uint32_t result = hash;
}

int main() {
    StateMachine sm;
    char input[MAX_INPUT_LEN + 1];
    
    init_state_machine(&sm);
    
    printf("Enter binary string (max %d digits): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] != '0' && input[i] != '1') {
            fprintf(stderr, "Invalid character: only '0' and '1' allowed\n");
            return 1;
        }
    }
    
    cpu_intensive_validation(input);
    
    int result = process_input(&sm, input);
    
    if (result) {
        printf("String accepted - reached accept state\n");
    } else {
        printf("String rejected - did not reach accept state\n");
    }
    
    return 0;
}