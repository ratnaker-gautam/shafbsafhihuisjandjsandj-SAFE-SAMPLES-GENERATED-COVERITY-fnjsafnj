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

Event get_user_event(void) {
    printf("\nAvailable events:\n");
    printf("0: START, 1: DATA_READY, 2: PROCESS_COMPLETE\n");
    printf("3: WRITE_COMPLETE, 4: ERROR, 5: RESET\n");
    printf("Choose event (0-5): ");
    
    char input[10];
    if (fgets(input, sizeof(input), stdin) != NULL) {
        char *endptr;
        long choice = strtol(input, &endptr, 10);
        if (endptr != input && choice >= 0 && choice <= 5) {
            return (Event)choice;
        }
    }
    printf("Invalid input. Using EVENT_ERROR.\n");
    return EVENT_ERROR;
}

void run_state_machine(StateMachine *sm) {
    State current_state = STATE_IDLE;
    Event event;
    
    while (1) {
        if (current_state >= 0 && current_state < (State)(sizeof(handlers)/sizeof(handlers[0]))) {
            event = get_user_event();
            State new_state = handlers[current_state](sm, event);
            
            if (new_state != current_state) {
                printf("State transition: %d -> %d\n", current_state, new_state);
                current_state = new_state;
            }
            
            if (current_state == STATE_DONE || current_state == STATE_ERROR) {
                printf("Final state reached: %d\n", current_state);
                break;
            }
        } else {
            printf("Invalid state detected. Terminating.\n");
            break;
        }
    }
}

int main(void) {
    StateMachine sm = {
        .