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
    size_t input_pos;
    int processed_count;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, 's', STATE_READING},
    {STATE_READING, 'p', STATE_PROCESSING},
    {STATE_READING, 'e', STATE_DONE},
    {STATE_PROCESSING, 'c', STATE_READING},
    {STATE_PROCESSING, 'e', STATE_DONE},
    {STATE_IDLE, 'e', STATE_DONE},
    {STATE_READING, '\0', STATE_ERROR},
    {STATE_PROCESSING, '\0', STATE_ERROR}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void initialize_machine(StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_IDLE;
    machine->input_pos = 0;
    machine->processed_count = 0;
    memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
}

State get_next_state(State current, char input) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == current && 
            transitions[i].input_char == input) {
            return transitions[i].next_state;
        }
    }
    return STATE_ERROR;
}

int process_input_char(StateMachine *machine, char input) {
    if (machine == NULL) return 0;
    
    if (machine->input_pos >= MAX_INPUT_LEN) {
        machine->current_state = STATE_ERROR;
        return 0;
    }
    
    State next_state = get_next_state(machine->current_state, input);
    
    if (next_state == STATE_ERROR) {
        machine->current_state = STATE_ERROR;
        return 0;
    }
    
    machine->current_state = next_state;
    
    if (machine->current_state == STATE_READING) {
        machine->input_buffer[machine->input_pos++] = input;
    } else if (machine->current_state == STATE_PROCESSING) {
        machine->processed_count++;
    }
    
    return 1;
}

void print_state_info(const StateMachine *machine) {
    if (machine == NULL) return;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            printf("State: IDLE\n");
            break;
        case STATE_READING:
            printf("State: READING (Buffer: %s)\n", machine->input_buffer);
            break;
        case STATE_PROCESSING:
            printf("State: PROCESSING (Count: %d)\n", machine->processed_count);
            break;
        case STATE_DONE:
            printf("State: DONE (Final count: %d)\n", machine->processed_count);
            break;
        case STATE_ERROR:
            printf("State: ERROR\n");
            break;
    }
}

int read_valid_input(char *buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0) return 0;
    
    if (fgets(buffer, (int)buffer_size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 1;
}

int main(void) {
    StateMachine machine;
    char input_line[MAX_INPUT_LEN + 1];
    
    initialize_machine(&machine);
    
    printf("Enter commands (s=start, p=process, c=continue, e=end): ");
    
    if (!read_valid_input(input_line, sizeof(input_line))) {
        printf("Input error\n");
        return 1;
    }
    
    size_t input_len = strlen(input_line);
    for (size_t i = 0; i < input_len; i++) {
        char cmd = input_line[i];
        
        if (!isalpha((unsigned char)cmd)) {
            printf("Invalid character: %c\n", cmd);
            machine.current_state = STATE_ERROR;
            break;
        }
        
        if (!process_input_char(&machine, cmd)) {
            break;
        }
        
        print_state_info(&machine);
        
        if (machine.current_state == STATE_DONE || 
            machine.current_state == STATE_ERROR) {
            break;
        }
    }
    
    if (machine.current_state != STATE_DONE && 
        machine.current_state != STATE_ERROR) {
        printf("Sequence incomplete\n");
    }
    
    return 0;
}