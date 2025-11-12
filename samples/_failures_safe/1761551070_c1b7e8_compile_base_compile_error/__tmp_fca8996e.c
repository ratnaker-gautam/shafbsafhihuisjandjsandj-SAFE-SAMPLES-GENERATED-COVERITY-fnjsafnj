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
    EVENT_PROCESS_DONE,
    EVENT_WRITE_DONE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
} Event;

typedef struct {
    State current_state;
    char buffer[256];
    size_t buffer_len;
    int data_value;
} StateMachine;

typedef State (*StateHandler)(StateMachine*, Event);

State handle_idle(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_START:
            printf("Starting data processing\n");
            sm->buffer_len = 0;
            sm->data_value = 0;
            return STATE_READING;
        case EVENT_RESET:
            printf("Resetting to idle\n");
            return STATE_IDLE;
        default:
            return STATE_IDLE;
    }
}

State handle_reading(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_DATA_READY:
            if (sm->buffer_len >= sizeof(sm->buffer)) {
                return STATE_ERROR;
            }
            printf("Enter data value (0-100): ");
            if (scanf("%d", &sm->data_value) != 1) {
                return STATE_ERROR;
            }
            if (sm->data_value < 0 || sm->data_value > 100) {
                return STATE_ERROR;
            }
            printf("Read value: %d\n", sm->data_value);
            return STATE_PROCESSING;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_READING;
    }
}

State handle_processing(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_PROCESS_DONE:
            printf("Processing data: %d -> %d\n", sm->data_value, sm->data_value * 2);
            sm->data_value *= 2;
            if (sm->data_value > 200) {
                return STATE_ERROR;
            }
            return STATE_WRITING;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_PROCESSING;
    }
}

State handle_writing(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_WRITE_DONE:
            printf("Writing result: %d\n", sm->data_value);
            return STATE_IDLE;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_WRITING;
    }
}

State handle_error(StateMachine* sm, Event evt) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (evt) {
        case EVENT_RESET:
            printf("Recovering from error state\n");
            return STATE_IDLE;
        default:
            printf("In error state - need reset\n");
            return STATE_ERROR;
    }
}

StateHandler state_handlers[STATE_COUNT] = {
    handle_idle,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error
};

void initialize_state_machine(StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->buffer_len = 0;
    sm->data_value = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int process_event(StateMachine* sm, Event evt) {
    if (sm == NULL || evt < 0 || evt >= EVENT_COUNT) {
        return 0;
    }
    if (sm->current_state < 0 || sm->current_state >= STATE_COUNT) {
        return 0;
    }
    
    StateHandler handler = state_handlers[sm->current_state];
    if (handler == NULL) {
        return 0;
    }
    
    State new_state = handler(sm, evt);
    if (new_state < 0 || new_state >= STATE_COUNT) {
        return 0;
    }
    
    sm->current_state = new_state;
    return 1;
}

int main() {
    StateMachine sm;
    initialize_state_machine(&sm);
    
    printf("State Machine Demo - Enter commands:\n");
    printf("0: START, 1: DATA_READY, 2: PROCESS_DONE, 3: WRITE_DONE\n");
    printf("4: ERROR, 5: RESET, 6: EXIT\n");
    
    int running = 1;
    while (running) {
        printf("Current state: %d | Enter event: ", sm.current_state);
        
        int input;
        if (scanf("%d", &input) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (input < 0 || input >= EVENT_COUNT) {
            if (input == 6) {
                running = 0;
                continue;
            }