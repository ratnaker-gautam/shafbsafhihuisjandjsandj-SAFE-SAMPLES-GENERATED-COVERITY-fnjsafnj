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
    EVENT_PROCESS,
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_INVALID
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

State handle_event(State current, Event event, StateMachine* machine);
Event get_next_event(const char* input);
int validate_input(const char* input);
void initialize_machine(StateMachine* machine);
void process_data(StateMachine* machine);

State handle_event(State current, Event event, StateMachine* machine) {
    switch(current) {
        case STATE_START:
            if (event == EVENT_START) {
                machine->data_count = 0;
                machine->buffer_len = 0;
                return STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (machine->buffer_len < MAX_INPUT_LEN - 1) {
                    machine->buffer[machine->buffer_len++] = 'D';
                    machine->data_count++;
                    return STATE_READING;
                } else {
                    return STATE_ERROR;
                }
            } else if (event == EVENT_PROCESS) {
                return STATE_PROCESSING;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_COMPLETE) {
                process_data(machine);
                return STATE_FINISHED;
            } else if (event == EVENT_ERROR) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_FINISHED:
            if (event == EVENT_START) {
                return STATE_START;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_START) {
                return STATE_START;
            }
            break;
    }
    
    return current;
}

Event get_next_event(const char* input) {
    if (input == NULL) {
        return EVENT_INVALID;
    }
    
    if (strlen(input) == 0) {
        return EVENT_INVALID;
    }
    
    switch(input[0]) {
        case 's': return EVENT_START;
        case 'd': return EVENT_DATA;
        case 'p': return EVENT_PROCESS;
        case 'c': return EVENT_COMPLETE;
        case 'e': return EVENT_ERROR;
        default: return EVENT_INVALID;
    }
}

int validate_input(const char* input) {
    if (input == NULL) {
        return 0;
    }
    
    size_t len = strlen(input);
    if (len == 0 || len > 10) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 'a' || input[i] > 'z') {
            return 0;
        }
    }
    
    return 1;
}

void initialize_machine(StateMachine* machine) {
    if (machine == NULL) {
        return;
    }
    
    machine->current_state = STATE_START;
    machine->buffer_len = 0;
    machine->data_count = 0;
    memset(machine->buffer, 0, MAX_INPUT_LEN);
}

void process_data(StateMachine* machine) {
    if (machine == NULL || machine->buffer_len == 0) {
        return;
    }
    
    printf("Processing %d data items: ", machine->data_count);
    for (size_t i = 0; i < machine->buffer_len; i++) {
        printf("%c", machine->buffer[i]);
    }
    printf("\n");
}

int main(void) {
    StateMachine machine;
    char input[20];
    int running = 1;
    
    initialize_machine(&machine);
    
    printf("State Machine Controller\n");
    printf("Commands: start(s), data(d), process(p), complete(c), error(e), quit(q)\n");
    
    while (running) {
        printf("Current state: ");
        switch(machine.current_state) {
            case STATE_START: printf("START"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_FINISHED: printf("FINISHED"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf("\nEnter command: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "q") == 0 || strcmp(input, "quit") == 0) {
            running = 0;
            continue;
        }
        
        if (!validate_input(input)) {
            printf("Invalid input. Use single letter commands.\n");
            continue;
        }
        
        Event event = get_next_event(input);
        if (event == EVENT_INVALID) {
            printf("Unknown command.\n");
            continue;
        }
        
        State new_state = handle_event(machine.current_state, event, &machine);
        
        if (new_state != machine.current_state) {
            printf("State transition: ");
            switch(machine