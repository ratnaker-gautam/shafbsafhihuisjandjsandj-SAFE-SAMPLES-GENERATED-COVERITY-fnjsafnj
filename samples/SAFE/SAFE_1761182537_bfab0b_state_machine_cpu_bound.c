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

Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_READING, 'A', STATE_PROCESSING},
    {STATE_READING, 'E', STATE_ERROR},
    {STATE_PROCESSING, 'C', STATE_COMPLETE},
    {STATE_PROCESSING, 'E', STATE_ERROR},
    {STATE_ERROR, 'R', STATE_IDLE},
    {STATE_COMPLETE, 'R', STATE_IDLE}
};

int find_transition(State current, char input) {
    for (size_t i = 0; i < sizeof(transitions)/sizeof(transitions[0]); i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return (int)i;
        }
    }
    return -1;
}

void process_data(int iterations) {
    volatile int result = 0;
    for (int i = 0; i < iterations; i++) {
        result += (i * i) % 1000;
        result = result % 10000;
    }
}

int main(void) {
    State current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1];
    int iteration_count = 1000;
    
    printf("State Machine Controller\n");
    printf("Commands: S(start), A(advance), C(complete), E(error), R(reset)\n");
    
    while (1) {
        printf("Current state: ");
        switch (current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_COMPLETE: printf("COMPLETE"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf("\nEnter command: ");
        
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
        
        int transition_idx = find_transition(current_state, command);
        if (transition_idx == -1) {
            printf("Invalid command for current state\n");
            continue;
        }
        
        current_state = transitions[transition_idx].next;
        
        if (current_state == STATE_PROCESSING) {
            printf("Processing data...\n");
            process_data(iteration_count);
            printf("Processing complete\n");
        } else if (current_state == STATE_COMPLETE) {
            printf("Operation completed successfully\n");
        } else if (current_state == STATE_ERROR) {
            printf("Error state reached\n");
        }
    }
    
    printf("Exiting state machine\n");
    return 0;
}