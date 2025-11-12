//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STATES 10
#define MAX_TRANSITIONS 20
#define MAX_EVENTS 10
#define BUFFER_SIZE 256

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

typedef struct {
    State current_state;
    Transition *transitions;
    size_t transition_count;
    char *name;
} StateMachine;

StateMachine* create_state_machine(const char *name) {
    if (name == NULL || strlen(name) == 0) {
        return NULL;
    }
    
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (sm == NULL) {
        return NULL;
    }
    
    sm->name = malloc(strlen(name) + 1);
    if (sm->name == NULL) {
        free(sm);
        return NULL;
    }
    strcpy(sm->name, name);
    
    sm->transitions = malloc(MAX_TRANSITIONS * sizeof(Transition));
    if (sm->transitions == NULL) {
        free(sm->name);
        free(sm);
        return NULL;
    }
    
    sm->current_state = STATE_IDLE;
    sm->transition_count = 0;
    
    return sm;
}

int add_transition(StateMachine *sm, State from, Event event, State to) {
    if (sm == NULL || sm->transition_count >= MAX_TRANSITIONS) {
        return 0;
    }
    
    Transition *t = &sm->transitions[sm->transition_count];
    t->current_state = from;
    t->event = event;
    t->next_state = to;
    sm->transition_count++;
    
    return 1;
}

State process_event(StateMachine *sm, Event event) {
    if (sm == NULL) {
        return STATE_ERROR;
    }
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        Transition *t = &sm->transitions[i];
        if (t->current_state == sm->current_state && t->event == event) {
            sm->current_state = t->next_state;
            return sm->current_state;
        }
    }
    
    return sm->current_state;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm != NULL) {
        free(sm->name);
        free(sm->transitions);
        free(sm);
    }
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

int main() {
    StateMachine *machine = create_state_machine("TaskProcessor");
    if (machine == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    add_transition(machine, STATE_IDLE, EVENT_START, STATE_ACTIVE);
    add_transition(machine, STATE_ACTIVE, EVENT_PAUSE, STATE_PAUSED);
    add_transition(machine, STATE_ACTIVE, EVENT_COMPLETE, STATE_DONE);
    add_transition(machine, STATE_ACTIVE, EVENT_ERROR, STATE_ERROR);
    add_transition(machine, STATE_PAUSED, EVENT_RESUME, STATE_ACTIVE);
    add_transition(machine, STATE_PAUSED, EVENT_STOP, STATE_IDLE);
    add_transition(machine, STATE_ERROR, EVENT_STOP, STATE_IDLE);
    
    char input[BUFFER_SIZE];
    printf("State Machine Simulation (enter event number 0-5 or 'q' to quit):\n");
    printf("0=START, 1=PAUSE, 2=RESUME, 3=STOP, 4=ERROR, 5=COMPLETE\n");
    
    while (1) {
        printf("Current state: %s\n", state_to_string(machine->current_state));
        printf("Enter event: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (input[0] == 'q' || input[0] == 'Q') {
            break;
        }
        
        char *endptr;
        long choice = strtol(input, &endptr, 10);
        if (endptr == input || choice < 0 || choice > 5) {
            printf("Invalid input. Please enter 0-5 or 'q'.\n");
            continue;
        }
        
        Event selected_event = (Event)choice