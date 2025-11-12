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

void handle_idle(StateMachine *sm, Event event) {
    switch (event) {
        case EVENT_START:
            sm->current_state = STATE_READY;
            sm->data_value = 0;
            sm->error_code = 0;
            printf("Transition: IDLE -> READY\n");
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
            printf("Transition: READY -> PROCESSING\n");
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
            sm->error_code = 0;
            printf("Transition: ERROR -> IDLE\n");
            break;
        default:
            printf("Invalid event for ERROR state\n");
            break;
    }
}

typedef void (*StateHandler)(StateMachine *, Event);

StateHandler handlers[STATE_COUNT] = {
    handle_idle,
    handle_ready,
    handle_processing,
    handle_complete,
    handle_error
};

int process_event(StateMachine *sm, Event event) {
    if (sm == NULL) {
        return -1;
    }
    if (sm->current_state < 0 || sm->current_state >= STATE_COUNT) {
        return -1;
    }
    if (event < 0 || event >= EVENT_COUNT) {
        return -1;
    }
    handlers[sm->current_state](sm, event);
    return 0;
}

Event get_user_event() {
    char input[32];
    printf("Enter event (start, data, process, success, failure, reset, quit): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_COUNT;
    }
    input[strcspn(input, "\n")] = '\0';
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "process") == 0) return EVENT_PROCESS;
    if (strcmp(input, "success") == 0) return EVENT_SUCCESS;
    if (strcmp(input, "failure") == 0) return EVENT_FAILURE;
    if (strcmp(input, "reset") == 0) return EVENT_RESET;
    if (strcmp(input, "quit") == 0) return EVENT_COUNT;
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
        if (process_event(&sm, event) != 0) {
            printf("Error processing event\n");
            break;
        }
        printf("Current state: ");
        switch (sm.current_state) {
            case STATE_IDLE: printf("IDLE"); break;
            case STATE_READY: printf("READY"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_COMPLETE: printf("COMPLETE"); break;
            case STATE_ERROR: printf("ERROR"); break;
            default: printf("UNKNOWN"); break;
        }
        printf("\n");
    }
    
    printf("Final state: ");
    switch (sm.current_state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READY: printf