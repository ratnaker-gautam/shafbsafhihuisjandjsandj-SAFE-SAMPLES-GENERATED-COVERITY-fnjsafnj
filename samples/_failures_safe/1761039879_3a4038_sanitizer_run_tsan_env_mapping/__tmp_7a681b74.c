//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    STATE_F,
    STATE_G,
    STATE_H,
    STATE_I,
    STATE_J
} state_t;

typedef struct {
    state_t current_state;
    char input_symbol;
    state_t next_state;
} transition_t;

static transition_t transitions[] = {
    {STATE_A, '0', STATE_B}, {STATE_A, '1', STATE_C},
    {STATE_B, '0', STATE_D}, {STATE_B, '1', STATE_E},
    {STATE_C, '0', STATE_F}, {STATE_C, '1', STATE_G},
    {STATE_D, '0', STATE_H}, {STATE_D, '1', STATE_I},
    {STATE_E, '0', STATE_J}, {STATE_E, '1', STATE_A},
    {STATE_F, '0', STATE_B}, {STATE_F, '1', STATE_C},
    {STATE_G, '0', STATE_D}, {STATE_G, '1', STATE_E},
    {STATE_H, '0', STATE_F}, {STATE_H, '1', STATE_G},
    {STATE_I, '0', STATE_H}, {STATE_I, '1', STATE_I},
    {STATE_J, '0', STATE_J}, {STATE_J, '1', STATE_A}
};

static const int num_transitions = sizeof(transitions) / sizeof(transitions[0]);

static state_t process_input(const char* input, size_t len) {
    state_t current = STATE_A;
    
    for (size_t i = 0; i < len; i++) {
        char symbol = input[i];
        int found = 0;
        
        for (int j = 0; j < num_transitions; j++) {
            if (transitions[j].current_state == current && 
                transitions[j].input_symbol == symbol) {
                current = transitions[j].next_state;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            return STATE_A;
        }
    }
    
    return current;
}

static int is_valid_input(const char* input, size_t len) {
    if (len == 0 || len > MAX_INPUT_LEN) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] != '0' && input[i] != '1') {
            return 0;
        }
    }
    
    return 1;
}

static const char* state_to_string(state_t state) {
    switch (state) {
        case STATE_A: return "A";
        case STATE_B: return "B";
        case STATE_C: return "C";
        case STATE_D: return "D";
        case STATE_E: return "E";
        case STATE_F: return "F";
        case STATE_G: return "G";
        case STATE_H: return "H";
        case STATE_I: return "I";
        case STATE_J: return "J";
        default: return "UNKNOWN";
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter binary sequence (max %d characters, only 0s and 1s): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!is_valid_input(input, len)) {
        fprintf(stderr, "Invalid input. Must contain only 0s and 1s and be 1-%d characters.\n", MAX_INPUT_LEN);
        return 1;
    }
    
    state_t final_state = process_input(input, len);
    
    printf("Input processed: %s\n", input);
    printf("Final state: %s\n", state_to_string(final_state));
    
    int accept_state = (final_state == STATE_A || final_state == STATE_E || final_state == STATE_J);
    printf("Sequence %s accepted by the state machine.\n", accept_state ? "was" : "was not");
    
    return 0;
}