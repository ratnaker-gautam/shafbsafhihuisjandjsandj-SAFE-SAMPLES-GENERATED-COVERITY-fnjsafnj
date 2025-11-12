//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR,
    STATE_COUNT
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    EVENT_COUNT
} Event;

typedef struct {
    State current_state;
    int data_value;
    int process_count;
} StateMachine;

void handle_idle(StateMachine *sm, Event event);
void handle_ready(StateMachine *sm, Event event);
void handle_processing(StateMachine *sm, Event event);
void handle_complete(StateMachine *sm, Event event);
void handle_error(StateMachine *sm, Event event);

typedef void (*StateHandler)(StateMachine *, Event);

static const StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

void handle_idle(StateMachine *sm, Event event) {
    switch (event) {
        case EVENT_START:
            sm->current_state = STATE_READY;
            sm->data_value = 0;
            sm->process_count = 0;
            printf("Transition: IDLE -> READY\n");
            break;
        case EVENT_RESET:
            printf("Already in IDLE state\n");
            break;
        default:
            printf("Invalid event for IDLE state\n");
            break;
    }
}

void handle_ready(StateMachine *sm, Event event) {
    switch (event) {
        case EVENT_DATA:
            sm->data_value = 42;
            sm->current_state = STATE_PROCESSING;
            printf("Transition: READY -> PROCESSING (data loaded)\n");
            break;
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            printf("Transition: READY -> IDLE\n");
            break;
        default:
            printf("Invalid event for READY state\n");
            break;
    }
}

void handle_processing(StateMachine *sm, Event event) {
    switch (event) {
        case EVENT_PROCESS:
            if (sm->data_value > 0) {
                sm->process_count++;
                if (sm->process_count >= 3) {
                    sm->current_state = STATE_COMPLETE;
                    printf("Transition: PROCESSING -> COMPLETE\n");
                } else {
                    printf("Processing iteration %d\n", sm->process_count);
                }
            } else {
                sm->current_state = STATE_ERROR;
                printf("Transition: PROCESSING -> ERROR (invalid data)\n");
            }
            break;
        case EVENT_FAILURE:
            sm->current_state = STATE_ERROR;
            printf("Transition: PROCESSING -> ERROR\n");
            break;
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            printf("Transition: PROCESSING -> IDLE\n");
            break;
        default:
            printf("Invalid event for PROCESSING state\n");
            break;
    }
}

void handle_complete(StateMachine *sm, Event event) {
    switch (event) {
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            printf("Transition: COMPLETE -> IDLE\n");
            break;
        default:
            printf("Invalid event for COMPLETE state\n");
            break;
    }
}

void handle_error(StateMachine *sm, Event event) {
    switch (event) {
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            printf("Transition: ERROR -> IDLE\n");
            break;
        default:
            printf("Invalid event for ERROR state\n");
            break;
    }
}

void state_machine_dispatch(StateMachine *sm, Event event) {
    if (sm == NULL || event < 0 || event >= EVENT_COUNT) {
        return;
    }
    if (sm->current_state < 0 || sm->current_state >= STATE_COUNT) {
        return;
    }
    state_handlers[sm->current_state](sm, event);
}

Event get_user_event() {
    char input[32];
    printf("\nAvailable events:\n");
    printf("0: START, 1: DATA, 2: PROCESS, 3: SUCCESS, 4: FAILURE, 5: RESET\n");
    printf("Enter event number: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_COUNT;
    }
    
    char *endptr;
    long value = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        return EVENT_COUNT;
    }
    
    if (value < 0 || value >= EVENT_COUNT) {
        return EVENT_COUNT;
    }
    
    return (Event)value;
}

int main() {
    StateMachine sm = {STATE_IDLE, 0, 0};
    
    printf("State Machine Demo Started\n");
    printf("Initial state: IDLE\n");
    
    while (1) {
        Event event = get_user_event();
        if (event == EVENT_COUNT) {
            printf("Invalid event input\n");
            continue;
        }
        
        state_machine_dispatch(&sm, event);
        
        if (sm.current_state == STATE_COMPLETE) {
            printf("Processing completed successfully after %d iterations\n", sm.process_count);
            printf("Final data value: %d\n", sm.data_value);
            break;