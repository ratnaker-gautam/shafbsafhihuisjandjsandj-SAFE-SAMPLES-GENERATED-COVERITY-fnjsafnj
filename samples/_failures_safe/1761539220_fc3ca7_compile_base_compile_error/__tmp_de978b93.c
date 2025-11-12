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

const char* state_names[STATE_COUNT] = {
    "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
};

const char* event_names[EVENT_COUNT] = {
    "START", "DATA", "PROCESS", "SUCCESS", "FAILURE", "RESET"
};

void handle_idle(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_START:
            sm->current_state = STATE_READY;
            sm->data_value = 0;
            break;
        case EVENT_RESET:
            break;
        default:
            sm->current_state = STATE_ERROR;
            sm->error_code = 1;
            break;
    }
}

void handle_ready(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_DATA:
            sm->data_value = 42;
            break;
        case EVENT_PROCESS:
            if(sm->data_value != 0) {
                sm->current_state = STATE_PROCESSING;
            } else {
                sm->current_state = STATE_ERROR;
                sm->error_code = 2;
            }
            break;
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            sm->data_value = 0;
            break;
        default:
            sm->current_state = STATE_ERROR;
            sm->error_code = 3;
            break;
    }
}

void handle_processing(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_SUCCESS:
            sm->current_state = STATE_COMPLETE;
            break;
        case EVENT_FAILURE:
            sm->current_state = STATE_ERROR;
            sm->error_code = 4;
            break;
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            sm->data_value = 0;
            break;
        default:
            sm->current_state = STATE_ERROR;
            sm->error_code = 5;
            break;
    }
}

void handle_complete(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            sm->data_value = 0;
            break;
        default:
            sm->current_state = STATE_ERROR;
            sm->error_code = 6;
            break;
    }
}

void handle_error(StateMachine* sm, Event event) {
    switch(event) {
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            sm->data_value = 0;
            sm->error_code = 0;
            break;
        default:
            break;
    }
}

void process_event(StateMachine* sm, Event event) {
    if(sm == NULL || event < 0 || event >= EVENT_COUNT) {
        return;
    }
    if(sm->current_state < 0 || sm->current_state >= STATE_COUNT) {
        return;
    }
    handlers[sm->current_state](sm, event);
}

int main() {
    StateMachine sm = {STATE_IDLE, 0, 0};
    char input[32];
    int event_choice;
    
    printf("State Machine Demo\n");
    printf("Available events:\n");
    for(int i = 0; i < EVENT_COUNT; i++) {
        printf("%d: %s\n", i, event_names[i]);
    }
    printf("Enter event number (0-%d) or -1 to exit:\n", EVENT_COUNT - 1);
    
    while(1) {
        printf("\nCurrent state: %s", state_names[sm.current_state]);
        if(sm.current_state == STATE_ERROR) {
            printf(" (Error code: %d)", sm.error_code);
        }
        if(sm.current_state == STATE_READY || sm.current_state == STATE_COMPLETE) {
            printf(" (Data: %d)", sm.data_value);
        }
        printf("\nEnter event: ");
        
        if(fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if(sscanf(input, "%d", &event_choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if(event_choice == -1) {
            break;
        }
        
        if(event_choice < 0 || event_choice >= EVENT_COUNT) {
            printf("Invalid event. Please enter 0-%d or -1 to exit.\n", EVENT_COUNT - 1);
            continue;
        }