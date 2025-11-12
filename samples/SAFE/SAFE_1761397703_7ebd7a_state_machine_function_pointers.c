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

StateMachine* create_state_machine(void) {
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (sm == NULL) {
        return NULL;
    }
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->error_code = 0;
    return sm;
}

void destroy_state_machine(StateMachine* sm) {
    free(sm);
}

State process_event(StateMachine* sm, Event event) {
    static StateHandler handlers[STATE_COUNT] = {
        handle_idle,
        handle_ready,
        handle_processing,
        handle_complete,
        handle_error
    };

    if (sm == NULL || event < 0 || event >= EVENT_COUNT) {
        return STATE_ERROR;
    }

    StateHandler handler = handlers[sm->current_state];
    if (handler == NULL) {
        return STATE_ERROR;
    }

    State new_state = handler(sm, event);
    if (new_state < 0 || new_state >= STATE_COUNT) {
        return STATE_ERROR;
    }

    sm->current_state = new_state;
    return new_state;
}

void print_state_info(StateMachine* sm) {
    const char* state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };

    if (sm == NULL || sm->current_state < 0 || sm->current_state >= STATE_COUNT) {
        printf("Invalid state machine\n");
        return;
    }

    printf("Current state: %s", state_names[sm->current_state]);
    printf(" | Data: %d", sm->data_value);
    if (sm->error_code != 0) {
        printf(" | Error: %d", sm->error_code);
    }
    printf("\n");
}

int main(void) {
    StateMachine* sm = create_state_machine();
    if (sm == NULL) {
        fprintf(stderr, "Failed to create state machine\n");
        return 1;
    }

    printf("State Machine Demo\n");
    printf("Available events: 0=START, 1=DATA, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET, 6=QUIT\n");

    int running = 1;
    while (running) {
        print_state_info(sm);
        printf("Enter event number: ");

        int input;
        if (scanf("%d", &input) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }

        if (input < 0 || input > 6) {
            printf("Invalid event number\n");
            continue;
        }

        if (input == 6) {
            running = 0;
            continue;
        }

        Event event = (Event)input;
        State new_state = process_event(sm, event);
        if (new_state == STATE_ERROR) {
            printf("Event processing failed\n");
        }
    }

    destroy_state_machine(sm);
    return 0;
}