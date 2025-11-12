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

static const Transition transitions[] = {
    {STATE_IDLE, 'S', STATE_READING},
    {STATE_READING, 'D', STATE_PROCESSING},
    {STATE_PROCESSING, 'C', STATE_COMPLETE},
    {STATE_READING, 'E', STATE_ERROR},
    {STATE_PROCESSING, 'E', STATE_ERROR},
    {STATE_ERROR, 'R', STATE_IDLE},
    {STATE_COMPLETE, 'R', STATE_IDLE}
};
static const int num_transitions = sizeof(transitions) / sizeof(transitions[0]);

State execute_state_machine(State current_state, char input) {
    for (int i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current_state && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return current_state;
}

void process_data(int iterations) {
    volatile int result = 0;
    for (int i = 0; i < iterations; i++) {
        result += (i * i) % 1000;
        for (int j = 0; j < 100; j++) {
            result = (result * 3) % 999;
        }
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    State current_state = STATE_IDLE;
    int total_iterations = 0;
    
    printf("State Machine Controller\n");
    printf("Commands: S=Start, D=Data, C=Complete, E=Error, R=Reset, Q=Quit\n");
    
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
        
        char command = toupper((unsigned char)input_buffer[0]);
        
        if (command == 'Q') {
            break;
        }
        
        State new_state = execute_state_machine(current_state, command);
        
        if (new_state != current_state) {
            printf("State transition: ");
            switch (current_state) {
                case STATE_IDLE: printf("IDLE"); break;
                case STATE_READING: printf("READING"); break;
                case STATE_PROCESSING: printf("PROCESSING"); break;
                case STATE_COMPLETE: printf("COMPLETE"); break;
                case STATE_ERROR: printf("ERROR"); break;
            }
            printf(" -> ");
            switch (new_state) {
                case STATE_IDLE: printf("IDLE"); break;
                case STATE_READING: printf("READING"); break;
                case STATE_PROCESSING: printf("PROCESSING"); break;
                case STATE_COMPLETE: printf("COMPLETE"); break;
                case STATE_ERROR: printf("ERROR"); break;
            }
            printf("\n");
            
            if (new_state == STATE_PROCESSING) {
                int iterations = 1000000;
                printf("Processing data (%d iterations)...\n", iterations);
                process_data(iterations);
                total_iterations += iterations;
                printf("Processing complete. Total iterations: %d\n", total_iterations);
            }
            
            current_state = new_state;
        } else {
            printf("Invalid transition from current state.\n");
        }
    }
    
    printf("Final state: ");
    switch (current_state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READING: printf("READING"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_COMPLETE: printf("COMPLETE"); break;
        case STATE_ERROR: printf("ERROR"); break;
    }
    printf("\nTotal processing iterations: %d\n", total_iterations);
    
    return 0;
}