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
    machine->buffer_pos = 0;
    machine->data_count = 0;
    memset(machine->buffer, 0, sizeof(machine->buffer));
}

int is_valid_input(char c) {
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
                if (is_valid_input(data) && machine->buffer_pos < MAX_INPUT_LEN - 1) {
                    machine->buffer[machine->buffer_pos++] = data;
                    return STATE_READING;
                } else {
                    return STATE_ERROR;
                }
            } else if (event == EVENT_COMPLETE) {
                if (machine->buffer_pos > 0) {
                    machine->buffer[machine->buffer_pos] = '\0';
                    return STATE_PROCESSING;
                } else {
                    return STATE_ERROR;
                }
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_COMPLETE) {
                machine->data_count++;
                if (machine->data_count < 3) {
                    machine->buffer_pos = 0;
                    return STATE_READING;
                } else {
                    return STATE_FINISHED;
                }
            }
            break;
            
        case STATE_FINISHED:
            if (event == EVENT_RESET) {
                initialize_machine(machine);
                return STATE_START;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                initialize_machine(machine);
                return STATE_START;
            }
            break;
            
        default:
            break;
    }
    
    return machine->current_state;
}

void process_data(StateMachine *machine) {
    if (machine == NULL || machine->current_state != STATE_PROCESSING) return;
    
    printf("Processing: %s\n", machine->buffer);
    
    int char_count = 0;
    int digit_count = 0;
    
    for (size_t i = 0; i < machine->buffer_pos && i < MAX_INPUT_LEN; i++) {
        char c = machine->buffer[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            char_count++;
        } else if (c >= '0' && c <= '9') {
            digit_count++;
        }
    }
    
    printf("Letters: %d, Digits: %d\n", char_count, digit_count);
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("State Machine Demo - Enter up to 3 strings to process\n");
    printf("Enter 'quit' to exit, 'reset' to start over\n");
    
    char input[MAX_INPUT_LEN];
    
    while (machine.current_state != STATE_FINISHED) {
        if (machine.current_state == STATE_START) {
            machine.current_state = handle_event(&machine, EVENT_START, 0);
        } else if (machine.current_state == STATE_READING) {
            printf("Enter string %d: ", machine.data_count + 1);
            
            if (fgets(input, sizeof(input), stdin) != NULL) {
                size_t len = strlen(input);
                if (len > 0 && input[len - 1] == '\n') {
                    input[len - 1] = '\0';
                    len--;
                }
                
                if (strcmp(input, "quit") == 0) {
                    break;
                } else if (strcmp(input, "reset") == 0) {
                    machine.current_state = handle_event(&machine, EVENT_RESET, 0);
                    continue;
                }
                
                if (len == 0) {
                    machine.current_state = handle_event(&machine, EVENT_ERROR, 0);
                } else {
                    for (size_t i = 0; i < len && i < MAX_INPUT_LEN; i++) {
                        machine.current_state = handle_event(&machine, EVENT_DATA, input[i]);
                        if (machine.current_state == STATE_ERROR) break;
                    }
                    
                    if (machine.current_state != STATE_ERROR) {
                        machine.current_state = handle_event(&machine, EVENT_COMPLETE, 0);
                    }
                }
            } else {
                machine.current_state = handle_event(&machine, EVENT_ERROR