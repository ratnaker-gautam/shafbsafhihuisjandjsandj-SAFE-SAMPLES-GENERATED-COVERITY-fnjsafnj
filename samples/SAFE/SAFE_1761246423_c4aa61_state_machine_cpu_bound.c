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
    char input_buffer[MAX_INPUT_LEN + 1];
    size_t input_pos;
    int processed_count;
    int error_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    machine->current_state = STATE_IDLE;
    machine->input_pos = 0;
    machine->processed_count = 0;
    machine->error_count = 0;
    memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
}

int is_valid_input(char c) {
    return isalnum(c) || c == ' ' || c == '\n';
}

State get_next_state(StateMachine *machine, char input_char) {
    static const Transition transitions[] = {
        {STATE_IDLE, ' ', STATE_IDLE},
        {STATE_IDLE, '\n', STATE_IDLE},
        {STATE_IDLE, 0, STATE_READING},
        {STATE_READING, ' ', STATE_PROCESSING},
        {STATE_READING, '\n', STATE_PROCESSING},
        {STATE_READING, 0, STATE_READING},
        {STATE_PROCESSING, 0, STATE_OUTPUT},
        {STATE_OUTPUT, 0, STATE_IDLE},
        {STATE_ERROR, 0, STATE_IDLE}
    };
    
    for (size_t i = 0; i < sizeof(transitions)/sizeof(transitions[0]); i++) {
        if (transitions[i].current == machine->current_state) {
            if (transitions[i].input_char == 0 || transitions[i].input_char == input_char) {
                return transitions[i].next;
            }
        }
    }
    
    return STATE_ERROR;
}

void process_state(StateMachine *machine, char input_char) {
    switch (machine->current_state) {
        case STATE_IDLE:
            if (input_char != ' ' && input_char != '\n') {
                if (machine->input_pos < MAX_INPUT_LEN) {
                    machine->input_buffer[machine->input_pos++] = input_char;
                }
            }
            break;
            
        case STATE_READING:
            if (input_char == ' ' || input_char == '\n') {
                break;
            }
            if (machine->input_pos < MAX_INPUT_LEN) {
                machine->input_buffer[machine->input_pos++] = input_char;
            } else {
                machine->error_count++;
            }
            break;
            
        case STATE_PROCESSING:
            if (machine->input_pos > 0) {
                for (size_t i = 0; i < machine->input_pos; i++) {
                    if (isalpha(machine->input_buffer[i])) {
                        machine->input_buffer[i] = toupper(machine->input_buffer[i]);
                    }
                }
                machine->processed_count++;
            }
            break;
            
        case STATE_OUTPUT:
            if (machine->input_pos > 0) {
                printf("Processed: %s\n", machine->input_buffer);
                machine->input_pos = 0;
                memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
            }
            break;
            
        case STATE_ERROR:
            machine->error_count++;
            machine->input_pos = 0;
            memset(machine->input_buffer, 0, sizeof(machine->input_buffer));
            break;
            
        case STATE_DONE:
            break;
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("Enter text (Ctrl+D to end):\n");
    
    int c;
    while ((c = getchar()) != EOF) {
        if (c == 4) break;
        
        if (!is_valid_input(c)) {
            machine.current_state = STATE_ERROR;
        } else {
            machine.current_state = get_next_state(&machine, (char)c);
        }
        
        process_state(&machine, (char)c);
        
        if (machine.current_state == STATE_ERROR) {
            fprintf(stderr, "Invalid input detected\n");
            machine.current_state = STATE_IDLE;
        }
        
        if (machine.processed_count >= 1000) {
            machine.current_state = STATE_DONE;
            break;
        }
    }
    
    if (machine.input_pos > 0 && machine.current_state != STATE_ERROR) {
        machine.current_state = STATE_PROCESSING;
        process_state(&machine, '\0');
        machine.current_state = STATE_OUTPUT;
        process_state(&machine, '\0');
    }
    
    printf("Processing complete. Words processed: %d, Errors: %d\n",
           machine.processed_count, machine.error_count);
    
    return 0;
}