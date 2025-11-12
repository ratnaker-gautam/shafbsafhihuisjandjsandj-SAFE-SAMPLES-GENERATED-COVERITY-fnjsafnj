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

State (*state_handlers[])(StateMachine *, Event) = {
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
        if (sm->buffer_len < MAX_INPUT_LEN - 1) {
            printf("Enter data to process: ");
            if (fgets(sm->buffer, MAX_INPUT_LEN, stdin) != NULL) {
                sm->buffer_len = strlen(sm->buffer);
                if (sm->buffer_len > 0 && sm->buffer[sm->buffer_len - 1] == '\n') {
                    sm->buffer[sm->buffer_len - 1] = '\0';
                    sm->buffer_len--;
                }
                if (sm->buffer_len > 0) {
                    if (strcmp(sm->buffer, "quit") == 0) {
                        return STATE_DONE;
                    }
                    return STATE_PROCESSING;
                }
            }
        }
        return STATE_ERROR;
    }
    return STATE_READING;
}

State handle_processing(StateMachine *sm, Event event) {
    if (event == EVENT_PROCESS_COMPLETE) {
        if (sm->buffer_len > 0) {
            for (size_t i = 0; i < sm->buffer_len; i++) {
                if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                    sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                }
            }
            sm->data_processed = 1;
            return STATE_WRITING;
        }
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

State handle_writing(StateMachine *sm, Event event) {
    if (event == EVENT_WRITE_COMPLETE) {
        if (sm->data_processed) {
            printf("Processed data: %s\n", sm->buffer);
            return STATE_DONE;
        }
        return STATE_ERROR;
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

Event get_next_event(State current_state) {
    switch (current_state) {
        case STATE_IDLE:
            return EVENT_START;
        case STATE_READING:
            return EVENT_DATA_READY;
        case STATE_PROCESSING:
            return EVENT_PROCESS_COMPLETE;
        case STATE_WRITING:
            return EVENT_WRITE_COMPLETE;
        case STATE_ERROR:
        case STATE_DONE:
            return EVENT_RESET;
        default:
            return EVENT_ERROR;
    }
}

int main(void) {
    StateMachine sm = {STATE_IDLE, {0}, 0, 0};
    int max_iterations = 20;
    int iteration = 0;
    
    printf("State Machine Demo - Data Processor\n");
    printf("Commands: Enter data when prompted, 'quit' to exit\n");
    
    while (iteration < max_iterations) {
        Event event = get_next_event(sm.current_state);
        
        if (event == EVENT_ERROR) {
            printf("Invalid state transition\n");
            break;
        }
        
        State new_state = state_handlers[sm.current_state](&sm, event);
        
        if (new_state >= STATE_IDLE && new_state <= STATE_DONE) {
            sm.current_state = new_state;
        } else {
            printf("Invalid state detected\n");
            break;
        }
        
        if (sm.current_state ==