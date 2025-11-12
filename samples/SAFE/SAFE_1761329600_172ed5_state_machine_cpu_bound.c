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
    char trigger;
    State next;
} Transition;

static Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_READING, 'P', STATE_PROCESSING},
    {STATE_READING, 'E', STATE_ERROR},
    {STATE_PROCESSING, 'C', STATE_COMPLETE},
    {STATE_PROCESSING, 'E', STATE_ERROR},
    {STATE_ERROR, 'R', STATE_IDLE},
    {STATE_COMPLETE, 'R', STATE_IDLE}
};
static const int num_transitions = sizeof(transitions) / sizeof(transitions[0]);

State get_next_state(State current, char input) {
    for (int i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].trigger == input) {
            return transitions[i].next;
        }
    }
    return current;
}

void process_data(int iterations) {
    volatile double result = 1.0;
    for (int i = 0; i < iterations; i++) {
        for (int j = 0; j < 1000; j++) {
            result = result * 1.0001 + 0.0001;
        }
    }
}

int main(void) {
    State current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN + 1];
    int iteration_count = 10000;
    
    printf("State Machine Controller\n");
    printf("Commands: S=Start, P=Process, C=Complete, E=Error, R=Reset, Q=Quit\n");
    
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
        
        if (strlen(input_buffer) == 0) {
            continue;
        }
        
        char command = toupper(input_buffer[0]);
        
        if (command == 'Q') {
            break;
        }
        
        State next_state = get_next_state(current_state, command);
        
        if (next_state == current_state) {
            printf("Invalid transition from current state\n");
            continue;
        }
        
        current_state = next_state;
        
        switch (current_state) {
            case STATE_PROCESSING:
                printf("Processing data...\n");
                process_data(iteration_count);
                printf("Processing complete\n");
                break;
                
            case STATE_COMPLETE:
                printf("Operation completed successfully\n");
                break;
                
            case STATE_ERROR:
                printf("Error state entered\n");
                break;
                
            case STATE_IDLE:
                printf("Reset to idle state\n");
                break;
                
            default:
                break;
        }
    }
    
    printf("Program terminated\n");
    return 0;
}