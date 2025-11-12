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
    EVENT_FAIL,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    Event event;
    State next_state;
} Transition;

static const Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING},
    {STATE_READING, EVENT_DATA, STATE_READING},
    {STATE_READING, EVENT_PROCESS, STATE_PROCESSING},
    {STATE_PROCESSING, EVENT_SUCCESS, STATE_COMPLETE},
    {STATE_PROCESSING, EVENT_FAIL, STATE_ERROR},
    {STATE_COMPLETE, EVENT_RESET, STATE_IDLE},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

State handle_event(State current, Event event) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == current && transitions[i].event == event) {
            return transitions[i].next_state;
        }
    }
    return current;
}

int get_user_input(char *buffer, size_t size) {
    if (buffer == NULL || size == 0) {
        return -1;
    }
    
    printf("Enter command (start/data/process/success/fail/reset/quit): ");
    
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 0;
}

Event parse_event(const char *input) {
    if (input == NULL) {
        return EVENT_FAIL;
    }
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS;
    if (strcmp(input, "success") == 0) return EVENT_SUCCESS;
    if (strcmp(input, "fail") == 0) return EVENT_FAIL;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    
    return EVENT_FAIL;
}

void print_state(State state) {
    switch (state) {
        case STATE_IDLE: printf("Current state: IDLE\n"); break;
        case STATE_READING: printf("Current state: READING\n"); break;
        case STATE_PROCESSING: printf("Current state: PROCESSING\n"); break;
        case STATE_COMPLETE: printf("Current state: COMPLETE\n"); break;
        case STATE_ERROR: printf("Current state: ERROR\n"); break;
        default: printf("Current state: UNKNOWN\n"); break;
    }
}

void process_state_action(State state) {
    switch (state) {
        case STATE_IDLE:
            printf("System ready. Send 'start' to begin.\n");
            break;
        case STATE_READING:
            printf("Reading data... Send 'data' for more or 'process' to continue.\n");
            break;
        case STATE_PROCESSING:
            printf("Processing data... This may take some time.\n");
            break;
        case STATE_COMPLETE:
            printf("Operation completed successfully!\n");
            break;
        case STATE_ERROR:
            printf("An error occurred during processing.\n");
            break;
        default:
            break;
    }
}

int main(void) {
    State current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN];
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Available commands: start, data, process, success, fail, reset, quit\n\n");
    
    while (running) {
        print_state(current_state);
        
        int input_result = get_user_input(input_buffer, sizeof(input_buffer));
        if (input_result != 0) {
            printf("Input error occurred.\n");
            continue;
        }
        
        if (strcmp(input_buffer, "quit") == 0) {
            printf("Exiting state machine.\n");
            break;
        }
        
        Event event = parse_event(input_buffer);
        State new_state = handle_event(current_state, event);
        
        if (new_state != current_state) {
            printf("State transition: ");
            print_state(new_state);
            current_state = new_state;
            process_state_action(current_state);
        } else {
            printf("Invalid transition from current state.\n");
        }
        
        printf("\n");
    }
    
    return 0;
}