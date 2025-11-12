//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_FINISHED,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_START;
    machine->buffer_len = 0;
    machine->data_count = 0;
    memset(machine->buffer, 0, sizeof(machine->buffer));
}

int is_valid_input(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == ' ' || c == '\n';
}

State handle_event(StateMachine *machine, Event event, char input_char) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (machine->current_state) {
        case STATE_START:
            switch (event) {
                case EVENT_START:
                    return STATE_READING;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_READING:
            switch (event) {
                case EVENT_DATA:
                    if (machine->buffer_len < MAX_INPUT_LEN - 1 && is_valid_input(input_char)) {
                        machine->buffer[machine->buffer_len++] = input_char;
                        return STATE_READING;
                    } else if (input_char == '\n') {
                        machine->buffer[machine->buffer_len] = '\0';
                        return STATE_PROCESSING;
                    } else {
                        return STATE_ERROR;
                    }
                case EVENT_ERROR:
                    return STATE_ERROR;
                default:
                    return machine->current_state;
            }
            
        case STATE_PROCESSING:
            switch (event) {
                case EVENT_COMPLETE:
                    machine->data_count++;
                    return STATE_FINISHED;
                case EVENT_ERROR:
                    return STATE_ERROR;
                default:
                    return machine->current_state;
            }
            
        case STATE_FINISHED:
            switch (event) {
                case EVENT_RESET:
                    initialize_machine(machine);
                    return STATE_START;
                default:
                    return machine->current_state;
            }
            
        case STATE_ERROR:
            switch (event) {
                case EVENT_RESET:
                    initialize_machine(machine);
                    return STATE_START;
                default:
                    return STATE_ERROR;
            }
            
        default:
            return STATE_ERROR;
    }
}

void process_data(StateMachine *machine) {
    if (machine == NULL || machine->current_state != STATE_PROCESSING) return;
    
    if (machine->buffer_len > 0) {
        printf("Processing: %s\n", machine->buffer);
        printf("Characters processed: %zu\n", machine->buffer_len);
    }
    
    machine->current_state = handle_event(machine, EVENT_COMPLETE, 0);
}

void print_state_info(StateMachine *machine) {
    if (machine == NULL) return;
    
    const char *state_names[] = {
        "START", "READING", "PROCESSING", "FINISHED", "ERROR"
    };
    
    if (machine->current_state >= STATE_START && machine->current_state <= STATE_ERROR) {
        printf("Current state: %s | Data count: %d\n", 
               state_names[machine->current_state], machine->data_count);
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("State Machine Demo - Type text and press Enter to process\n");
    printf("Type 'quit' to exit, 'reset' to reset state machine\n");
    
    char input[MAX_INPUT_LEN];
    int running = 1;
    
    while (running) {
        print_state_info(&machine);
        
        if (machine.current_state == STATE_START) {
            machine.current_state = handle_event(&machine, EVENT_START, 0);
            continue;
        }
        
        if (machine.current_state == STATE_READING) {
            printf("Enter text: ");
            
            if (fgets(input, sizeof(input), stdin) != NULL) {
                if (strncmp(input, "quit", 4) == 0) {
                    running = 0;
                    continue;
                } else if (strncmp(input, "reset", 5) == 0) {
                    machine.current_state = handle_event(&machine, EVENT_RESET, 0);
                    continue;
                }
                
                size_t input_len = strlen(input);
                for (size_t i = 0; i < input_len; i++) {
                    machine.current_state = handle_event(&machine, EVENT_DATA, input[i]);
                    if (machine.current_state == STATE_ERROR) {
                        printf("Invalid input detected!\n");
                        break;
                    }
                }
            } else {
                machine.current_state = handle_event(&machine, EVENT_ERROR, 0);
            }
        }
        
        if (machine.current_state == STATE_PROCESSING) {
            process_data(&machine);
        }
        
        if (machine.current_state == STATE_FINISHED) {
            printf("Processing complete. Type 'reset' to continue or 'quit