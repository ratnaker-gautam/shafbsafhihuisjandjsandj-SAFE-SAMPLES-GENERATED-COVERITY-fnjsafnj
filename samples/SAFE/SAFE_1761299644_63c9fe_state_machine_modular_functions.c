//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 50

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_DONE,
    STATE_ERROR
} State;

typedef struct {
    State current_state;
    char input_char;
    State next_state;
} Transition;

typedef struct {
    State current_state;
    char input_buffer[MAX_INPUT_LEN + 1];
    size_t input_len;
    int processed_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_IDLE;
    machine->input_len = 0;
    machine->processed_count = 0;
    memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
}

int add_transition(Transition transitions[], size_t *count, State current, char input, State next) {
    if (transitions == NULL || count == NULL || *count >= MAX_TRANSITIONS) return 0;
    transitions[*count].current_state = current;
    transitions[*count].input_char = input;
    transitions[*count].next_state = next;
    (*count)++;
    return 1;
}

State get_next_state(const Transition transitions[], size_t count, State current, char input) {
    if (transitions == NULL) return STATE_ERROR;
    for (size_t i = 0; i < count; i++) {
        if (transitions[i].current_state == current && transitions[i].input_char == input) {
            return transitions[i].next_state;
        }
    }
    return STATE_ERROR;
}

int process_input(StateMachine *machine, const char *input, const Transition transitions[], size_t transition_count) {
    if (machine == NULL || input == NULL || transitions == NULL) return 0;
    
    size_t input_length = strlen(input);
    if (input_length == 0 || input_length > MAX_INPUT_LEN) return 0;
    
    initialize_machine(machine);
    
    for (size_t i = 0; i < input_length; i++) {
        char current_char = input[i];
        
        if (!isprint(current_char)) {
            machine->current_state = STATE_ERROR;
            break;
        }
        
        State next_state = get_next_state(transitions, transition_count, machine->current_state, current_char);
        
        if (next_state == STATE_ERROR) {
            machine->current_state = STATE_ERROR;
            break;
        }
        
        machine->current_state = next_state;
        
        if (machine->current_state == STATE_READING) {
            if (machine->input_len < MAX_INPUT_LEN) {
                machine->input_buffer[machine->input_len++] = current_char;
            } else {
                machine->current_state = STATE_ERROR;
                break;
            }
        } else if (machine->current_state == STATE_PROCESSING) {
            machine->processed_count++;
        }
    }
    
    if (machine->current_state != STATE_ERROR && machine->current_state != STATE_DONE) {
        machine->current_state = STATE_ERROR;
    }
    
    return machine->current_state != STATE_ERROR;
}

void print_result(const StateMachine *machine) {
    if (machine == NULL) return;
    
    switch (machine->current_state) {
        case STATE_DONE:
            printf("Processing complete. Input: '%s', Processed items: %d\n", 
                   machine->input_buffer, machine->processed_count);
            break;
        case STATE_ERROR:
            printf("Error: Invalid input sequence\n");
            break;
        default:
            printf("Unexpected state: %d\n", machine->current_state);
            break;
    }
}

int main(void) {
    Transition transitions[MAX_TRANSITIONS];
    size_t transition_count = 0;
    
    add_transition(transitions, &transition_count, STATE_IDLE, 'S', STATE_READING);
    add_transition(transitions, &transition_count, STATE_READING, 'P', STATE_PROCESSING);
    add_transition(transitions, &transition_count, STATE_PROCESSING, 'C', STATE_READING);
    add_transition(transitions, &transition_count, STATE_READING, 'E', STATE_DONE);
    add_transition(transitions, &transition_count, STATE_PROCESSING, 'E', STATE_DONE);
    
    char user_input[MAX_INPUT_LEN + 1];
    printf("Enter sequence (S=start, P=process, C=continue, E=end): ");
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(user_input);
    if (len > 0 && user_input[len - 1] == '\n') {
        user_input[len - 1] = '\0';
    }
    
    if (strlen(user_input) == 0) {
        printf("Error: Empty input\n");
        return 1;
    }
    
    StateMachine machine;
    int success = process_input(&machine, user_input, transitions, transition_count);
    
    if (!success) {
        printf("Error: Failed to process input\n");
        return 1;
    }
    
    print_result(&machine);
    
    return 0;
}