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
            sm->data_value = 0;
            sm->error_code = 0;
            return STATE_READY;
        default:
            return STATE_IDLE;
    }
}

State handle_ready(StateMachine* sm, Event event) {
    switch (event) {
        case EVENT_DATA:
            sm->data_value = 42;
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
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

static StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

void process_event(StateMachine* sm, Event event) {
    if (sm == NULL || event < 0 || event >= EVENT_COUNT) {
        return;
    }
    State new_state = state_handlers[sm->current_state](sm, event);
    if (new_state >= 0 && new_state < STATE_COUNT) {
        sm->current_state = new_state;
    }
}

const char* state_name(State state) {
    static const char* names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    if (state >= 0 && state < STATE_COUNT) {
        return names[state];
    }
    return "UNKNOWN";
}

const char* event_name(Event event) {
    static const char* names[] = {
        "START", "DATA", "PROCESS", "SUCCESS", "FAILURE", "RESET"
    };
    if (event >= 0 && event < EVENT_COUNT) {
        return names[event];
    }
    return "UNKNOWN";
}

int main(void) {
    StateMachine sm = {STATE_IDLE, 0, 0};
    char input[32];
    Event event;
    
    printf("State Machine Demo\n");
    printf("Available events: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET\n");
    printf("Enter event number or 'q' to quit\n");
    
    while (1) {
        printf("\nCurrent state: %s", state_name(sm.current_state));
        if (sm.current_state == STATE_COMPLETE) {
            printf(" (data: %d)", sm.data_value);
        } else if (sm.current_state == STATE_ERROR) {
            printf(" (error: %d)", sm.error_code);
        }
        printf("\nEnter event: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (input[0] == 'q' || input[0] == 'Q') {
            break;
        }
        
        char* endptr;
        long value = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\n') {
            printf("Invalid input. Please enter a number or 'q'.\n");
            continue;
        }
        
        if (value < 0 || value >= EVENT_COUNT) {
            printf("Event number must be between 0 and %d.\n", EVENT_COUNT - 1);
            continue;
        }
        
        event = (Event)value;
        process_event(&sm, event);
    }
    
    return 0;
}