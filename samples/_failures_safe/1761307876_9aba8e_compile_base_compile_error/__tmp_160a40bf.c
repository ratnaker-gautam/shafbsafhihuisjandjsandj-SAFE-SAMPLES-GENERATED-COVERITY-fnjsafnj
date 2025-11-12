//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_TRANSITIONS 20

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_DONE,
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
    {STATE_READING, EVENT_ERROR, STATE_ERROR},
    {STATE_PROCESSING, EVENT_COMPLETE, STATE_DONE},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR},
    {STATE_DONE, EVENT_RESET, STATE_IDLE},
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

State handle_event(StateMachine *machine, Event event) {
    if (machine == NULL) return STATE_ERROR;
    
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == machine->current_state && 
            transitions[i].event == event) {
            machine->current_state = transitions[i].next_state;
            break;
        }
    }
    return machine->current_state;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (!isalnum((unsigned char)input[i]) && input[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

int process_data(StateMachine *machine) {
    if (machine == NULL || machine->current_state != STATE_PROCESSING) return 0;
    if (machine->buffer_len == 0) return 0;
    
    machine->data_count = 0;
    for (size_t i = 0; i < machine->buffer_len; i++) {
        if (isalpha((unsigned char)machine->buffer[i])) {
            machine->data_count++;
        }
    }
    return 1;
}

void reset_machine(StateMachine *machine) {
    if (machine == NULL) return;
    initialize_machine(machine);
}

void print_state_info(const StateMachine *machine) {
    if (machine == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READING", "PROCESSING", "DONE", "ERROR"
    };
    
    printf("Current state: %s\n", state_names[machine->current_state]);
    if (machine->current_state == STATE_DONE) {
        printf("Processed %d alphabetic characters\n", machine->data_count);
    }
}

int main(void) {
    StateMachine machine;
    char input[MAX_INPUT_LEN];
    
    initialize_machine(&machine);
    
    printf("State Machine Demo - Character Counter\n");
    printf("Commands: start, data <text>, process, reset, quit\n");
    
    while (1) {
        print_state_info(&machine);
        printf("> ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
            len--;
        }
        
        if (strcmp(input, "quit") == 0) {
            break;
        } else if (strcmp(input, "start") == 0) {
            handle_event(&machine, EVENT_START);
        } else if (strncmp(input, "data ", 5) == 0) {
            const char *text = input + 5;
            size_t text_len = strlen(text);
            
            if (validate_input(text, text_len)) {
                if (text_len < sizeof(machine.buffer) - machine.buffer_len) {
                    memcpy(machine.buffer + machine.buffer_len, text, text_len);
                    machine.buffer_len += text_len;
                    handle_event(&machine, EVENT_DATA);
                } else {
                    handle_event(&machine, EVENT_ERROR);
                }
            } else {
                handle_event(&machine, EVENT_ERROR);
            }
        } else if (strcmp(input, "process") == 0) {
            if (machine.current_state == STATE_READING && machine.buffer_len > 0) {
                handle_event(&machine, EVENT_PROCESS);
                if (process_data(&machine)) {
                    handle_event