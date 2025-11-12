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
    State current_state;
    State next_state;
    char trigger_char;
} Transition;

typedef struct {
    State current_state;
    char input_buffer[MAX_INPUT_LEN];
    size_t input_len;
    int processed_count;
    int error_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    machine->current_state = STATE_IDLE;
    machine->input_len = 0;
    machine->processed_count = 0;
    machine->error_count = 0;
    memset(machine->input_buffer, 0, MAX_INPUT_LEN);
}

State process_transition(State current, char input, Transition *transitions, int num_transitions) {
    for (int i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == current && transitions[i].trigger_char == input) {
            return transitions[i].next_state;
        }
    }
    return STATE_ERROR;
}

int is_valid_input(char c) {
    return isalnum(c) || c == ' ' || c == '\n';
}

void process_input(StateMachine *machine, Transition *transitions, int num_transitions) {
    char input;
    int result;
    
    while ((result = getchar()) != EOF) {
        if (result == '\n') {
            continue;
        }
        
        input = (char)result;
        
        if (!is_valid_input(input)) {
            machine->error_count++;
            machine->current_state = STATE_ERROR;
            continue;
        }
        
        State next_state = process_transition(machine->current_state, input, transitions, num_transitions);
        
        switch (next_state) {
            case STATE_IDLE:
                machine->input_len = 0;
                memset(machine->input_buffer, 0, MAX_INPUT_LEN);
                break;
                
            case STATE_READING:
                if (machine->input_len < MAX_INPUT_LEN - 1) {
                    machine->input_buffer[machine->input_len++] = input;
                } else {
                    machine->error_count++;
                    next_state = STATE_ERROR;
                }
                break;
                
            case STATE_PROCESSING:
                if (machine->input_len > 0) {
                    for (size_t i = 0; i < machine->input_len; i++) {
                        for (int j = 0; j < 1000; j++) {
                            machine->input_buffer[i] = toupper(machine->input_buffer[i]);
                        }
                    }
                    machine->processed_count++;
                }
                break;
                
            case STATE_COMPLETE:
                if (machine->input_len > 0) {
                    printf("Processed: %s\n", machine->input_buffer);
                    machine->input_len = 0;
                    memset(machine->input_buffer, 0, MAX_INPUT_LEN);
                }
                break;
                
            case STATE_ERROR:
                machine->error_count++;
                machine->input_len = 0;
                memset(machine->input_buffer, 0, MAX_INPUT_LEN);
                break;
        }
        
        machine->current_state = next_state;
        
        if (machine->current_state == STATE_COMPLETE) {
            machine->current_state = STATE_IDLE;
        }
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    Transition transitions[] = {
        {STATE_IDLE, STATE_READING, 'S'},
        {STATE_IDLE, STATE_READING, 's'},
        {STATE_READING, STATE_READING, ' '},
        {STATE_READING, STATE_PROCESSING, 'E'},
        {STATE_READING, STATE_PROCESSING, 'e'},
        {STATE_READING, STATE_ERROR, 'X'},
        {STATE_READING, STATE_ERROR, 'x'},
        {STATE_PROCESSING, STATE_COMPLETE, 'C'},
        {STATE_PROCESSING, STATE_COMPLETE, 'c'},
        {STATE_ERROR, STATE_IDLE, 'R'},
        {STATE_ERROR, STATE_IDLE, 'r'}
    };
    
    int num_transitions = sizeof(transitions) / sizeof(transitions[0]);
    
    printf("State Machine Processor\n");
    printf("Commands: S/s=Start, E/e=Process, C/c=Complete, X/x=Error, R/r=Reset\n");
    printf("Input: ");
    fflush(stdout);
    
    process_input(&machine, transitions, num_transitions);
    
    printf("\nSummary: Processed %d inputs, %d errors\n", 
           machine.processed_count, machine.error_count);
    
    return 0;
}