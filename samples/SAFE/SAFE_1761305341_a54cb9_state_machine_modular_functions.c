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
    EVENT_PROCESS,
    EVENT_DONE,
    EVENT_ERROR
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int processed_count;
} StateMachine;

State handle_event(State current_state, Event event, StateMachine* machine);
int validate_input(const char* input, size_t len);
void process_data(StateMachine* machine);
void initialize_machine(StateMachine* machine);

void initialize_machine(StateMachine* machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_START;
    memset(machine->buffer, 0, sizeof(machine->buffer));
    machine->buffer_len = 0;
    machine->processed_count = 0;
}

int validate_input(const char* input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    return 1;
}

void process_data(StateMachine* machine) {
    if (machine == NULL || machine->buffer_len == 0) return;
    
    int word_count = 0;
    int in_word = 0;
    
    for (size_t i = 0; i < machine->buffer_len; i++) {
        if (machine->buffer[i] == ' ' || machine->buffer[i] == '\t' || machine->buffer[i] == '\n') {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            word_count++;
        }
    }
    
    machine->processed_count = word_count;
}

State handle_event(State current_state, Event event, StateMachine* machine) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (current_state) {
        case STATE_START:
            if (event == EVENT_START) {
                return STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                return STATE_PROCESSING;
            } else if (event == EVENT_ERROR) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_PROCESS) {
                process_data(machine);
                return STATE_FINISHED;
            } else if (event == EVENT_ERROR) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_FINISHED:
            if (event == EVENT_DONE) {
                return STATE_START;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_START) {
                initialize_machine(machine);
                return STATE_START;
            }
            break;
            
        default:
            return STATE_ERROR;
    }
    
    return current_state;
}

int main(void) {
    StateMachine machine;
    char input[MAX_INPUT_LEN];
    
    initialize_machine(&machine);
    
    printf("State Machine Demo - Word Counter\n");
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input, input_len)) {
        printf("Invalid input detected\n");
        machine.current_state = handle_event(machine.current_state, EVENT_ERROR, &machine);
    } else {
        machine.current_state = handle_event(machine.current_state, EVENT_START, &machine);
        
        if (machine.current_state == STATE_READING) {
            strncpy(machine.buffer, input, sizeof(machine.buffer) - 1);
            machine.buffer[sizeof(machine.buffer) - 1] = '\0';
            machine.buffer_len = input_len;
            
            machine.current_state = handle_event(machine.current_state, EVENT_DATA, &machine);
        }
        
        if (machine.current_state == STATE_PROCESSING) {
            machine.current_state = handle_event(machine.current_state, EVENT_PROCESS, &machine);
        }
        
        if (machine.current_state == STATE_FINISHED) {
            printf("Processing complete. Found %d words.\n", machine.processed_count);
            machine.current_state = handle_event(machine.current_state, EVENT_DONE, &machine);
        }
    }
    
    if (machine.current_state == STATE_ERROR) {
        printf("An error occurred during processing.\n");
        return 1;
    }
    
    return 0;
}