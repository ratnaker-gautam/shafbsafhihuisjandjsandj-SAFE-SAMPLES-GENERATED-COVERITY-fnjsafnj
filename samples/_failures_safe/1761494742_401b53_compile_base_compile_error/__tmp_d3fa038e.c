//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
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
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_processed;
} StateMachine;

State handle_idle(StateMachine *sm, Event event, const char *data);
State handle_reading(StateMachine *sm, Event event, const char *data);
State handle_processing(StateMachine *sm, Event event, const char *data);
State handle_writing(StateMachine *sm, Event event, const char *data);
State handle_error(StateMachine *sm, Event event, const char *data);
State handle_done(StateMachine *sm, Event event, const char *data);

void initialize_machine(StateMachine *sm) {
    sm->current_state = STATE_IDLE;
    memset(sm->buffer, 0, MAX_INPUT_LEN);
    sm->buffer_len = 0;
    sm->data_processed = 0;
}

State handle_idle(StateMachine *sm, Event event, const char *data) {
    if (event == EVENT_START) {
        printf("Starting data processing\n");
        return STATE_READING;
    }
    return STATE_IDLE;
}

State handle_reading(StateMachine *sm, Event event, const char *data) {
    if (event == EVENT_DATA && data != NULL) {
        size_t data_len = strlen(data);
        if (data_len < MAX_INPUT_LEN - sm->buffer_len) {
            strncpy(sm->buffer + sm->buffer_len, data, data_len);
            sm->buffer_len += data_len;
            sm->buffer[sm->buffer_len] = '\0';
            printf("Read %zu bytes, total: %zu\n", data_len, sm->buffer_len);
            return STATE_READING;
        } else {
            printf("Buffer full, moving to processing\n");
            return STATE_PROCESSING;
        }
    } else if (event == EVENT_PROCESS) {
        return STATE_PROCESSING;
    } else if (event == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_READING;
}

State handle_processing(StateMachine *sm, Event event, const char *data) {
    if (event == EVENT_PROCESS && sm->buffer_len > 0) {
        for (size_t i = 0; i < sm->buffer_len; i++) {
            if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
            }
        }
        sm->data_processed = 1;
        printf("Data processed: %s\n", sm->buffer);
        return STATE_WRITING;
    } else if (event == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

State handle_writing(StateMachine *sm, Event event, const char *data) {
    if (event == EVENT_WRITE && sm->data_processed) {
        printf("Writing data: %s\n", sm->buffer);
        return STATE_DONE;
    } else if (event == EVENT_ERROR) {
        return STATE_ERROR;
    }
    return STATE_WRITING;
}

State handle_error(StateMachine *sm, Event event, const char *data) {
    if (event == EVENT_RESET) {
        printf("Resetting state machine\n");
        initialize_machine(sm);
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

State handle_done(StateMachine *sm, Event event, const char *data) {
    if (event == EVENT_RESET) {
        printf("Resetting state machine\n");
        initialize_machine(sm);
        return STATE_IDLE;
    }
    return STATE_DONE;
}

State process_event(StateMachine *sm, Event event, const char *data) {
    switch (sm->current_state) {
        case STATE_IDLE:
            return handle_idle(sm, event, data);
        case STATE_READING:
            return handle_reading(sm, event, data);
        case STATE_PROCESSING:
            return handle_processing(sm, event, data);
        case STATE_WRITING:
            return handle_writing(sm, event, data);
        case STATE_ERROR:
            return handle_error(sm, event, data);
        case STATE_DONE:
            return handle_done(sm, event, data);
        default:
            return STATE_ERROR;
    }
}

Event get_user_event() {
    printf("Available events:\n");
    printf("0: START, 1: DATA, 2: PROCESS, 3: WRITE, 4: ERROR, 5: COMPLETE, 6: RESET\n");
    printf("Enter event number: ");
    
    int input;
    if (scanf("%d", &input) != 1) {
        while (getchar() != '\n');
        return EVENT_ERROR;
    }
    
    if (input >= 0 && input <= 6) {
        return (