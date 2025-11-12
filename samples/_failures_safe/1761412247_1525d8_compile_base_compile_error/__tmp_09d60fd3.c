//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_COUNT
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
} Event;

typedef struct {
    State current_state;
    char buffer[256];
    size_t buffer_pos;
    int data_value;
} StateMachine;

typedef State (*StateHandler)(StateMachine*, Event);

State handle_idle(StateMachine* sm, Event evt);
State handle_reading(StateMachine* sm, Event evt);
State handle_processing(StateMachine* sm, Event evt);
State handle_writing(StateMachine* sm, Event evt);
State handle_error(StateMachine* sm, Event evt);

static StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error
};

State handle_idle(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_START:
            sm->buffer_pos = 0;
            sm->data_value = 0;
            printf("State: IDLE -> READING\n");
            return STATE_READING;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_IDLE;
    }
}

State handle_reading(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_DATA_READY:
            if (sm->buffer_pos < sizeof(sm->buffer)) {
                sm->buffer[sm->buffer_pos] = 'A' + (sm->buffer_pos % 26);
                sm->buffer_pos++;
                if (sm->buffer_pos >= 10) {
                    printf("State: READING -> PROCESSING\n");
                    return STATE_PROCESSING;
                }
            }
            return STATE_READING;
        case EVENT_ERROR:
            printf("State: READING -> ERROR\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("State: READING -> IDLE\n");
            return STATE_IDLE;
        default:
            return STATE_READING;
    }
}

State handle_processing(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_PROCESS_COMPLETE:
            sm->data_value = (int)sm->buffer_pos * 10;
            printf("Processed data value: %d\n", sm->data_value);
            printf("State: PROCESSING -> WRITING\n");
            return STATE_WRITING;
        case EVENT_ERROR:
            printf("State: PROCESSING -> ERROR\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("State: PROCESSING -> IDLE\n");
            return STATE_IDLE;
        default:
            return STATE_PROCESSING;
    }
}

State handle_writing(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_WRITE_COMPLETE:
            printf("Written data: %.*s\n", (int)sm->buffer_pos, sm->buffer);
            printf("State: WRITING -> IDLE\n");
            return STATE_IDLE;
        case EVENT_ERROR:
            printf("State: WRITING -> ERROR\n");
            return STATE_ERROR;
        case EVENT_RESET:
            printf("State: WRITING -> IDLE\n");
            return STATE_IDLE;
        default:
            return STATE_WRITING;
    }
}

State handle_error(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_RESET:
            printf("State: ERROR -> IDLE\n");
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

void state_machine_init(StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->buffer_pos = 0;
    sm->data_value = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

void state_machine_dispatch(StateMachine* sm, Event evt) {
    if (sm == NULL || evt < 0 || evt >= EVENT_COUNT) return;
    if (sm->current_state < 0 || sm->current_state >= STATE_COUNT) return;
    
    StateHandler handler = state_handlers[sm->current_state];
    if (handler != NULL) {
        State new_state = handler(sm, evt);
        if (new_state >= 0 && new_state < STATE_COUNT) {
            sm->current_state = new_state;
        }
    }
}

int main() {
    StateMachine sm;
    state_machine_init(&sm);
    
    printf("State Machine Demo\n");
    printf("Available events: 0=START, 1=DATA_READY, 2=PROCESS_COMPLETE, 3=WRITE_COMPLETE, 4=ERROR, 5=RESET\n");
    printf("Enter event numbers (0-5), -1 to exit:\n");
    
    int input;
    while (1