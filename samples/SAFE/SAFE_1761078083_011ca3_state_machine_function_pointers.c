//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: function_pointers ; Variation: state_machine
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

typedef State (*StateHandler)(StateMachine*, Event);

State handle_idle(StateMachine* sm, Event event) {
    switch (event) {
        case EVENT_START:
            return STATE_READY;
        default:
            return STATE_IDLE;
    }
}

State handle_ready(StateMachine* sm, Event event) {
    switch (event) {
        case EVENT_DATA:
            return STATE_PROCESSING;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_READY;
    }
}

State handle_processing(StateMachine* sm, Event event) {
    switch (event) {
        case EVENT_SUCCESS:
            return STATE_COMPLETE;
        case EVENT_FAILURE:
            sm->error_code = 1;
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_PROCESSING;
    }
}

State handle_complete(StateMachine* sm, Event event) {
    switch (event) {
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_COMPLETE;
    }
}

State handle_error(StateMachine* sm, Event event) {
    switch (event) {
        case EVENT_RESET:
            sm->error_code = 0;
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

void initialize_state_machine(StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->error_code = 0;
}

int process_event(StateMachine* sm, Event event) {
    if (sm == NULL) return -1;
    if (sm->current_state >= STATE_COUNT) return -1;
    if (event >= EVENT_COUNT) return -1;
    
    StateHandler handler = state_handlers[sm->current_state];
    if (handler == NULL) return -1;
    
    State new_state = handler(sm, event);
    if (new_state >= STATE_COUNT) return -1;
    
    sm->current_state = new_state;
    return 0;
}

const char* state_to_string(State state) {
    static const char* names[] = {
        "IDLE",
        "READY",
        "PROCESSING",
        "COMPLETE",
        "ERROR"
    };
    if (state >= STATE_COUNT) return "UNKNOWN";
    return names[state];
}

const char* event_to_string(Event event) {
    static const char* names[] = {
        "START",
        "DATA",
        "PROCESS",
        "SUCCESS",
        "FAILURE",
        "RESET"
    };
    if (event >= EVENT_COUNT) return "UNKNOWN";
    return names[event];
}

int main() {
    StateMachine sm;
    initialize_state_machine(&sm);
    
    printf("State Machine Demo\n");
    printf("Commands: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET, 6=QUIT\n");
    
    int running = 1;
    while (running) {
        printf("\nCurrent state: %s", state_to_string(sm.current_state));
        if (sm.current_state == STATE_ERROR) {
            printf(" (Error code: %d)", sm.error_code);
        }
        printf("\nEnter command: ");
        
        int input;
        if (scanf("%d", &input) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (input < 0 || input > 6) {
            printf("Invalid command\n");
            continue;
        }
        
        if (input == 6) {
            running = 0;
            continue;
        }
        
        Event event = (Event)input;
        printf("Processing event: %s\n", event_to_string(event));
        
        if (process_event(&sm, event) != 0) {
            printf("Error processing event\n");
        }
    }
    
    printf("Goodbye!\n");
    return 0;
}