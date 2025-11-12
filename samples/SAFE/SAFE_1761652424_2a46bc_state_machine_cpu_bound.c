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
    memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
}

State handle_transition(StateMachine *machine, char input, const Transition *transitions, size_t num_transitions) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == machine->current_state && transitions[i].trigger == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

void process_input(StateMachine *machine, const char *input_str) {
    const Transition transitions[] = {
        {STATE_IDLE, 'S', STATE_READING},
        {STATE_READING, 'P', STATE_PROCESSING},
        {STATE_READING, 'E', STATE_COMPLETE},
        {STATE_PROCESSING, 'C', STATE_COMPLETE},
        {STATE_PROCESSING, 'R', STATE_READING}
    };
    const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);
    
    size_t input_length = strlen(input_str);
    if (input_length == 0 || input_length > MAX_INPUT_LEN) {
        machine->error_count++;
        return;
    }
    
    for (size_t i = 0; i < input_length; i++) {
        char current_char = input_str[i];
        
        if (!isalpha(current_char)) {
            machine->error_count++;
            machine->current_state = STATE_ERROR;
            continue;
        }
        
        current_char = toupper(current_char);
        
        State new_state = handle_transition(machine, current_char, transitions, num_transitions);
        
        if (new_state == STATE_ERROR) {
            machine->error_count++;
            machine->current_state = STATE_ERROR;
        } else {
            machine->current_state = new_state;
            
            if (machine->current_state == STATE_PROCESSING) {
                for (size_t j = 0; j < 1000; j++) {
                    int dummy = j * j;
                    dummy += j;
                }
                machine->processed_count++;
            }
        }
        
        if (machine->current_state == STATE_COMPLETE) {
            break;
        }
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    char user_input[MAX_INPUT_LEN + 1];
    
    printf("Enter state sequence (S=Start, P=Process, C=Complete, R=Reset, E=End): ");
    
    if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(user_input);
    if (input_len > 0 && user_input[input_len - 1] == '\n') {
        user_input[input_len - 1] = '\0';
    }
    
    if (strlen(user_input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    process_input(&machine, user_input);
    
    printf("Final state: ");
    switch (machine.current_state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READING: printf("READING"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_COMPLETE: printf("COMPLETE"); break;
        case STATE_ERROR: printf("ERROR"); break;
    }
    printf("\n");
    
    printf("Processed operations: %d\n", machine.processed_count);
    printf("Errors encountered: %d\n", machine.error_count);
    
    return 0;
}