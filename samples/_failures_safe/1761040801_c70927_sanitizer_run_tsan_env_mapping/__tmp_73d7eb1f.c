//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
    {STATE_IDLE, 'Q', STATE_COMPLETE},
    {STATE_READING, 'A', STATE_PROCESSING},
    {STATE_READING, 'Q', STATE_COMPLETE},
    {STATE_PROCESSING, 'C', STATE_READING},
    {STATE_PROCESSING, 'Q', STATE_COMPLETE},
    {STATE_PROCESSING, 'E', STATE_ERROR}
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

void process_state(State state, int* data) {
    switch (state) {
        case STATE_IDLE:
            *data = 0;
            break;
        case STATE_READING:
            (*data)++;
            break;
        case STATE_PROCESSING:
            for (int i = 0; i < 1000; i++) {
                *data += (i % 2 == 0) ? 1 : -1;
            }
            break;
        case STATE_ERROR:
            *data = -1;
            break;
        default:
            break;
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    State current_state = STATE_IDLE;
    int data_value = 0;
    
    printf("State Machine Simulation\n");
    printf("Commands: S=Start, A=Process, C=Continue, E=Error, Q=Quit\n");
    
    while (current_state != STATE_COMPLETE && current_state != STATE_ERROR) {
        printf("Current state: %d, Data: %d\n", current_state, data_value);
        printf("Enter command: ");
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            break;
        }
        
        if (strlen(input_buffer) == 0 || input_buffer[0] == '\n') {
            continue;
        }
        
        char command = toupper(input_buffer[0]);
        
        if (command != 'S' && command != 'A' && command != 'C' && 
            command != 'E' && command != 'Q') {
            printf("Invalid command. Use S, A, C, E, or Q.\n");
            continue;
        }
        
        State next_state = get_next_state(current_state, command);
        
        if (next_state == current_state) {
            printf("Invalid transition from state %d with command %c\n", 
                   current_state, command);
            continue;
        }
        
        current_state = next_state;
        process_state(current_state, &data_value);
    }
    
    printf("Final state: %d, Final data: %d\n", current_state, data_value);
    
    if (current_state == STATE_ERROR) {
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}