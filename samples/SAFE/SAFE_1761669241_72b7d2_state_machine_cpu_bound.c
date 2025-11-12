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

void process_character(StateMachine *machine, char c) {
    switch (machine->current_state) {
        case STATE_IDLE:
            if (isalpha(c)) {
                machine->input_buffer[machine->input_len++] = c;
                machine->current_state = STATE_READING;
            }
            break;
        case STATE_READING:
            if (isalpha(c) && machine->input_len < MAX_INPUT_LEN - 1) {
                machine->input_buffer[machine->input_len++] = c;
            } else if (c == '\n') {
                machine->current_state = STATE_PROCESSING;
            } else {
                machine->current_state = STATE_ERROR;
            }
            break;
        case STATE_PROCESSING:
            for (size_t i = 0; i < machine->input_len; i++) {
                if (machine->output_len < MAX_INPUT_LEN - 1) {
                    machine->output_buffer[machine->output_len++] = toupper(machine->input_buffer[i]);
                }
            }
            machine->processed_count++;
            machine->current_state = STATE_OUTPUT;
            break;
        case STATE_OUTPUT:
            printf("Processed: %s\n", machine->output_buffer);
            machine->current_state = STATE_DONE;
            break;
        case STATE_ERROR:
            printf("Error: Invalid input\n");
            machine->current_state = STATE_DONE;
            break;
        case STATE_DONE:
            break;
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    Transition transitions[MAX_TRANSITIONS];
    int transition_count = 0;
    
    add_transition(transitions, &transition_count, STATE_IDLE, 'a', STATE_READING);
    add_transition(transitions, &transition_count, STATE_IDLE, 'z', STATE_READING);
    add_transition(transitions, &transition_count, STATE_READING, 'a', STATE_READING);
    add_transition(transitions, &transition_count, STATE_READING, 'z', STATE_READING);
    add_transition(transitions, &transition_count, STATE_READING, '\n', STATE_PROCESSING);
    add_transition(transitions, &transition_count, STATE_PROCESSING, ' ', STATE_OUTPUT);
    add_transition(transitions, &transition_count, STATE_OUTPUT, ' ', STATE_DONE);
    
    printf("Enter alphabetic strings (max %d chars). Press enter to process, Ctrl+D to exit:\n", MAX_INPUT_LEN - 1);
    
    int c;
    while ((c = getchar()) != EOF) {
        if (c == '\n' && machine.current_state == STATE_DONE) {
            initialize_machine(&machine);
            continue;
        }
        
        if (machine.current_state == STATE_DONE) {
            continue;
        }
        
        process_character(&machine, (char)c);
        
        if (machine.current_state == STATE_DONE) {
            printf("Total processed: %d\n", machine.processed_count);
            printf("Enter next string or Ctrl+D to exit:\n");
        }
    }
    
    if (machine.current_state != STATE_IDLE && machine.current_state != STATE_DONE) {
        process_character(&machine, '\n');
    }
    
    printf("Final count: %d strings processed\n", machine.processed_count);
    return 0;
}