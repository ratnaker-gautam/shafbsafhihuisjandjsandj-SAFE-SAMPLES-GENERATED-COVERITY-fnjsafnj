//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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
    size_t position;
    int result;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    machine->current_state = STATE_IDLE;
    machine->input_len = 0;
    machine->position = 0;
    machine->result = 0;
    memset(machine->input_buffer, 0, MAX_INPUT_LEN);
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

State get_next_state(const Transition transitions[], int count, State current, char input) {
    for (int i = 0; i < count; i++) {
        if (transitions[i].current == current && transitions[i].input_char == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

int is_valid_input(char c) {
    return isdigit(c) || c == ' ' || c == '\n' || c == '\0';
}

void process_state(StateMachine *machine, const Transition transitions[], int transition_count) {
    char current_char = machine->position < machine->input_len ? 
                       machine->input_buffer[machine->position] : '\0';
    
    if (!is_valid_input(current_char)) {
        machine->current_state = STATE_ERROR;
        return;
    }

    State next_state = get_next_state(transitions, transition_count, 
                                     machine->current_state, current_char);
    
    switch (machine->current_state) {
        case STATE_IDLE:
            if (isdigit(current_char)) {
                machine->result = current_char - '0';
                machine->position++;
            }
            break;
            
        case STATE_READING:
            if (isdigit(current_char)) {
                int digit = current_char - '0';
                if (machine->result > (INT_MAX - digit) / 10) {
                    machine->current_state = STATE_ERROR;
                    return;
                }
                machine->result = machine->result * 10 + digit;
                machine->position++;
            }
            break;
            
        case STATE_PROCESSING:
            machine->position++;
            break;
            
        case STATE_OUTPUT:
            break;
            
        default:
            break;
    }
    
    machine->current_state = next_state;
}

int main(void) {
    StateMachine machine;
    Transition transitions[MAX_TRANSITIONS];
    int transition_count = 0;
    
    initialize_machine(&machine);
    
    add_transition(transitions, &transition_count, STATE_IDLE, ' ', STATE_IDLE);
    add_transition(transitions, &transition_count, STATE_IDLE, '\n', STATE_IDLE);
    add_transition(transitions, &transition_count, STATE_IDLE, '\0', STATE_DONE);
    for (char c = '0'; c <= '9'; c++) {
        add_transition(transitions, &transition_count, STATE_IDLE, c, STATE_READING);
    }
    
    add_transition(transitions, &transition_count, STATE_READING, ' ', STATE_PROCESSING);
    add_transition(transitions, &transition_count, STATE_READING, '\n', STATE_OUTPUT);
    add_transition(transitions, &transition_count, STATE_READING, '\0', STATE_OUTPUT);
    for (char c = '0'; c <= '9'; c++) {
        add_transition(transitions, &transition_count, STATE_READING, c, STATE_READING);
    }
    
    add_transition(transitions, &transition_count, STATE_PROCESSING, ' ', STATE_PROCESSING);
    add_transition(transitions, &transition_count, STATE_PROCESSING, '\n', STATE_OUTPUT);
    add_transition(transitions, &transition_count, STATE_PROCESSING, '\0', STATE_OUTPUT);
    for (char c = '0'; c <= '9'; c++) {
        add_transition(transitions, &transition_count, STATE_PROCESSING, c, STATE_READING);
    }
    
    add_transition(transitions, &transition_count, STATE_OUTPUT, ' ', STATE_IDLE);
    add_transition(transitions, &transition_count, STATE_OUTPUT, '\n', STATE_IDLE);
    add_transition(transitions, &transition_count, STATE_OUTPUT, '\0', STATE_DONE);
    for (char c = '0'; c <= '9'; c++) {
        add_transition(transitions, &transition_count, STATE_OUTPUT, c, STATE_READING);
    }
    
    printf("Enter numbers separated by spaces: ");
    if (fgets(machine.input_buffer, MAX_INPUT_LEN, stdin) == NULL) {
        return 1;