//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LENGTH 100
#define MAX_STATES 10

typedef enum {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_WRITE,
    EVENT_ERROR,
    EVENT_COMPLETE,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LENGTH];
    size_t buffer_length;
    int data_processed;
} StateMachine;

State handle_idle_state(StateMachine* sm, Event event, const char* data);
State handle_reading_state(StateMachine* sm, Event event, const char* data);
State handle_processing_state(StateMachine* sm, Event event, const char* data);
State handle_writing_state(StateMachine* sm, Event event, const char* data);
State handle_error_state(StateMachine* sm, Event event, const char* data);
State handle_done_state(StateMachine* sm, Event event, const char* data);

void initialize_state_machine(StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    memset(sm->buffer, 0, MAX_INPUT_LENGTH);
    sm->buffer_length = 0;
    sm->data_processed = 0;
}

State transition_state(StateMachine* sm, Event event, const char* data) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            return handle_idle_state(sm, event, data);
        case STATE_READING:
            return handle_reading_state(sm, event, data);
        case STATE_PROCESSING:
            return handle_processing_state(sm, event, data);
        case STATE_WRITING:
            return handle_writing_state(sm, event, data);
        case STATE_ERROR:
            return handle_error_state(sm, event, data);
        case STATE_DONE:
            return handle_done_state(sm, event, data);
        default:
            return STATE_ERROR;
    }
}

State handle_idle_state(StateMachine* sm, Event event, const char* data) {
    if (event == EVENT_START) {
        printf("State: IDLE -> READING\n");
        return STATE_READING;
    }
    return STATE_IDLE;
}

State handle_reading_state(StateMachine* sm, Event event, const char* data) {
    if (event == EVENT_DATA && data != NULL) {
        size_t data_len = strlen(data);
        if (data_len > 0 && data_len < MAX_INPUT_LENGTH) {
            strncpy(sm->buffer, data, MAX_INPUT_LENGTH - 1);
            sm->buffer[MAX_INPUT_LENGTH - 1] = '\0';
            sm->buffer_length = data_len;
            printf("State: READING -> PROCESSING\n");
            return STATE_PROCESSING;
        } else {
            printf("State: READING -> ERROR\n");
            return STATE_ERROR;
        }
    }
    return STATE_READING;
}

State handle_processing_state(StateMachine* sm, Event event, const char* data) {
    if (event == EVENT_PROCESS) {
        if (sm->buffer_length > 0) {
            for (size_t i = 0; i < sm->buffer_length; i++) {
                if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                    sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                }
            }
            sm->data_processed = 1;
            printf("State: PROCESSING -> WRITING\n");
            return STATE_WRITING;
        } else {
            printf("State: PROCESSING -> ERROR\n");
            return STATE_ERROR;
        }
    }
    return STATE_PROCESSING;
}

State handle_writing_state(StateMachine* sm, Event event, const char* data) {
    if (event == EVENT_WRITE) {
        if (sm->data_processed && sm->buffer_length > 0) {
            printf("Processed data: %s\n", sm->buffer);
            printf("State: WRITING -> DONE\n");
            return STATE_DONE;
        } else {
            printf("State: WRITING -> ERROR\n");
            return STATE_ERROR;
        }
    }
    return STATE_WRITING;
}

State handle_error_state(StateMachine* sm, Event event, const char* data) {
    if (event == EVENT_RESET) {
        printf("State: ERROR -> IDLE\n");
        initialize_state_machine(sm);
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

State handle_done_state(StateMachine* sm, Event event, const char* data) {
    if (event == EVENT_RESET) {
        printf("State: DONE -> IDLE\n");
        initialize_state_machine(sm);
        return STATE_IDLE;
    }
    return STATE_DONE;
}

Event get_user_event() {
    printf("\nAvailable events:\n");
    printf("0: START, 1: DATA, 2: PROCESS, 3: WRITE, 4: ERROR, 5: COMPLETE, 6: RESET\n");
    printf("Enter event number: ");
    
    int input;
    if (scanf("%d