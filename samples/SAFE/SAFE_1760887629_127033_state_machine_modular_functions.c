//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 20

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_DONE,
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
    Transition transitions[MAX_TRANSITIONS];
    size_t num_transitions;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    machine->current_state = STATE_IDLE;
    machine->input_len = 0;
    machine->num_transitions = 0;
    
    Transition trans[] = {
        {STATE_IDLE, 's', STATE_READING},
        {STATE_READING, 'p', STATE_PROCESSING},
        {STATE_READING, 'e', STATE_DONE},
        {STATE_PROCESSING, 'c', STATE_READING},
        {STATE_PROCESSING, 'e', STATE_DONE},
        {STATE_IDLE, 'e', STATE_DONE},
        {STATE_READING, 'x', STATE_ERROR},
        {STATE_PROCESSING, 'x', STATE_ERROR}
    };
    
    size_t num = sizeof(trans) / sizeof(trans[0]);
    if (num > MAX_TRANSITIONS) {
        num = MAX_TRANSITIONS;
    }
    
    for (size_t i = 0; i < num; i++) {
        machine->transitions[i] = trans[i];
    }
    machine->num_transitions = num;
}

State get_next_state(const StateMachine *machine, char input) {
    for (size_t i = 0; i < machine->num_transitions; i++) {
        if (machine->transitions[i].current == machine->current_state &&
            machine->transitions[i].trigger == input) {
            return machine->transitions[i].next;
        }
    }
    return machine->current_state;
}

void process_input(StateMachine *machine, char input) {
    if (!isprint(input)) {
        return;
    }
    
    State next_state = get_next_state(machine, input);
    
    switch (next_state) {
        case STATE_READING:
            if (machine->input_len < MAX_INPUT_LEN - 1) {
                machine->input_buffer[machine->input_len++] = input;
                machine->input_buffer[machine->input_len] = '\0';
            }
            break;
        case STATE_PROCESSING:
            if (machine->input_len > 0) {
                for (size_t i = 0; i < machine->input_len; i++) {
                    if (islower(machine->input_buffer[i])) {
                        machine->input_buffer[i] = toupper(machine->input_buffer[i]);
                    }
                }
            }
            break;
        case STATE_DONE:
            break;
        case STATE_ERROR:
            break;
        default:
            break;
    }
    
    machine->current_state = next_state;
}

void print_state_info(const StateMachine *machine) {
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "DONE", "ERROR"
    };
    
    printf("Current state: %s\n", state_names[machine->current_state]);
    if (machine->input_len > 0) {
        printf("Buffer: %s\n", machine->input_buffer);
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("State Machine Demo\n");
    printf("Commands: s=start reading, p=process, c=continue reading, e=end, x=error\n");
    printf("Enter commands one character at a time:\n");
    
    int input_char;
    while (machine.current_state != STATE_DONE && machine.current_state != STATE_ERROR) {
        print_state_info(&machine);
        printf("Enter command: ");
        
        input_char = getchar();
        if (input_char == EOF) {
            break;
        }
        
        while (getchar() != '\n') {
            if (feof(stdin)) {
                break;
            }
        }
        
        if (input_char == '\n') {
            continue;
        }
        
        process_input(&machine, (char)input_char);
    }
    
    print_state_info(&machine);
    
    if (machine.current_state == STATE_DONE) {
        printf("Final buffer: %s\n", machine.input_buffer);
        printf("Processing completed successfully.\n");
    } else if (machine.current_state == STATE_ERROR) {
        printf("State machine entered error state.\n");
    }
    
    return 0;
}