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
    int data_value;
    int process_count;
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

void handle_ready(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_DATA:
            if (sm->data_value < 100) {
                sm->data_value += 10;
                printf("Data updated: %d\n", sm->data_value);
            } else {
                printf("Data limit reached\n");
            }
            break;
        case EVENT_PROCESS:
            if (sm->data_value > 0) {
                sm->current_state = STATE_PROCESSING;
                printf("Transition: READY -> PROCESSING\n");
            } else {
                printf("No data to process\n");
            }
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

void handle_processing(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_SUCCESS:
            sm->process_count++;
            if (sm->process_count >= 3) {
                sm->current_state = STATE_DONE;
                printf("Transition: PROCESSING -> DONE\n");
            } else {
                sm->current_state = STATE_READY;
                printf("Transition: PROCESSING -> READY\n");
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

void handle_done(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            sm->data_value = 0;
            sm->process_count = 0;
            printf("Transition: DONE -> IDLE\n");
            break;
        default:
            printf("Invalid event for DONE state\n");
            break;
    }
}

void handle_error(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            sm->data_value = 0;
            sm->process_count = 0;
            printf("Transition: ERROR -> IDLE\n");
            break;
        default:
            printf("Invalid event for ERROR state\n");
            break;
    }
}

void process_event(StateMachine* sm, Event event) {
    if (sm == NULL) return;
    if (sm->current_state < STATE_IDLE || sm->current_state > STATE_ERROR) return;
    
    handlers[sm->current_state](sm, event);
}

Event get_user_event() {
    printf("\nAvailable events:\n");
    printf("0: START, 1: DATA, 2: PROCESS, 3: SUCCESS, 4: FAILURE, 5: RESET\n");
    printf("Enter event number: ");
    
    int input;
    if (scanf("%d", &input) != 1) {
        while (getchar() != '\n');
        return EVENT_RESET;
    }
    
    if (input < 0 || input > 5) {
        return EVENT_RESET;
    }
    
    return (Event)input;
}

int main() {
    StateMachine sm = {STATE_IDLE, 0, 0};
    
    printf("State Machine Demo Started\n");
    printf("Initial state: IDLE\n");
    
    while (1) {
        printf("\nCurrent state: ");
        switch(sm.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READY: printf("READY (data: %d)", sm.data_value); break;
            case STATE_PROCESSING: printf("PROCESSING (count: %d)", sm.process_count); break;
            case STATE_DONE: printf("DONE"); break;
            case STATE_ERROR: printf("ERROR"); break;
        }
        printf("\n");