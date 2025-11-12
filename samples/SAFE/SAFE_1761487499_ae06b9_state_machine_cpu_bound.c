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
    State current_state;
    State next_state;
    char trigger_char;
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
    memset(machine->input_buffer, 0, MAX_INPUT_LEN);
}

State handle_idle(StateMachine *machine, char input) {
    if (input == 'S') {
        machine->input_len = 0;
        memset(machine->input_buffer, 0, MAX_INPUT_LEN);
        return STATE_READING;
    }
    return STATE_IDLE;
}

State handle_reading(StateMachine *machine, char input) {
    if (input == 'E') {
        if (machine->input_len > 0) {
            return STATE_PROCESSING;
        }
        return STATE_ERROR;
    }
    if (machine->input_len < MAX_INPUT_LEN - 1) {
        if (isalnum(input) || input == ' ') {
            machine->input_buffer[machine->input_len++] = input;
            return STATE_READING;
        }
    }
    return STATE_ERROR;
}

State handle_processing(StateMachine *machine, char input) {
    if (input == 'C') {
        int sum = 0;
        for (size_t i = 0; i < machine->input_len; i++) {
            if (isdigit(machine->input_buffer[i])) {
                sum += machine->input_buffer[i] - '0';
            }
        }
        machine->processed_count += sum;
        return STATE_COMPLETE;
    }
    return STATE_ERROR;
}

State handle_complete(StateMachine *machine, char input) {
    if (input == 'R') {
        return STATE_IDLE;
    }
    return STATE_COMPLETE;
}

State handle_error(StateMachine *machine, char input) {
    if (input == 'R') {
        machine->error_count++;
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

void process_input(StateMachine *machine, char input) {
    State next_state = machine->current_state;
    
    switch (machine->current_state) {
        case STATE_IDLE:
            next_state = handle_idle(machine, input);
            break;
        case STATE_READING:
            next_state = handle_reading(machine, input);
            break;
        case STATE_PROCESSING:
            next_state = handle_processing(machine, input);
            break;
        case STATE_COMPLETE:
            next_state = handle_complete(machine, input);
            break;
        case STATE_ERROR:
            next_state = handle_error(machine, input);
            break;
    }
    
    machine->current_state = next_state;
}

void simulate_cpu_work(int iterations) {
    volatile int result = 0;
    for (int i = 0; i < iterations; i++) {
        result += i * i;
        if (result > 1000000) {
            result = result % 1000;
        }
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    char input_sequence[] = "Sabc123E C R Sdef456E C R SxyzE C R Sghi789E C R";
    size_t seq_len = strlen(input_sequence);
    
    printf("Starting state machine simulation...\n");
    
    for (size_t i = 0; i < seq_len; i++) {
        char current_char = input_sequence[i];
        
        if (current_char == ' ') {
            continue;
        }
        
        process_input(&machine, current_char);
        
        simulate_cpu_work(10000);
        
        printf("Char: %c, State: %d, Processed: %d, Errors: %d\n", 
               current_char, machine.current_state, 
               machine.processed_count, machine.error_count);
    }
    
    printf("Final - Processed: %d, Errors: %d\n", 
           machine.processed_count, machine.error_count);
    
    return 0;
}