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
    size_t buffer_pos;
    int data_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_START;
    memset(machine->buffer, 0, sizeof(machine->buffer));
    machine->buffer_pos = 0;
    machine->data_count = 0;
}

int is_valid_input_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
           (c >= '0' && c <= '9') || c == ' ' || c == '\n';
}

Event get_next_event(const char *input, size_t *pos) {
    if (input == NULL || pos == NULL) return EVENT_ERROR;
    
    if (*pos >= strlen(input)) return EVENT_COMPLETE;
    
    char c = input[*pos];
    if (!is_valid_input_char(c)) return EVENT_ERROR;
    
    if (c == '\n') {
        (*pos)++;
        return EVENT_COMPLETE;
    }
    
    (*pos)++;
    return EVENT_DATA;
}

State handle_event(StateMachine *machine, Event event, char data) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (machine->current_state) {
        case STATE_START:
            switch (event) {
                case EVENT_START:
                    return STATE_READING;
                case EVENT_DATA:
                    if (machine->buffer_pos < MAX_INPUT_LEN - 1) {
                        machine->buffer[machine->buffer_pos++] = data;
                    }
                    return STATE_READING;
                default:
                    return STATE_ERROR;
            }
            
        case STATE_READING:
            switch (event) {
                case EVENT_DATA:
                    if (machine->buffer_pos < MAX_INPUT_LEN - 1) {
                        machine->buffer[machine->buffer_pos++] = data;
                    }
                    return STATE_READING;
                case EVENT_COMPLETE:
                    machine->buffer[machine->buffer_pos] = '\0';
                    machine->data_count++;
                    return STATE_PROCESSING;
                case EVENT_ERROR:
                    return STATE_ERROR;
                default:
                    return STATE_READING;
            }
            
        case STATE_PROCESSING:
            switch (event) {
                case EVENT_COMPLETE:
                    return STATE_FINISHED;
                case EVENT_RESET:
                    initialize_machine(machine);
                    return STATE_START;
                default:
                    return STATE_PROCESSING;
            }
            
        case STATE_FINISHED:
            switch (event) {
                case EVENT_RESET:
                    initialize_machine(machine);
                    return STATE_START;
                default:
                    return STATE_FINISHED;
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
    
    printf("Processing data %d: '", machine->data_count);
    for (size_t i = 0; i < machine->buffer_pos; i++) {
        if (machine->buffer[i] != '\0') {
            putchar(machine->buffer[i]);
        }
    }
    printf("'\n");
}

void run_state_machine(const char *input) {
    if (input == NULL) return;
    
    StateMachine machine;
    initialize_machine(&machine);
    
    size_t input_pos = 0;
    Event event = EVENT_START;
    
    while (machine.current_state != STATE_FINISHED && 
           machine.current_state != STATE_ERROR) {
        
        State old_state = machine.current_state;
        machine.current_state = handle_event(&machine, event, input[input_pos]);
        
        if (machine.current_state == STATE_PROCESSING && old_state != STATE_PROCESSING) {
            process_data(&machine);
            event = EVENT_COMPLETE;
        } else if (machine.current_state == STATE_READING) {
            event = get_next_event(input, &input_pos);
        } else if (machine.current_state == STATE_ERROR) {
            printf("State machine entered error state\n");
            event = EVENT_RESET;
        } else if (machine.current_state == STATE_FINISHED) {
            printf("State machine completed successfully\n");
            break;
        }
        
        if (input_pos >= strlen(input) && machine.current_state == STATE_READING) {
            event = EVENT_COMPLETE;
        }
    }
}

int main(void) {
    const char *test_input = "Hello World\nTest Data\nAnother Line\n";
    printf("Running state machine test...\n");
    run_state_machine(test_input);
    return 0;
}