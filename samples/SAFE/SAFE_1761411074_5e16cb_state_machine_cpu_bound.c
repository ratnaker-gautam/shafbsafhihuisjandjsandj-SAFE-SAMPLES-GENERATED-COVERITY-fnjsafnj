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
    STATE_OUTPUT,
    STATE_ERROR,
    STATE_DONE
} State;

typedef struct {
    State current;
    char input_char;
    State next;
} Transition;

typedef struct {
    State current_state;
    char input_buffer[MAX_INPUT_LEN];
    size_t input_len;
    char output_buffer[MAX_INPUT_LEN];
    size_t output_len;
    int error_code;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    machine->current_state = STATE_IDLE;
    machine->input_len = 0;
    machine->output_len = 0;
    machine->error_code = 0;
    memset(machine->input_buffer, 0, MAX_INPUT_LEN);
    memset(machine->output_buffer, 0, MAX_INPUT_LEN);
}

int add_transition(Transition transitions[], int *count, State current, char input, State next) {
    if (*count >= MAX_TRANSITIONS) {
        return 0;
    }
    transitions[*count].current = current;
    transitions[*count].input_char = input;
    transitions[*count].next = next;
    (*count)++;
    return 1;
}

State get_next_state(Transition transitions[], int count, State current, char input) {
    for (int i = 0; i < count; i++) {
        if (transitions[i].current == current && transitions[i].input_char == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

int process_character(StateMachine *machine, char c) {
    static Transition transitions[MAX_TRANSITIONS];
    static int transition_count = 0;
    
    if (transition_count == 0) {
        add_transition(transitions, &transition_count, STATE_IDLE, 'S', STATE_READING);
        add_transition(transitions, &transition_count, STATE_READING, 'E', STATE_PROCESSING);
        add_transition(transitions, &transition_count, STATE_PROCESSING, 'P', STATE_OUTPUT);
        add_transition(transitions, &transition_count, STATE_OUTPUT, 'X', STATE_DONE);
        for (char ch = 'A'; ch <= 'Z'; ch++) {
            if (ch != 'S' && ch != 'E' && ch != 'P' && ch != 'X') {
                add_transition(transitions, &transition_count, STATE_READING, ch, STATE_READING);
            }
        }
    }
    
    State next_state = get_next_state(transitions, transition_count, machine->current_state, c);
    
    switch (machine->current_state) {
        case STATE_IDLE:
            if (c == 'S') {
                machine->input_len = 0;
                machine->output_len = 0;
            }
            break;
            
        case STATE_READING:
            if (machine->input_len < MAX_INPUT_LEN - 1) {
                machine->input_buffer[machine->input_len++] = c;
                machine->input_buffer[machine->input_len] = '\0';
            } else {
                next_state = STATE_ERROR;
                machine->error_code = 1;
            }
            break;
            
        case STATE_PROCESSING:
            if (machine->input_len > 0) {
                for (size_t i = 0; i < machine->input_len; i++) {
                    if (machine->output_len < MAX_INPUT_LEN - 1) {
                        machine->output_buffer[machine->output_len++] = toupper(machine->input_buffer[i]);
                    } else {
                        next_state = STATE_ERROR;
                        machine->error_code = 2;
                        break;
                    }
                }
                machine->output_buffer[machine->output_len] = '\0';
            }
            break;
            
        case STATE_OUTPUT:
            printf("Processed: %s\n", machine->output_buffer);
            break;
            
        case STATE_ERROR:
            printf("Error %d occurred\n", machine->error_code);
            break;
            
        case STATE_DONE:
            break;
    }
    
    machine->current_state = next_state;
    return (next_state != STATE_ERROR && next_state != STATE_DONE);
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    char input_sequence[] = "SHelloEPWorldX";
    size_t seq_len = strlen(input_sequence);
    
    if (seq_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input sequence too long\n");
        return 1;
    }
    
    for (size_t i = 0; i < seq_len; i++) {
        if (!process_character(&machine, input_sequence[i])) {
            break;
        }
        
        long counter = 0;
        for (long j = 0; j < 1000000; j++) {
            counter += j * j;
            if (counter < 0) counter = 0;
        }
    }
    
    if (machine.current_state != STATE_DONE && machine.current_state != STATE_ERROR) {
        printf("State machine did not reach terminal state\n");
        return 1;
    }
    
    return 0;
}