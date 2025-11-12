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
    if (sm == NULL) return;
    
    switch (event) {
        case EVENT_START:
            sm->current_state = STATE_READY;
            sm->data_value = 0;
            sm->error_code = 0;
            break;
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            sm->data_value = 0;
            sm->error_code = 0;
            break;
        default:
            break;
    }
}

void handle_ready(StateMachine* sm, Event event) {
    if (sm == NULL) return;
    
    switch (event) {
        case EVENT_DATA:
            sm->data_value = 42;
            sm->current_state = STATE_READY;
            break;
        case EVENT_PROCESS:
            if (sm->data_value > 0) {
                sm->current_state = STATE_PROCESSING;
            } else {
                sm->current_state = STATE_ERROR;
                sm->error_code = 1;
            }
            break;
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            sm->data_value = 0;
            sm->error_code = 0;
            break;
        default:
            break;
    }
}

void handle_processing(StateMachine* sm, Event event) {
    if (sm == NULL) return;
    
    switch (event) {
        case EVENT_SUCCESS:
            if (sm->data_value > 0) {
                sm->current_state = STATE_COMPLETE;
            } else {
                sm->current_state = STATE_ERROR;
                sm->error_code = 2;
            }
            break;
        case EVENT_FAILURE:
            sm->current_state = STATE_ERROR;
            sm->error_code = 3;
            break;
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            sm->data_value = 0;
            sm->error_code = 0;
            break;
        default:
            break;
    }
}

void handle_complete(StateMachine* sm, Event event) {
    if (sm == NULL) return;
    
    switch (event) {
        case EVENT_RESET:
            sm->current_state = STATE_IDLE;
            sm->data_value = 0;
            sm->error_code = 0;
            break;
        default:
            break;
    }
}

void handle_error(StateMachine* sm, Event event) {
    if (sm == NULL) return;
    
    switch (event) {
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
    if (sm == NULL || sm->current_state >= STATE_COUNT || event >= EVENT_COUNT) {
        return;
    }
    
    handlers[sm->current_state](sm, event);
}

int main() {
    StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.data_value = 0;
    sm.error_code = 0;
    
    printf("State Machine Demo\n");
    printf("Commands: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET, 6=QUIT\n");
    
    int running = 1;
    while (running) {
        printf("\nCurrent state: %s", state_names[sm.current_state]);
        printf(" | Data: %d", sm.data_value);
        if (sm.current_state == STATE_ERROR) {
            printf(" | Error: %d", sm.error_code);
        }
        printf("\nEnter command: ");
        
        int input;
        if (scanf("%d", &input) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        if (input < 0 || input > 6) {
            printf("Invalid command. Use 0-6.\n");
            continue;
        }
        
        if (input == 6) {