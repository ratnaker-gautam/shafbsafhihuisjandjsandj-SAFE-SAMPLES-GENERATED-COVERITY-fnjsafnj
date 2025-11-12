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
    {STATE_READING, 'D', STATE_PROCESSING},
    {STATE_PROCESSING, 'C', STATE_COMPLETE},
    {STATE_READING, 'X', STATE_ERROR},
    {STATE_PROCESSING, 'X', STATE_ERROR},
    {STATE_ERROR, 'R', STATE_IDLE}
};
const int num_transitions = sizeof(transitions) / sizeof(transitions[0]);

State process_state_machine(State current, char input) {
    for (int i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input == input) {
            return transitions[i].next;
        }
    }
    return current;
}

void cpu_intensive_work(int iterations) {
    volatile uint64_t result = 0;
    for (int i = 0; i < iterations; i++) {
        for (int j = 0; j < 1000; j++) {
            result += (i * j) % 1000;
        }
    }
}

int main(void) {
    char input_buffer[MAX_INPUT_LEN + 1];
    State current_state = STATE_IDLE;
    int input_len;
    
    printf("State Machine Processor\n");
    printf("Commands: S=Start, D=Data, C=Complete, X=Error, R=Reset, Q=Quit\n");
    
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
        
        input_len = strlen(input_buffer);
        if (input_len > 0 && input_buffer[input_len - 1] == '\n') {
            input_buffer[input_len - 1] = '\0';
            input_len--;
        }
        
        if (input_len != 1) {
            printf("Invalid input: single character expected\n");
            continue;
        }
        
        char command = toupper(input_buffer[0]);
        
        if (command == 'Q') {
            break;
        }
        
        State new_state = process_state_machine(current_state, command);
        
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
                printf("Performing CPU-intensive work...\n");
                cpu_intensive_work(10000);
                printf("Work completed\n");
            }
            
            current_state = new_state;
        } else {
            printf("No state transition for command '%c'\n", command);
        }
        
        if (current_state == STATE_COMPLETE) {
            printf("Processing complete. Resetting to IDLE.\n");
            current_state = STATE_IDLE;
        }
    }
    
    printf("Exiting state machine processor\n");
    return 0;
}