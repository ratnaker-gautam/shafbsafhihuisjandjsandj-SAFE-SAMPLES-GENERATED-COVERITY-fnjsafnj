//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D,
    STATE_E,
    STATE_F,
    STATE_G,
    STATE_H,
    STATE_END
} state_t;

typedef enum {
    EVENT_ZERO,
    EVENT_ONE,
    EVENT_TWO,
    EVENT_THREE,
    EVENT_INVALID
} event_t;

static state_t transition_table[MAX_STATES][4] = {
    {STATE_A, STATE_B, STATE_C, STATE_START},
    {STATE_B, STATE_C, STATE_D, STATE_A},
    {STATE_C, STATE_D, STATE_E, STATE_B},
    {STATE_D, STATE_E, STATE_F, STATE_C},
    {STATE_E, STATE_F, STATE_G, STATE_D},
    {STATE_F, STATE_G, STATE_H, STATE_E},
    {STATE_G, STATE_H, STATE_END, STATE_F},
    {STATE_H, STATE_END, STATE_START, STATE_G},
    {STATE_END, STATE_START, STATE_A, STATE_H},
    {STATE_START, STATE_A, STATE_B, STATE_C}
};

static event_t get_event(char input) {
    if (input >= '0' && input <= '3') {
        return (event_t)(input - '0');
    }
    return EVENT_INVALID;
}

static void process_state_machine(const char* input) {
    state_t current_state = STATE_START;
    size_t len = strnlen(input, MAX_INPUT_LEN);
    
    for (size_t i = 0; i < len; i++) {
        event_t event = get_event(input[i]);
        if (event == EVENT_INVALID) {
            printf("Invalid input character '%c' at position %zu\n", input[i], i);
            return;
        }
        
        if (current_state >= MAX_STATES) {
            printf("Invalid state transition\n");
            return;
        }
        
        current_state = transition_table[current_state][event];
        printf("State: %d, Event: %d\n", current_state, event);
        
        if (current_state == STATE_END) {
            printf("Reached end state\n");
            return;
        }
    }
    
    printf("Final state: %d\n", current_state);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    
    printf("Enter state machine input (digits 0-3): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strnlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 1 && input[0] == '\0') {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    process_state_machine(input);
    
    return 0;
}