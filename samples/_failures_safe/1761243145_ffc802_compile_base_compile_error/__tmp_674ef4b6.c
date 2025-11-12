//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10
#define MAX_EVENTS 10

typedef enum {
    STATE_IDLE,
    STATE_ACTIVE,
    STATE_PAUSED,
    STATE_ERROR,
    STATE_DONE
} State;

typedef enum {
    EVENT_START,
    EVENT_PAUSE,
    EVENT_RESUME,
    EVENT_STOP,
    EVENT_ERROR,
    EVENT_COMPLETE
} Event;

typedef struct {
    State current_state;
    Event event;
    State next_state;
} Transition;

static const Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_ACTIVE},
    {STATE_ACTIVE, EVENT_PAUSE, STATE_PAUSED},
    {STATE_ACTIVE, EVENT_STOP, STATE_IDLE},
    {STATE_ACTIVE, EVENT_ERROR, STATE_ERROR},
    {STATE_ACTIVE, EVENT_COMPLETE, STATE_DONE},
    {STATE_PAUSED, EVENT_RESUME, STATE_ACTIVE},
    {STATE_PAUSED, EVENT_STOP, STATE_IDLE},
    {STATE_ERROR, EVENT_START, STATE_IDLE},
    {STATE_DONE, EVENT_START, STATE_IDLE}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

State handle_transition(State current, Event event) {
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].current_state == current && transitions[i].event == event) {
            return transitions[i].next_state;
        }
    }
    return current;
}

int get_user_input(char* buffer, size_t buffer_size) {
    if (buffer == NULL || buffer_size == 0) {
        return -1;
    }
    
    printf("Enter event (start, pause, resume, stop, error, complete, quit): ");
    
    if (fgets(buffer, (int)buffer_size, stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 0;
}

Event parse_event(const char* input) {
    if (input == NULL) {
        return EVENT_ERROR;
    }
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "pause") == 0) return EVENT_PAUSE;
    if (strcmp(input, "resume") == 0) return EVENT_RESUME;
    if (strcmp(input, "stop") == 0) return EVENT_STOP;
    if (strcmp(input, "error") == 0) return EVENT_ERROR;
    if (strcmp(input, "complete") == 0) return EVENT_COMPLETE;
    
    return EVENT_ERROR;
}

const char* state_to_string(State state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_ACTIVE: return "ACTIVE";
        case STATE_PAUSED: return "PAUSED";
        case STATE_ERROR: return "ERROR";
        case STATE_DONE: return "DONE";
        default: return "UNKNOWN";
    }
}

const char* event_to_string(Event event) {
    switch (event) {
        case EVENT_START: return "START";
        case EVENT_PAUSE: return "PAUSE";
        case EVENT_RESUME: return "RESUME";
        case EVENT_STOP: return "STOP";
        case EVENT_ERROR: return "ERROR";
        case EVENT_COMPLETE: return "COMPLETE";
        default: return "UNKNOWN";
    }
}

void print_state_info(State current_state) {
    printf("Current state: %s\n", state_to_string(current_state));
    printf("Available events: ");
    
    switch (current_state) {
        case STATE_IDLE:
            printf("start");
            break;
        case STATE_ACTIVE:
            printf("pause, stop, error, complete");
            break;
        case STATE_PAUSED:
            printf("resume, stop");
            break;
        case STATE_ERROR:
            printf("start");
            break;
        case STATE_DONE:
            printf("start");
            break;
        default:
            printf("none");
    }
    printf("\n");
}

int main(void) {
    State current_state = STATE_IDLE;
    char input_buffer[MAX_INPUT_LEN];
    
    printf("State Machine Demo\n");
    printf("Type 'quit' to exit\n\n");
    
    while (1) {
        print_state_info(current_state);
        
        if (get_user_input(input_buffer, sizeof(input_buffer)) != 0) {
            printf("Input error occurred\n");
            continue;
        }
        
        if (strcmp(input_buffer, "quit") == 0) {
            printf("Exiting state machine\n");
            break;
        }
        
        Event event = parse_event(input_buffer);
        if (event == EVENT_ERROR && strcmp(input_buffer, "error") != 0) {
            printf("Invalid event: %s\n", input_buffer);
            continue;
        }
        
        State new_state = handle_transition(current_state, event);
        
        if (new_state != current_state) {
            printf("Transition: %s -> %s via %s\n", 
                   state_to_string(current_state), 
                   state_to_string(new_state),
                   event