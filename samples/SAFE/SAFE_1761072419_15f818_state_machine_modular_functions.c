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
    memset(machine->buffer, 0, sizeof(machine->buffer));
    machine->buffer_pos = 0;
    machine->data_count = 0;
}

int is_valid_input_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
           (c >= '0' && c <= '9') || c == ' ' || c == '\n';
}

State handle_start_state(StateMachine *machine, Event event, char data) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *machine, Event event, char data) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (machine->buffer_pos < MAX_INPUT_LEN - 1 && is_valid_input_char(data)) {
                machine->buffer[machine->buffer_pos++] = data;
                return STATE_READING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_COMPLETE:
            if (machine->buffer_pos > 0) {
                machine->buffer[machine->buffer_pos] = '\0';
                return STATE_PROCESSING;
            } else {
                return STATE_ERROR;
            }
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

State handle_processing_state(StateMachine *machine, Event event, char data) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            machine->data_count++;
            printf("Processed: %s (count: %d)\n", machine->buffer, machine->data_count);
            return STATE_FINISHED;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

State handle_finished_state(StateMachine *machine, Event event, char data) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_START;
        default:
            return STATE_FINISHED;
    }
}

State handle_error_state(StateMachine *machine, Event event, char data) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_machine(machine);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State process_event(StateMachine *machine, Event event, char data) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (machine->current_state) {
        case STATE_START:
            return handle_start_state(machine, event, data);
        case STATE_READING:
            return handle_reading_state(machine, event, data);
        case STATE_PROCESSING:
            return handle_processing_state(machine, event, data);
        case STATE_FINISHED:
            return handle_finished_state(machine, event, data);
        case STATE_ERROR:
            return handle_error_state(machine, event, data);
        default:
            return STATE_ERROR;
    }
}

Event get_event_from_input(char input) {
    if (input == '\n') return EVENT_COMPLETE;
    if (input == '!') return EVENT_ERROR;
    if (input == '@') return EVENT_RESET;
    if (is_valid_input_char(input)) return EVENT_DATA;
    return EVENT_ERROR;
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("State Machine Demo\n");
    printf("Enter text (max %d chars):\n", MAX_INPUT_LEN - 1);
    printf("Press Enter to process, ! for error, @ to reset\n");
    
    int c;
    while ((c = getchar()) != EOF) {
        if (c == EOF) break;
        
        Event event = get_event_from_input((char)c);
        State new_state = process_event(&machine, event, (char)c);
        
        if (new_state != machine.current_state) {
            machine.current_state = new_state;
        }
        
        if (machine.current_state == STATE_FINISHED) {
            printf("Operation completed. Press @ to reset or continue.\n");
        } else if (machine.current_state == STATE_ERROR) {
            printf("Error occurred. Press @ to reset.\n");
        }
        
        if (c == '\n') {
            fflush(stdout);
        }
    }
    
    return 0;
}