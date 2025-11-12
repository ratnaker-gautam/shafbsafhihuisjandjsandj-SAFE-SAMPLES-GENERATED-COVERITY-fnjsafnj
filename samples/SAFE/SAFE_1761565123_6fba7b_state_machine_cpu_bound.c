//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 50

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
} State;

typedef struct {
    State current;
    char input;
    State next;
} Transition;

static Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_READING, 'A', STATE_PROCESSING},
    {STATE_PROCESSING, 'C', STATE_COMPLETE},
    {STATE_READING, 'X', STATE_ERROR},
    {STATE_PROCESSING, 'X', STATE_ERROR},
    {STATE_IDLE, 'X', STATE_ERROR},
    {STATE_COMPLETE, 'R', STATE_IDLE},
    {STATE_ERROR, 'R', STATE_IDLE}
};
static const int num_transitions = sizeof(transitions) / sizeof(transitions[0]);

State get_next_state(State current, char input) {
    for (int i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return current;
}

void process_state(State state, int *counter) {
    switch (state) {
        case STATE_IDLE:
            break;
        case STATE_READING:
            break;
        case STATE_PROCESSING:
            for (int i = 0; i < 1000; i++) {
                *counter += i % 7;
            }
            break;
        case STATE_COMPLETE:
            for (int i = 0; i < 500; i++) {
                *counter += i % 11;
            }
            break;
        case STATE_ERROR:
            *counter = -1;
            break;
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    State current_state = STATE_IDLE;
    int computation_counter = 0;
    
    printf("State Machine Simulation\n");
    printf("Commands: S=Start, A=Advance, C=Complete, X=Error, R=Reset, Q=Quit\n");
    
    while (1) {
        printf("Current state: %d, Counter: %d\n", current_state, computation_counter);
        printf("Enter command: ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        if (strlen(input_buffer) == 0 || input_buffer[0] == '\n') {
            continue;
        }
        
        char command = toupper(input_buffer[0]);
        
        if (command == 'Q') {
            break;
        }
        
        State next_state = get_next_state(current_state, command);
        
        if (next_state != current_state) {
            process_state(next_state, &computation_counter);
            current_state = next_state;
        } else {
            printf("Invalid transition from state %d with command '%c'\n", current_state, command);
        }
        
        if (computation_counter < 0) {
            computation_counter = 0;
        }
    }
    
    printf("Final counter value: %d\n", computation_counter);
    return 0;
}