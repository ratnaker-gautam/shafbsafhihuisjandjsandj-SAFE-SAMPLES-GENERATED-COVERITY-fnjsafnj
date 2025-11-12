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
    STATE_PROCESSING,
    STATE_ERROR,
    STATE_DONE
} StateType;

typedef struct {
    int id;
    StateType type;
    char name[32];
} State;

typedef struct {
    int from_state;
    int to_state;
    int event_id;
    char description[64];
} Transition;

typedef struct {
    State *states;
    Transition *transitions;
    int current_state;
    int state_count;
    int transition_count;
} StateMachine;

StateMachine* create_state_machine(void) {
    StateMachine *sm = malloc(sizeof(StateMachine));
    if (!sm) return NULL;
    
    sm->state_count = 5;
    sm->states = malloc(sizeof(State) * sm->state_count);
    if (!sm->states) {
        free(sm);
        return NULL;
    }
    
    sm->transition_count = 8;
    sm->transitions = malloc(sizeof(Transition) * sm->transition_count);
    if (!sm->transitions) {
        free(sm->states);
        free(sm);
        return NULL;
    }
    
    State states[] = {
        {0, STATE_IDLE, "Idle"},
        {1, STATE_ACTIVE, "Active"},
        {2, STATE_PROCESSING, "Processing"},
        {3, STATE_ERROR, "Error"},
        {4, STATE_DONE, "Done"}
    };
    
    memcpy(sm->states, states, sizeof(states));
    
    Transition transitions[] = {
        {0, 1, 1, "Start processing"},
        {1, 2, 2, "Begin work"},
        {2, 4, 3, "Complete successfully"},
        {2, 3, 4, "Encounter error"},
        {3, 1, 5, "Retry from error"},
        {3, 0, 6, "Reset from error"},
        {1, 0, 7, "Cancel operation"},
        {4, 0, 8, "Reset from done"}
    };
    
    memcpy(sm->transitions, transitions, sizeof(transitions));
    sm->current_state = 0;
    
    return sm;
}

void destroy_state_machine(StateMachine *sm) {
    if (sm) {
        free(sm->states);
        free(sm->transitions);
        free(sm);
    }
}

int process_event(StateMachine *sm, int event_id) {
    if (!sm || event_id < 1 || event_id > MAX_EVENTS) return 0;
    
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].from_state == sm->current_state && 
            sm->transitions[i].event_id == event_id) {
            sm->current_state = sm->transitions[i].to_state;
            return 1;
        }
    }
    return 0;
}

void print_current_state(const StateMachine *sm) {
    if (!sm || sm->current_state < 0 || sm->current_state >= sm->state_count) return;
    
    printf("Current state: %s\n", sm->states[sm->current_state].name);
}

void print_available_events(const StateMachine *sm) {
    if (!sm) return;
    
    printf("Available events: ");
    int found = 0;
    for (int i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].from_state == sm->current_state) {
            if (found) printf(", ");
            printf("%d (%s)", sm->transitions[i].event_id, sm->transitions[i].description);
            found = 1;
        }
    }
    if (!found) printf("None");
    printf("\n");
}

int main(void) {
    StateMachine *sm = create_state_machine();
    if (!sm) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }
    
    char input[BUFFER_SIZE];
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Enter event numbers to transition states, 'q' to quit\n\n");
    
    while (running) {
        print_current_state(sm);
        print_available_events(sm);
        
        printf("Enter event: ");
        if (!fgets(input, sizeof(input), stdin)) {
            break;
        }
        
        if (input[0] == 'q' && (input[1] == '\n' || input[1] == '\0')) {
            break;
        }
        
        char *endptr;
        long event = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\n' || event < 1 || event > INT32_MAX) {
            printf("Invalid input. Please enter a valid event number.\n");
            continue;
        }
        
        if (!process_event(sm, (int)event)) {
            printf("Invalid event for current state.\n");
        }
        
        printf("\n");
    }
    
    destroy_state_machine(sm);
    return 0;
}