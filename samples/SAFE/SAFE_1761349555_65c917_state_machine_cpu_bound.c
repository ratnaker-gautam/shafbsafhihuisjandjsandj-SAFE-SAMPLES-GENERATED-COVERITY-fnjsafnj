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
    size_t position;
    int result;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, ' ', STATE_IDLE},
    {STATE_IDLE, '\n', STATE_IDLE},
    {STATE_IDLE, '\t', STATE_IDLE},
    {STATE_IDLE, '0', STATE_READING},
    {STATE_IDLE, '1', STATE_READING},
    {STATE_READING, '0', STATE_READING},
    {STATE_READING, '1', STATE_READING},
    {STATE_READING, ' ', STATE_PROCESSING},
    {STATE_READING, '\n', STATE_PROCESSING},
    {STATE_READING, '\t', STATE_PROCESSING},
    {STATE_READING, '\0', STATE_PROCESSING},
    {STATE_PROCESSING, ' ', STATE_PROCESSING},
    {STATE_PROCESSING, '\n', STATE_PROCESSING},
    {STATE_PROCESSING, '\t', STATE_PROCESSING},
    {STATE_PROCESSING, '\0', STATE_OUTPUT},
    {STATE_PROCESSING, '0', STATE_ERROR},
    {STATE_PROCESSING, '1', STATE_ERROR},
    {STATE_OUTPUT, ' ', STATE_IDLE},
    {STATE_OUTPUT, '\n', STATE_IDLE},
    {STATE_OUTPUT, '\t', STATE_IDLE},
    {STATE_OUTPUT, '0', STATE_READING},
    {STATE_OUTPUT, '1', STATE_READING},
    {STATE_OUTPUT, '\0', STATE_DONE},
    {STATE_ERROR, '\0', STATE_DONE}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void initialize_machine(StateMachine *machine) {
    machine->current_state = STATE_IDLE;
    machine->input_len = 0;
    machine->position = 0;
    machine->result = 0;
    memset(machine->input_buffer, 0, MAX_INPUT_LEN);
}

State get_next_state(State current, char input) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current == current && transitions[i].input_char == input) {
            return transitions[i].next;
        }
    }
    return STATE_ERROR;
}

int process_binary_number(const char *str, size_t len) {
    int value = 0;
    for (size_t i = 0; i < len; i++) {
        value = (value << 1) | (str[i] - '0');
    }
    return value;
}

void run_state_machine(StateMachine *machine) {
    while (machine->current_state != STATE_DONE && machine->current_state != STATE_ERROR) {
        char current_char = (machine->position < machine->input_len) ? 
                           machine->input_buffer[machine->position] : '\0';
        
        State next_state = get_next_state(machine->current_state, current_char);
        
        switch (machine->current_state) {
            case STATE_READING:
                if (next_state == STATE_PROCESSING) {
                    size_t num_len = machine->position;
                    while (num_len > 0 && isspace(machine->input_buffer[num_len - 1])) {
                        num_len--;
                    }
                    if (num_len > 0) {
                        int num = process_binary_number(machine->input_buffer, num_len);
                        machine->result += num;
                    }
                }
                break;
                
            case STATE_OUTPUT:
                printf("%d\n", machine->result);
                machine->result = 0;
                break;
                
            case STATE_ERROR:
                fprintf(stderr, "Error: Invalid input format\n");
                return;
                
            default:
                break;
        }
        
        machine->current_state = next_state;
        machine->position++;
        
        if (machine->position > MAX_INPUT_LEN) {
            machine->current_state = STATE_ERROR;
            fprintf(stderr, "Error: Input too long\n");
            return;
        }
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("Enter binary numbers separated by whitespace: ");
    
    if (fgets(machine.input_buffer, MAX_INPUT_LEN, stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    machine.input_len = strlen(machine.input_buffer);
    if (machine.input_len > 0 && machine.input_buffer[machine.input_len - 1] == '\n') {
        machine.input_buffer[machine.input_len - 1] = '\0';
        machine.input_len--;
    }
    
    run_state_machine(&machine);
    
    return (machine.current_state == STATE_ERROR) ? 1 : 0;
}