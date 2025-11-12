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

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LENGTH) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

State handle_event(StateMachine *machine, Event event, const char *data) {
    if (machine == NULL) return STATE_ERROR;
    
    switch (machine->current_state) {
        case STATE_START:
            if (event == EVENT_START) {
                printf("State machine started\n");
                return STATE_READING;
            }
            break;
            
        case STATE_READING:
            if (event == EVENT_DATA) {
                if (validate_input(data)) {
                    size_t data_len = strlen(data);
                    if (machine->buffer_index + data_len < MAX_INPUT_LENGTH - 1) {
                        strncpy(machine->buffer + machine->buffer_index, data, MAX_INPUT_LENGTH - machine->buffer_index - 1);
                        machine->buffer_index += data_len;
                        machine->data_count++;
                        printf("Data received: %s\n", data);
                        return STATE_READING;
                    } else {
                        return STATE_ERROR;
                    }
                } else {
                    return STATE_ERROR;
                }
            } else if (event == EVENT_PROCESS && machine->data_count > 0) {
                printf("Processing %d data items\n", machine->data_count);
                return STATE_PROCESSING;
            }
            break;
            
        case STATE_PROCESSING:
            if (event == EVENT_COMPLETE) {
                printf("Processing complete. Final data: %s\n", machine->buffer);
                return STATE_FINISHED;
            } else if (event == EVENT_ERROR) {
                return STATE_ERROR;
            }
            break;
            
        case STATE_FINISHED:
            if (event == EVENT_RESET) {
                initialize_machine(machine);
                printf("State machine reset\n");
                return STATE_START;
            }
            break;
            
        case STATE_ERROR:
            if (event == EVENT_RESET) {
                initialize_machine(machine);
                printf("State machine reset after error\n");
                return STATE_START;
            }
            break;
    }
    
    return machine->current_state;
}

Event get_user_event(void) {
    char input[MAX_INPUT_LENGTH];
    
    printf("\nAvailable events:\n");
    printf("1 - START\n");
    printf("2 - DATA\n");
    printf("3 - PROCESS\n");
    printf("4 - COMPLETE\n");
    printf("5 - ERROR\n");
    printf("6 - RESET\n");
    printf("0 - EXIT\n");
    printf("Choose event: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    int choice;
    if (sscanf(input, "%d", &choice) != 1) {
        return EVENT_ERROR;
    }
    
    switch (choice) {
        case 0: return EVENT_ERROR;
        case 1: return EVENT_START;
        case 2: return EVENT_DATA;
        case 3: return EVENT_PROCESS;
        case 4: return EVENT_COMPLETE;
        case 5: return EVENT_ERROR;
        case 6: return EVENT_RESET;
        default: return EVENT_ERROR;
    }
}

const char* state_to_string(State state) {
    switch (state) {
        case STATE_START: return "START";
        case STATE_READING: return "READING";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_FINISHED: return "FINISHED";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main(void) {
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("State Machine Demo\n");
    printf("==================\n");
    
    while (1) {
        Event event = get_user_event();
        
        if (event == EVENT_ERROR && machine.current_state != STATE_ERROR) {
            printf("Invalid event selection\n");
            continue;
        }
        
        char data_input[MAX_INPUT_LENGTH] = {0};
        if (event == EVENT_DATA) {
            printf("Enter data: ");
            if (fgets(data_input, sizeof(data_input), stdin) == NULL) {
                event = EVENT_ERROR;
            } else {
                size_t len = strlen(data_input);