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
    int data_count;
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
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("Starting data processing...\n");
            sm->buffer_len = 0;
            sm->data_count = 0;
            return STATE_READING;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_IDLE;
    }
}

State handle_reading(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (sm->buffer_len < MAX_INPUT_LEN - 1) {
                printf("Enter data (or 'done' to finish): ");
                if (fgets(sm->buffer + sm->buffer_len, MAX_INPUT_LEN - sm->buffer_len, stdin) != NULL) {
                    size_t len = strlen(sm->buffer + sm->buffer_len);
                    if (len > 0 && sm->buffer[sm->buffer_len + len - 1] == '\n') {
                        sm->buffer[sm->buffer_len + len - 1] = '\0';
                        len--;
                    }
                    sm->buffer_len += len;
                    sm->data_count++;
                    
                    if (strcmp(sm->buffer + sm->buffer_len - len, "done") == 0) {
                        sm->buffer_len -= (len + 1);
                        return STATE_PROCESSING;
                    }
                    
                    if (sm->buffer_len < MAX_INPUT_LEN - 1) {
                        sm->buffer[sm->buffer_len++] = ' ';
                    }
                }
            }
            return STATE_READING;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_READING;
    }
}

State handle_processing(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS:
            printf("Processing %d data items...\n", sm->data_count);
            if (sm->buffer_len > 0) {
                for (size_t i = 0; i < sm->buffer_len; i++) {
                    if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                        sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
                    }
                }
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

State handle_writing(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_WRITE:
            printf("Processed data: %s\n", sm->buffer_len > 0 ? sm->buffer : "(empty)");
            return STATE_DONE;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_WRITING;
    }
}

State handle_error(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from error state...\n");
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

State handle_done(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing complete!\n");
            return STATE_DONE;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_DONE;
    }
}

void initialize_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    
    sm->current_state = STATE_IDLE;
    sm->buffer_len = 0;
    sm->data_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

Event get_next_event(State current_state) {
    switch (current_state) {
        case STATE_IDLE: