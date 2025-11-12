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

typedef void (*StateHandler)(StateMachine*, Event);

void handle_idle(StateMachine* sm, Event event);
void handle_ready(StateMachine* sm, Event event);
void handle_processing(StateMachine* sm, Event event);
void handle_complete(StateMachine* sm, Event event);
void handle_error(StateMachine* sm, Event event);

StateHandler handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

void handle_idle(StateMachine* sm, Event event) {
    if (sm == NULL) return;
    
    switch (event) {
        case EVENT_START:
            sm->current_state = STATE_READY;
            sm->data_value = 0;
            sm->process_count = 0;
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

void handle_ready(StateMachine* sm, Event event) {
    if (sm == NULL) return;
    
    switch (event) {
        case EVENT_DATA:
            sm->data_value = 42;
            sm->current_state = STATE_PROCESSING;
            printf("Transition: READY -> PROCESSING (data: %d)\n", sm->data_value);
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

void handle_processing(StateMachine* sm, Event event) {
    if (sm == NULL) return;
    
    switch (event) {
        case EVENT_PROCESS:
            if (sm->process_count < 3) {
                sm->data_value += 10;
                sm->process_count++;
                printf("Processing data: %d (count: %d)\n", sm->data_value, sm->process_count);
            }
            if (sm->process_count >= 3) {
                sm->current_state = STATE_COMPLETE;
                printf("Transition: PROCESSING -> COMPLETE\n");
            }
            break;
        case EVENT_FAILURE:
            sm->current_state = STATE_ERROR;
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

void handle_complete(StateMachine* sm, Event event) {
    if (sm == NULL) return;
    
    switch (event) {
        case EVENT_SUCCESS:
            printf("Process completed successfully. Final data: %d\n", sm->data_value);
            break;
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            printf("Reset to IDLE state\n");
            break;
        default:
            printf("Invalid event for COMPLETE state\n");
            break;
    }
}

void handle_error(StateMachine* sm, Event event) {
    if (sm == NULL) return;
    
    switch (event) {
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            printf("Reset to IDLE state\n");
            break;
        default:
            printf("Invalid event for ERROR state\n");
            break;
    }
}

void process_event(StateMachine* sm, Event event) {
    if (sm == NULL || event < 0 || event >= EVENT_COUNT) {
        printf("Invalid state machine or event\n");
        return;
    }
    
    if (sm->current_state < 0 || sm->current_state >= STATE_COUNT) {
        printf("Invalid state\n");
        return;
    }
    
    handlers[sm->current_state](sm, event);
}

int main() {
    StateMachine sm = {STATE_IDLE, 0, 0};
    char input[32];
    int choice;
    
    printf("State Machine Demo\n");
    printf("Available events:\n");
    printf("0: START, 1: DATA, 2: PROCESS, 3: SUCCESS, 4: FAILURE, 5: RESET, 6: EXIT\n");
    
    while (1) {
        printf("\nCurrent state: ");
        switch (sm.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READY: printf("READY"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_COMPLETE: printf("COMPLETE"); break;
            case STATE_ERROR: printf("ERROR"); break;
            default: printf("UNKNOWN"); break;
        }
        printf("\nEnter event number: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;