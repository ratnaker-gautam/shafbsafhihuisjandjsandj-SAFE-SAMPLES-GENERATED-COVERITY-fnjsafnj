//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LENGTH 100
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
    EVENT_PROCESS,
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LENGTH];
    size_t buffer_index;
    int data_count;
} StateMachine;

void initialize_machine(StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_START;
    machine->buffer_index = 0;
    machine->data_count = 0;
    memset(machine->buffer, 0, sizeof(machine->buffer));
}

int is_valid_input(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == ' ' || c == '\n';
}

State handle_event(StateMachine *machine, Event event, char data) {
    if (machine == NULL) return STATE_ERROR;

    switch (machine->current_state) {
        case STATE_START:
            if (event == EVENT_START) {
                return STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (is_valid_input(data) && machine->buffer_index < MAX_INPUT_LENGTH - 1) {
                    machine->buffer[machine->buffer_index++] = data;
                    machine->data_count++;
                    return STATE_READING;
                } else if (event == EVENT_PROCESS && machine->data_count > 0) {
                    return STATE_PROCESSING;
                } else if (event == EVENT_ERROR) {
                    return STATE_ERROR;
                }
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_COMPLETE) {
                return STATE_FINISHED;
            } else if (event == EVENT_ERROR) {
                return STATE_ERROR;
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
    
    printf("Processing %d characters: ", machine->data_count);
    for (size_t i = 0; i < machine->buffer_index; i++) {
        if (machine->buffer[i] >= 'a' && machine->buffer[i] <= 'z') {
            machine->buffer[i] = machine->buffer[i] - 'a' + 'A';
        }
    }
    printf("%s\n", machine->buffer);
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("State Machine Demo - Text Processor\n");
    printf("Enter text (max %d chars). Commands: 'p'=process, 'r'=reset, 'q'=quit\n", MAX_INPUT_LENGTH - 1);
    
    char input;
    int running = 1;
    
    while (running) {
        if (scanf("%c", &input) != 1) {
            machine.current_state = handle_event(&machine, EVENT_ERROR, 0);
            continue;
        }
        
        switch (input) {
            case 'p':
            case 'P':
                machine.current_state = handle_event(&machine, EVENT_PROCESS, 0);
                if (machine.current_state == STATE_PROCESSING) {
                    process_data(&machine);
                    machine.current_state = handle_event(&machine, EVENT_COMPLETE, 0);
                }
                break;
                
            case 'r':
            case 'R':
                machine.current_state = handle_event(&machine, EVENT_RESET, 0);
                printf("Machine reset to START state\n");
                break;
                
            case 'q':
            case 'Q':
                running = 0;
                break;
                
            case '\n':
                break;
                
            default:
                if (machine.current_state == STATE_READING || machine.current_state == STATE_START) {
                    if (machine.current_state == STATE_START) {
                        machine.current_state = handle_event(&machine, EVENT_START, 0);
                    }
                    machine.current_state = handle_event(&machine, EVENT_DATA, input);
                }
                break;
        }
        
        if (machine.current_state == STATE_ERROR) {
            printf("Error state reached. Press 'r' to reset.\n");
        }
    }
    
    printf("Final state: ");
    switch (machine.current_state) {
        case STATE_START: printf("START\n"); break;
        case STATE_READING: printf("READING\n"); break;
        case STATE_PROCESSING: printf("PROCESSING\n"); break;
        case STATE_FINISHED: printf("FINISHED\n"); break;
        case STATE_ERROR: printf("ERROR\n"); break;
    }
    
    return 0;
}