//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LENGTH 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
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
    char buffer[MAX_INPUT_LENGTH];
    size_t data_length;
    int error_code;
} StateMachine;

State handle_start_state(StateMachine* sm, Event event);
State handle_reading_state(StateMachine* sm, Event event);
State handle_processing_state(StateMachine* sm, Event event);
State handle_writing_state(StateMachine* sm, Event event);
State handle_error_state(StateMachine* sm, Event event);
State handle_done_state(StateMachine* sm, Event event);

void initialize_state_machine(StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->data_length = 0;
    sm->error_code = 0;
}

State transition_state(StateMachine* sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_START:
            return handle_start_state(sm, event);
        case STATE_READING:
            return handle_reading_state(sm, event);
        case STATE_PROCESSING:
            return handle_processing_state(sm, event);
        case STATE_WRITING:
            return handle_writing_state(sm, event);
        case STATE_ERROR:
            return handle_error_state(sm, event);
        case STATE_DONE:
            return handle_done_state(sm, event);
        default:
            return STATE_ERROR;
    }
}

State handle_start_state(StateMachine* sm, Event event) {
    if (event == EVENT_START) {
        printf("State machine started. Enter data: ");
        return STATE_READING;
    }
    return STATE_START;
}

State handle_reading_state(StateMachine* sm, Event event) {
    if (event == EVENT_DATA_READY) {
        if (fgets(sm->buffer, sizeof(sm->buffer), stdin) != NULL) {
            sm->data_length = strnlen(sm->buffer, sizeof(sm->buffer));
            if (sm->data_length > 0 && sm->buffer[sm->data_length - 1] == '\n') {
                sm->buffer[sm->data_length - 1] = '\0';
                sm->data_length--;
            }
            if (sm->data_length > 0) {
                printf("Data received: %s\n", sm->buffer);
                return STATE_PROCESSING;
            }
        }
        sm->error_code = 1;
        return STATE_ERROR;
    }
    return STATE_READING;
}

State handle_processing_state(StateMachine* sm, Event event) {
    if (event == EVENT_PROCESS_COMPLETE) {
        if (sm->data_length > 0 && sm->data_length < sizeof(sm->buffer)) {
            for (size_t i = 0; i < sm->data_length; i++) {
                if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                    sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                }
            }
            printf("Processed data: %s\n", sm->buffer);
            return STATE_WRITING;
        }
        sm->error_code = 2;
        return STATE_ERROR;
    }
    return STATE_PROCESSING;
}

State handle_writing_state(StateMachine* sm, Event event) {
    if (event == EVENT_WRITE_COMPLETE) {
        printf("Data written successfully.\n");
        return STATE_DONE;
    }
    return STATE_WRITING;
}

State handle_error_state(StateMachine* sm, Event event) {
    if (event == EVENT_RESET) {
        printf("Resetting state machine.\n");
        initialize_state_machine(sm);
        return STATE_START;
    }
    return STATE_ERROR;
}

State handle_done_state(StateMachine* sm, Event event) {
    if (event == EVENT_RESET) {
        printf("Resetting state machine.\n");
        initialize_state_machine(sm);
        return STATE_START;
    }
    return STATE_DONE;
}

int main(void) {
    StateMachine sm;
    initialize_state_machine(&sm);
    
    printf("State Machine Demo - Text Processor\n");
    printf("Commands: start, process, write, reset, quit\n");
    
    char input[MAX_INPUT_LENGTH];
    int running = 1;
    
    while (running) {
        printf("\nCurrent state: ");
        switch (sm.current_state) {
            case STATE_START: printf("START"); break;
            case STATE_READING: printf("READING"); break;
            case STATE_PROCESSING: printf("PROCESSING"); break;
            case STATE_WRITING: printf("WRITING"); break;
            case STATE_ERROR: printf("ERROR"); break;
            case STATE_DONE: printf("DONE"); break;
        }
        printf("\nEnter command: ");
        
        if