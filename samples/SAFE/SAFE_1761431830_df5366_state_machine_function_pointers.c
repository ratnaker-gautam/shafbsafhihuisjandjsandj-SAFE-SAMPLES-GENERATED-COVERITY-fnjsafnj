//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_DONE,
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
    char buffer[256];
    size_t data_length;
} StateMachine;

typedef void (*StateHandler)(StateMachine*, Event);

void handle_idle(StateMachine* sm, Event event);
void handle_ready(StateMachine* sm, Event event);
void handle_processing(StateMachine* sm, Event event);
void handle_done(StateMachine* sm, Event event);
void handle_error(StateMachine* sm, Event event);

StateHandler handlers[] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_done,
    handle_error
};

void handle_idle(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_START:
            printf("Transition: IDLE -> READY\n");
            sm->current_state = STATE_READY;
            sm->data_length = 0;
            memset(sm->buffer, 0, sizeof(sm->buffer));
            break;
        case EVENT_RESET:
            printf("Already in IDLE state\n");
            break;
        default:
            printf("Invalid event for IDLE state\n");
            sm->current_state = STATE_ERROR;
            break;
    }
}

void handle_ready(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_DATA:
            if(sm->data_length < sizeof(sm->buffer) - 1) {
                printf("Data received, transitioning to PROCESSING\n");
                sm->current_state = STATE_PROCESSING;
            } else {
                printf("Buffer full, transitioning to ERROR\n");
                sm->current_state = STATE_ERROR;
            }
            break;
        case EVENT_RESET:
            printf("Transition: READY -> IDLE\n");
            sm->current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for READY state\n");
            sm->current_state = STATE_ERROR;
            break;
    }
}

void handle_processing(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_SUCCESS:
            printf("Processing successful, transitioning to DONE\n");
            sm->current_state = STATE_DONE;
            break;
        case EVENT_FAILURE:
            printf("Processing failed, transitioning to ERROR\n");
            sm->current_state = STATE_ERROR;
            break;
        case EVENT_RESET:
            printf("Transition: PROCESSING -> IDLE\n");
            sm->current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for PROCESSING state\n");
            sm->current_state = STATE_ERROR;
            break;
    }
}

void handle_done(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_RESET:
            printf("Transition: DONE -> IDLE\n");
            sm->current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for DONE state\n");
            sm->current_state = STATE_ERROR;
            break;
    }
}

void handle_error(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_RESET:
            printf("Transition: ERROR -> IDLE\n");
            sm->current_state = STATE_IDLE;
            break;
        default:
            printf("Invalid event for ERROR state\n");
            break;
    }
}

void process_event(StateMachine* sm, Event event) {
    if(sm == NULL) return;
    if(sm->current_state < STATE_IDLE || sm->current_state > STATE_ERROR) {
        sm->current_state = STATE_ERROR;
        return;
    }
    handlers[sm->current_state](sm, event);
}

Event get_user_event() {
    printf("\nAvailable events:\n");
    printf("0: START, 1: DATA, 2: PROCESS, 3: SUCCESS, 4: FAILURE, 5: RESET\n");
    printf("Enter event number (0-5): ");
    
    int input;
    if(scanf("%d", &input) != 1) {
        while(getchar() != '\n');
        return EVENT_RESET;
    }
    
    if(input < 0 || input > 5) {
        return EVENT_RESET;
    }
    
    return (Event)input;
}

const char* state_to_string(State state) {
    switch(state) {
        case STATE_IDLE: return "IDLE";
        case STATE_READY: return "READY";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_DONE: return "DONE";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

int main() {
    StateMachine sm = {STATE_IDLE, "", 0};
    
    printf("State Machine Demo - Function Pointers\n");
    printf("Initial state: %s\n", state_to_string(sm.current_state));
    
    for(int i = 0; i < 10; i++) {
        Event event = get_user_event();
        process_event(&sm, event);
        printf("Current state: %s\n", state_to_string(sm.current_state));
        
        if(sm.current_state == STATE_DONE) {
            printf("Process completed successfully!\n");
            break;
        }
    }
    
    return 0;
}