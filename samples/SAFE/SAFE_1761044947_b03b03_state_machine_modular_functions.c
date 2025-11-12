//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    size_t buffer_pos;
    int data_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_START;
    machine->buffer_pos = 0;
    machine->data_count = 0;
    memset(machine->buffer, 0, sizeof(machine->buffer));
}

int is_valid_input_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
           (c >= '0' && c <= '9') || c == ' ' || c == '\n';
}

State handle_event(StateMachine *machine, Event event, char data) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (machine->current_state) {
        case STATE_START:
            if (event == EVENT_START) {
                machine->data_count = 0;
                return STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (is_valid_input_char(data) && machine->buffer_pos < MAX_INPUT_LEN - 1) {
                    machine->buffer[machine->buffer_pos++] = data;
                    return STATE_READING;
                } else if (data == '\n') {
                    machine->buffer[machine->buffer_pos] = '\0';
                    return STATE_PROCESSING;
                } else {
                    return STATE_ERROR;
                }
            } else if (event == EVENT_ERROR) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_COMPLETE) {
                machine->data_count++;
                machine->buffer_pos = 0;
                memset(machine->buffer, 0, sizeof(machine->buffer));
                return STATE_FINISHED;
            } else if (event == EVENT_ERROR) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_FINISHED:
            if (event == EVENT_RESET) {
                return STATE_START;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                return STATE_START;
            }
            break;
            
        default:
            return STATE_ERROR;
    }
    
    return machine->current_state;
}

void process_data(StateMachine *machine) {
    if (machine == NULL || machine->buffer_pos == 0) return;
    
    printf("Processing: '");
    for (size_t i = 0; i < machine->buffer_pos && i < MAX_INPUT_LEN; i++) {
        if (machine->buffer[i] != '\0') {
            printf("%c", machine->buffer[i]);
        }
    }
    printf("'\n");
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("State Machine Demo - Enter text lines (empty line to finish):\n");
    
    int running = 1;
    while (running) {
        char input[MAX_INPUT_LEN];
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            machine.current_state = handle_event(&machine, EVENT_ERROR, 0);
            continue;
        }
        
        if (strlen(input) == 1 && input[0] == '\n') {
            if (machine.current_state == STATE_READING && machine.buffer_pos > 0) {
                machine.current_state = handle_event(&machine, EVENT_DATA, '\n');
                if (machine.current_state == STATE_PROCESSING) {
                    process_data(&machine);
                    machine.current_state = handle_event(&machine, EVENT_COMPLETE, 0);
                }
            }
            running = 0;
            continue;
        }
        
        if (machine.current_state == STATE_START) {
            machine.current_state = handle_event(&machine, EVENT_START, 0);
        }
        
        if (machine.current_state == STATE_READING) {
            for (size_t i = 0; i < strlen(input) && i < MAX_INPUT_LEN; i++) {
                machine.current_state = handle_event(&machine, EVENT_DATA, input[i]);
                if (machine.current_state == STATE_ERROR) {
                    printf("Error: Invalid input character\n");
                    machine.current_state = handle_event(&machine, EVENT_RESET, 0);
                    break;
                }
            }
            
            if (machine.current_state == STATE_PROCESSING) {
                process_data(&machine);
                machine.current_state = handle_event(&machine, EVENT_COMPLETE, 0);
            }
        }
        
        if (machine.current_state == STATE_ERROR) {
            printf("State machine error occurred. Resetting...\n");
            machine.current_state = handle_event(&machine, EVENT_RESET, 0);
        }
    }
    
    printf("Processed %d data items. Exiting.\n", machine.data_count);
    return 0;
}