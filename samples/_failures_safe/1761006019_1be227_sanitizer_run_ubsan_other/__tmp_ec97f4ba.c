//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: state_machine
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
            if (input == '\n') return STATE_IDLE;
            if (is_valid_input(input)) return STATE_READING;
            return STATE_ERROR;
        
        case STATE_READING:
            if (input == '\n') return STATE_PROCESSING;
            if (is_valid_input(input)) return STATE_READING;
            return STATE_ERROR;
        
        case STATE_PROCESSING:
            return STATE_COMPLETE;
        
        case STATE_COMPLETE:
            if (input == '\n') return STATE_IDLE;
            if (is_valid_input(input)) return STATE_READING;
            return STATE_ERROR;
        
        case STATE_ERROR:
            if (input == '\n') return STATE_IDLE;
            return STATE_ERROR;
        
        default:
            return STATE_ERROR;
    }
}

void process_input(StateMachine *machine, char input) {
    if (!is_valid_input(input) && input != '\n') {
        machine->current_state = STATE_ERROR;
        machine->error_count++;
        return;
    }
    
    State next_state = get_next_state(machine->current_state, input);
    
    if (next_state == STATE_READING) {
        if (machine->input_len < MAX_INPUT_LEN) {
            machine->input_buffer[machine->input_len++] = input;
        } else {
            next_state = STATE_ERROR;
        }
    }
    
    if (next_state == STATE_PROCESSING) {
        if (machine->input_len > 0) {
            int sum = 0;
            for (size_t i = 0; i < machine->input_len; i++) {
                sum += (int)machine->input_buffer[i];
            }
            machine->processed_count++;
            machine->input_len = 0;
            memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
        }
    }
    
    if (next_state == STATE_ERROR) {
        machine->error_count++;
        machine->input_len = 0;
        memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
    }
    
    machine->current_state = next_state;
}

void simulate_cpu_work(int iterations) {
    volatile int result = 0;
    for (int i = 0; i < iterations; i++) {
        result += i * i;
        result ^= i;
        result = (result << 3) | (result >> 29);
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("Enter text (alphanumeric characters and spaces):\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to exit\n");
    
    int c;
    while ((c = getchar()) != EOF) {
        if (c == '\0') continue;
        
        process_input(&machine, (char)c);
        
        if (machine.current_state == STATE_PROCESSING) {
            simulate_cpu_work(10000);
        }
        
        if (machine.current_state == STATE_COMPLETE) {
            printf("Processed input %d\n", machine.processed_count);
            machine.current_state = STATE_IDLE;
        }
        
        if (machine.current_state == STATE_ERROR) {
            printf("Error: Invalid input character\n");
            machine.current_state = STATE_IDLE;
        }
    }
    
    printf("\nFinal statistics:\n");
    printf("Processed inputs: %d\n", machine.processed_count);
    printf("Errors: %d\n", machine.error_count);
    
    return 0;
}