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
    int error_code;
} StateMachine;

void handle_idle(StateMachine *sm, Event event);
void handle_ready(StateMachine *sm, Event event);
void handle_processing(StateMachine *sm, Event event);
void handle_complete(StateMachine *sm, Event event);
void handle_error(StateMachine *sm, Event event);

typedef void (*StateHandler)(StateMachine *, Event);

StateHandler handlers[STATE_COUNT] = {
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
            printf("Transition: IDLE -> READY\n");
            break;
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            printf("Reset to IDLE state\n");
            break;
        default:
            printf("Invalid event for IDLE state\n");
            break;
    }
}

void handle_ready(StateMachine *sm, Event event) {
    switch (event) {
        case EVENT_DATA:
            sm->current_state = STATE_PROCESSING;
            printf("Transition: READY -> PROCESSING\n");
            break;
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            printf("Reset to IDLE state\n");
            break;
        default:
            printf("Invalid event for READY state\n");
            break;
    }
}

void handle_processing(StateMachine *sm, Event event) {
    switch (event) {
        case EVENT_SUCCESS:
            sm->current_state = STATE_COMPLETE;
            printf("Transition: PROCESSING -> COMPLETE\n");
            break;
        case EVENT_FAILURE:
            sm->current_state = STATE_ERROR;
            sm->error_code = 1;
            printf("Transition: PROCESSING -> ERROR\n");
            break;
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            printf("Reset to IDLE state\n");
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
            printf("Reset to IDLE state\n");
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
            sm->error_code = 0;
            printf("Reset to IDLE state\n");
            break;
        default:
            printf("Invalid event for ERROR state\n");
            break;
    }
}

void process_event(StateMachine *sm, Event event) {
    if (sm == NULL) {
        return;
    }
    if (event < 0 || event >= EVENT_COUNT) {
        printf("Invalid event\n");
        return;
    }
    if (sm->current_state < 0 || sm->current_state >= STATE_COUNT) {
        printf("Invalid state\n");
        return;
    }
    handlers[sm->current_state](sm, event);
}

Event get_user_event() {
    printf("\nAvailable events:\n");
    printf("0: START\n");
    printf("1: DATA\n");
    printf("2: PROCESS\n");
    printf("3: SUCCESS\n");
    printf("4: FAILURE\n");
    printf("5: RESET\n");
    printf("6: EXIT\n");
    printf("Enter event number: ");
    
    int input;
    if (scanf("%d", &input) != 1) {
        while (getchar() != '\n');
        return EVENT_COUNT;
    }
    
    if (input >= 0 && input <= 5) {
        return (Event)input;
    }
    return EVENT_COUNT;
}

int main() {
    StateMachine sm = {STATE_IDLE, 0, 0};
    
    printf("State Machine Demo\n");
    printf("Current state: IDLE\n");
    
    while (1) {
        Event event = get_user_event();
        if (event == EVENT_COUNT) {
            break;
        }
        
        process_event(&sm, event);
        
        switch (sm.current_state) {
            case STATE_IDLE:
                printf("Current state: IDLE\n");
                break;
            case STATE_READY:
                printf("Current state: READY\n");
                break;
            case STATE_PROCESSING:
                printf("Current state: PROCESSING\n");
                break;
            case STATE_COMPLETE:
                printf("Current state: COMPLETE\n");
                break;
            case STATE_ERROR:
                printf("Current state: ERROR (code: %d)\n", sm.error_code);
                break;
            default:
                break;
        }
    }
    
    printf("Exiting state machine demo\n");
    return 0;
}