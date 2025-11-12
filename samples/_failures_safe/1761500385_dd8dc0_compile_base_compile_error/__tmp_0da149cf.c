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
    {STATE_PROCESSING, EVENT_SUCCESS, STATE_COMPLETE},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR},
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

int process_data(StateMachine *machine) {
    if (machine == NULL || machine->buffer_len == 0) return 0;
    
    int word_count = 0;
    int in_word = 0;
    
    for (size_t i = 0; i < machine->buffer_len; i++) {
        if (isalnum((unsigned char)machine->buffer[i])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    
    machine->data_count = word_count;
    return (word_count > 0) ? 1 : 0;
}

void handle_state_idle(StateMachine *machine) {
    printf("State: IDLE\n");
    printf("Enter 'start' to begin: ");
    
    char input[MAX_INPUT_LEN];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "start") == 0) {
            machine->current_state = get_next_state(machine->current_state, EVENT_START);
        }
    }
}

void handle_state_reading(StateMachine *machine) {
    printf("State: READING (Current words: %d)\n", machine->data_count);
    printf("Enter text or 'process' to continue: ");
    
    char input[MAX_INPUT_LEN];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        
        if (strcmp(input, "process") == 0) {
            machine->current_state = get_next_state(machine->current_state, EVENT_PROCESS);
        } else if (is_valid_input(input)) {
            size_t input_len = strlen(input);
            if (machine->buffer_len + input_len + 1 < sizeof(machine->buffer)) {
                if (machine->buffer_len > 0) {
                    machine->buffer[machine->buffer_len] = ' ';
                    machine->buffer_len++;
                }
                strcpy(machine->buffer + machine->buffer_len, input);
                machine->buffer_len += input_len;
                machine->current_state = get_next_state(machine->current_state, EVENT_DATA);
            }
        }
    }
}

void handle_state_processing(StateMachine *machine) {
    printf("State: PROCESSING\n");
    
    if (process_data(machine)) {
        machine->current_state = get_next_state(machine->current_state, EVENT_SUCCESS);
    } else {
        machine->current_state = get_next_state(machine->current_state, EVENT_ERROR);
    }
}

void handle_state_complete(StateMachine *machine) {
    printf("State: COMPLETE\n");
    printf("Processed %d words: %s\n", machine->data_count, machine->buffer);
    printf("Enter 'reset' to continue: ");
    
    char input[MAX_INPUT_LEN];
    if (fgets(input, sizeof(input), stdin) != NULL) {