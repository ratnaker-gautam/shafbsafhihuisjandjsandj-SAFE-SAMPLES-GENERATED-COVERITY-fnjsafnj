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
    char input_char;
    State next;
} Transition;

typedef struct {
    State current_state;
    char input_buffer[MAX_INPUT_LEN + 1];
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

int is_valid_input(char c) {
    return isalnum(c) || c == ' ' || c == '\n';
}

State get_next_state(State current, char input) {
    switch (current) {
        case STATE_IDLE:
            if (input == 'S') return STATE_READING;
            break;
        case STATE_READING:
            if (input == 'E') return STATE_PROCESSING;
            if (is_valid_input(input)) return STATE_READING;
            break;
        case STATE_PROCESSING:
            if (input == 'C') return STATE_COMPLETE;
            break;
        case STATE_COMPLETE:
            return STATE_IDLE;
        case STATE_ERROR:
            return STATE_IDLE;
        default:
            break;
    }
    return STATE_ERROR;
}

void process_state(StateMachine *machine) {
    switch (machine->current_state) {
        case STATE_IDLE:
            break;
        case STATE_READING:
            if (machine->input_len < MAX_INPUT_LEN) {
                machine->input_buffer[machine->input_len] = 'X';
                machine->input_len++;
            }
            break;
        case STATE_PROCESSING:
            for (size_t i = 0; i < machine->input_len; i++) {
                machine->input_buffer[i] = (char)((machine->input_buffer[i] + 1) % 128);
            }
            machine->processed_count++;
            break;
        case STATE_COMPLETE:
            machine->input_len = 0;
            memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
            break;
        case STATE_ERROR:
            machine->error_count++;
            machine->input_len = 0;
            memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
            break;
        default:
            break;
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    char input_sequence[] = "SabcEdefCSE123ECinvalidSxyzC";
    size_t seq_len = strlen(input_sequence);
    
    if (seq_len > 1000) {
        fprintf(stderr, "Input sequence too long\n");
        return 1;
    }
    
    for (size_t i = 0; i < seq_len; i++) {
        char current_char = input_sequence[i];
        
        State next_state = get_next_state(machine.current_state, current_char);
        
        if (next_state == STATE_ERROR && machine.current_state != STATE_ERROR) {
            machine.current_state = STATE_ERROR;
        } else {
            machine.current_state = next_state;
        }
        
        process_state(&machine);
        
        if (machine.processed_count > 1000 || machine.error_count > 100) {
            fprintf(stderr, "Safety limit reached\n");
            break;
        }
    }
    
    printf("Processing complete: %d cycles processed, %d errors\n", 
           machine.processed_count, machine.error_count);
    
    return 0;
}