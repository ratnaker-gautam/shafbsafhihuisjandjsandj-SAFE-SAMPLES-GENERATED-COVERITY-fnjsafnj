//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
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

int is_valid_input_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '.' || c == ',';
}

int handle_idle_state(StateMachine *machine) {
    printf("Machine is idle. Enter 'start' to begin: ");
    char input[20];
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "start") == 0) {
        machine->current_state = get_next_state(machine->current_state, EVENT_START);
        printf("Transitioned to READING state.\n");
    }
    return 1;
}

int handle_reading_state(StateMachine *machine) {
    printf("Reading mode. Enter data (max %d chars) or 'process' to continue: ", MAX_INPUT_LEN);
    char input[MAX_INPUT_LEN + 10];
    if (fgets(input, sizeof(input), stdin) == NULL) return 0;
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "process") == 0) {
        if (machine->buffer_len > 0) {
            machine->current_state = get_next_state(machine->current_state, EVENT_PROCESS);
            printf("Transitioned to PROCESSING state.\n");
        } else {
            printf("No data to process. Please enter some data first.\n");
        }
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0) {
        printf("Empty input ignored.\n");
        return 1;
    }
    
    for (size_t i = 0; i < input_len; i++) {
        if (!is_valid_input_char(input[i])) {
            printf("Invalid character detected. Only alphanumeric, space, '.', and ',' allowed.\n");
            return 1;
        }
    }
    
    if (machine->buffer_len + input_len >= MAX_INPUT_LEN) {
        printf("Input too large. Buffer full.\n");
        return 1;
    }
    
    memcpy(machine->buffer + machine->buffer_len, input, input_len);
    machine->buffer_len += input_len;
    machine->data_count++;
    printf("Data added. Total characters: %zu, Data entries: %d\n", machine->buffer_len, machine->data_count);
    machine->current_state = get_next_state(machine->current_state, EVENT_DATA);
    return 1;
}

int handle_processing_state(StateMachine *machine) {
    printf("Processing data...\n");
    
    if (machine->buffer_len == 0) {
        printf("No data to process. Transitioning to ERROR state.\n");
        machine->current_state = get_next_state(machine->current_state, EVENT_ERROR);
        return 1;
    }
    
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
    
    printf("Processing complete. Found %d words in %zu characters.\n", word_count, machine->buffer_len);
    
    if (word_count > 0) {
        machine->current_state = get