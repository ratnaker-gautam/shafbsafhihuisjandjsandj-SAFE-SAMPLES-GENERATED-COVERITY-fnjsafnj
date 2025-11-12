//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 50

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    Event event;
    State next_state;
} Transition;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

static const Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING},
    {STATE_READING, EVENT_DATA, STATE_READING},
    {STATE_READING, EVENT_PROCESS, STATE_PROCESSING},
    {STATE_PROCESSING, EVENT_SUCCESS, STATE_COMPLETE},
    {STATE_PROCESSING, EVENT_FAILURE, STATE_ERROR},
    {STATE_COMPLETE, EVENT_RESET, STATE_IDLE},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

void initialize_machine(StateMachine *machine) {
    if (machine == NULL) return;
    machine->current_state = STATE_IDLE;
    machine->buffer_len = 0;
    machine->data_count = 0;
    memset(machine->buffer, 0, sizeof(machine->buffer));
}

State get_next_state(State current, Event event) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == current && transitions[i].event == event) {
            return transitions[i].next_state;
        }
    }
    return current;
}

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isalnum((unsigned char)input[i]) && input[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

int handle_event(StateMachine *machine, Event event, const char *data) {
    if (machine == NULL) return 0;
    
    State next_state = get_next_state(machine->current_state, event);
    
    switch (event) {
        case EVENT_START:
            if (machine->current_state == STATE_IDLE) {
                machine->current_state = next_state;
                printf("Machine started. Ready to receive data.\n");
                return 1;
            }
            break;
            
        case EVENT_DATA:
            if (machine->current_state == STATE_READING && data != NULL) {
                if (is_valid_input(data)) {
                    size_t data_len = strlen(data);
                    if (machine->buffer_len + data_len < MAX_INPUT_LEN - 1) {
                        strcat(machine->buffer, data);
                        machine->buffer_len += data_len;
                        machine->data_count++;
                        printf("Data added: %s (Total: %zu chars, %d items)\n", 
                               data, machine->buffer_len, machine->data_count);
                        machine->current_state = next_state;
                        return 1;
                    }
                }
            }
            break;
            
        case EVENT_PROCESS:
            if (machine->current_state == STATE_READING && machine->data_count > 0) {
                machine->current_state = next_state;
                printf("Processing %d data items...\n", machine->data_count);
                return 1;
            }
            break;
            
        case EVENT_SUCCESS:
            if (machine->current_state == STATE_PROCESSING) {
                machine->current_state = next_state;
                printf("Processing completed successfully.\n");
                return 1;
            }
            break;
            
        case EVENT_FAILURE:
            if (machine->current_state == STATE_PROCESSING) {
                machine->current_state = next_state;
                printf("Processing failed.\n");
                return 1;
            }
            break;
            
        case EVENT_RESET:
            machine->current_state = next_state;
            initialize_machine(machine);
            printf("Machine reset to idle state.\n");
            return 1;
            
        default:
            break;
    }
    
    return 0;
}

void print_state(State state) {
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "COMPLETE", "ERROR"
    };
    if (state < STATE_IDLE || state > STATE_ERROR) {
        printf("UNKNOWN");
    } else {
        printf("%s", state_names[state]);
    }
}

void print_menu(void) {
    printf("\nCurrent commands:\n");
    printf("1 - START\n");
    printf("2 - ADD DATA\n");
    printf("3 - PROCESS\n");
    printf("4 - SUCCESS\n");
    printf("5 - FAILURE\n");
    printf("6 - RESET\n");
    printf("0 - EXIT\n");
    printf("Choose: ");
}

int main(void) {
    StateMachine machine;
    char input[MAX_INPUT_LEN];
    int choice;
    
    initialize