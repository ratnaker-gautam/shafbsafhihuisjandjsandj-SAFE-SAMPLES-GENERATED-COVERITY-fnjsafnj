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
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_processed;
} StateMachine;

State handle_idle(StateMachine *sm, Event event);
State handle_reading(StateMachine *sm, Event event);
State handle_processing(StateMachine *sm, Event event);
State handle_writing(StateMachine *sm, Event event);
State handle_error(StateMachine *sm, Event event);
State handle_done(StateMachine *sm, Event event);

typedef State (*StateHandler)(StateMachine *, Event);

StateHandler handlers[] = {
    handle_idle,
    handle_reading,
    handle_processing,
    handle_writing,
    handle_error,
    handle_done
};

State handle_idle(StateMachine *sm, Event event) {
    if (event == EVENT_START) {
        printf("Starting data processing...\n");
        sm->buffer_len = 0;
        sm->data_processed = 0;
        return STATE_READING;
    }
    return STATE_IDLE;
}

State handle_reading(StateMachine *sm, Event event) {
    if (event == EVENT_DATA_READY) {
        printf("Enter data to process (max %d chars): ", MAX_INPUT_LEN - 1);
        if (fgets(sm->buffer, MAX_INPUT_LEN, stdin) != NULL) {
            sm->buffer_len = strlen(sm->buffer);
            if (sm->buffer_len > 0 && sm->buffer[sm->buffer_len - 1] == '\n') {
                sm->buffer[sm->buffer_len - 1] = '\0';
                sm->buffer_len--;
            }
            if (sm->buffer_len > 0) {
                return STATE_PROCESSING;
            }
        }
        return STATE_ERROR;
    }
    return STATE_READING;
}

State handle_processing(StateMachine *sm, Event event) {
    if (event == EVENT_PROCESS_COMPLETE) {
        for (size_t i = 0; i < sm->buffer_len; i++) {
            if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
            }
        }
        sm->data_processed = 1;
        return STATE_WRITING;
    }
    return STATE_PROCESSING;
}

State handle_writing(StateMachine *sm, Event event) {
    if (event == EVENT_WRITE_COMPLETE) {
        printf("Processed data: %s\n", sm->buffer);
        return STATE_DONE;
    }
    return STATE_WRITING;
}

State handle_error(StateMachine *sm, Event event) {
    if (event == EVENT_RESET) {
        printf("Resetting state machine...\n");
        sm->buffer_len = 0;
        sm->data_processed = 0;
        return STATE_IDLE;
    }
    return STATE_ERROR;
}

State handle_done(StateMachine *sm, Event event) {
    if (event == EVENT_RESET) {
        printf("Resetting state machine...\n");
        sm->buffer_len = 0;
        sm->data_processed = 0;
        return STATE_IDLE;
    }
    return STATE_DONE;
}

Event get_user_event(State current_state) {
    char input[10];
    
    switch (current_state) {
        case STATE_IDLE:
            printf("State: IDLE - Enter 'start' to begin: ");
            break;
        case STATE_READING:
            printf("State: READING - Press enter after input: ");
            return EVENT_DATA_READY;
        case STATE_PROCESSING:
            printf("State: PROCESSING - Press enter to continue: ");
            break;
        case STATE_WRITING:
            printf("State: WRITING - Press enter to continue: ");
            break;
        case STATE_ERROR:
            printf("State: ERROR - Enter 'reset' to continue: ");
            break;
        case STATE_DONE:
            printf("State: DONE - Enter 'reset' to start over: ");
            break;
    }
    
    if (fgets(input, sizeof(input), stdin) != NULL) {
        if (current_state == STATE_IDLE && strncmp(input, "start", 5) == 0) {
            return EVENT_START;
        } else if ((current_state == STATE_ERROR || current_state == STATE_DONE) && 
                   strncmp(input, "reset", 5) == 0) {
            return EVENT_RESET;
        } else if (current_state == STATE_PROCESSING) {
            return EVENT_PROCESS_COMPLETE;
        } else if (current_state == STATE_WRITING) {
            return EVENT_WRITE_COMPLETE;
        }
    }
    
    if (current_state == STATE_ERROR || current_state == STATE_DONE) {
        return EVENT_RESET;
    }
    
    return EVENT_ERROR;
}

void initialize_state_machine