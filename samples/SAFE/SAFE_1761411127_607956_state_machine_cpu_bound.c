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
    int processed_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    machine->current_state = STATE_IDLE;
    machine->input_len = 0;
    machine->output_len = 0;
    machine->processed_count = 0;
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
    switch (machine->current_state) {
        case STATE_IDLE:
            if (isalpha(c)) {
                machine->input_buffer[0] = c;
                machine->input_len = 1;
                machine->current_state = STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (isalpha(c)) {
                if (machine->input_len < MAX_INPUT_LEN - 1) {
                    machine->input_buffer[machine->input_len] = c;
                    machine->input_len++;
                } else {
                    machine->current_state = STATE_ERROR;
                }
            } else if (c == '\n' || c == ';') {
                machine->current_state = STATE_PROCESSING;
            } else {
                machine->current_state = STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            for (size_t i = 0; i < machine->input_len; i++) {
                if (machine->output_len < MAX_INPUT_LEN - 1) {
                    machine->output_buffer[machine->output_len] = toupper(machine->input_buffer[i]);
                    machine->output_len++;
                }
            }
            machine->processed_count++;
            machine->current_state = STATE_OUTPUT;
            break;
            
        case STATE_OUTPUT:
            if (machine->output_len > 0) {
                printf("Processed: %s\n", machine->output_buffer);
                machine->output_len = 0;
                memset(machine->output_buffer, 0, MAX_INPUT_LEN);
            }
            machine->current_state = STATE_IDLE;
            memset(machine->input_buffer, 0, MAX_INPUT_LEN);
            machine->input_len = 0;
            break;
            
        case STATE_ERROR:
            printf("Error: Invalid input sequence\n");
            machine->current_state = STATE_IDLE;
            memset(machine->input_buffer, 0, MAX_INPUT_LEN);
            machine->input_len = 0;
            memset(machine->output_buffer, 0, MAX_INPUT_LEN);
            machine->output_len = 0;
            break;
            
        default:
            machine->current_state = STATE_ERROR;
            break;
    }
    
    return machine->current_state != STATE_ERROR;
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    char input[MAX_INPUT_LEN * 2];
    printf("Enter text (end with empty line):\n");
    
    while (fgets(input, sizeof(input), stdin) != NULL) {
        if (input[0] == '\n') {
            break;
        }
        
        for (size_t i = 0; i < strlen(input) && input[i] != '\0'; i++) {
            if (!process_character(&machine, input[i])) {
                break;
            }
        }
        
        if (machine.current_state == STATE_PROCESSING) {
            process_character(&machine, ';');
        }
    }
    
    printf("Total words processed: %d\n", machine.processed_count);
    return 0;
}