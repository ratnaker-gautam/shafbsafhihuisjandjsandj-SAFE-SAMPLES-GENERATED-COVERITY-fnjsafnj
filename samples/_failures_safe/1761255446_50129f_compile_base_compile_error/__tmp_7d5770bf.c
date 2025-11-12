//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 64
#define MAX_STATES 8

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
    EVENT_RESET,
    EVENT_EXIT
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

void state_machine_init(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
}

State handle_idle_state(StateMachine *sm, Event event, const char *data) {
    switch (event) {
        case EVENT_START:
            printf("State: IDLE -> READING\n");
            return STATE_READING;
        case EVENT_EXIT:
            return STATE_DONE;
        default:
            return STATE_IDLE;
    }
}

State handle_reading_state(StateMachine *sm, Event event, const char *data) {
    switch (event) {
        case EVENT_DATA:
            if (data != NULL && sm->buffer_len < MAX_INPUT_LEN - 1) {
                size_t data_len = strlen(data);
                if (data_len > 0 && data_len < MAX_INPUT_LEN - sm->buffer_len) {
                    strncat(sm->buffer, data, data_len);
                    sm->buffer_len += data_len;
                    sm->data_count++;
                    printf("Data received: %s (total: %zu)\n", data, sm->buffer_len);
                }
            }
            return STATE_READING;
        case EVENT_PROCESS:
            if (sm->data_count > 0) {
                printf("State: READING -> PROCESSING\n");
                return STATE_PROCESSING;
            }
            return STATE_READING;
        case EVENT_ERROR:
            printf("State: READING -> ERROR\n");
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

State handle_processing_state(StateMachine *sm, Event event, const char *data) {
    switch (event) {
        case EVENT_WRITE:
            printf("State: PROCESSING -> WRITING\n");
            return STATE_WRITING;
        case EVENT_ERROR:
            printf("State: PROCESSING -> ERROR\n");
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

State handle_writing_state(StateMachine *sm, Event event, const char *data) {
    switch (event) {
        case EVENT_START:
            printf("Written data: %s\n", sm->buffer);
            memset(sm->buffer, 0, sizeof(sm->buffer));
            sm->buffer_len = 0;
            sm->data_count = 0;
            printf("State: WRITING -> IDLE\n");
            return STATE_IDLE;
        case EVENT_ERROR:
            printf("State: WRITING -> ERROR\n");
            return STATE_ERROR;
        default:
            return STATE_WRITING;
    }
}

State handle_error_state(StateMachine *sm, Event event, const char *data) {
    switch (event) {
        case EVENT_RESET:
            printf("State: ERROR -> IDLE\n");
            memset(sm->buffer, 0, sizeof(sm->buffer));
            sm->buffer_len = 0;
            sm->data_count = 0;
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

void process_event(StateMachine *sm, Event event, const char *data) {
    if (sm == NULL) return;
    
    State next_state = sm->current_state;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            next_state = handle_idle_state(sm, event, data);
            break;
        case STATE_READING:
            next_state = handle_reading_state(sm, event, data);
            break;
        case STATE_PROCESSING:
            next_state = handle_processing_state(sm, event, data);
            break;
        case STATE_WRITING:
            next_state = handle_writing_state(sm, event, data);
            break;
        case STATE_ERROR:
            next_state = handle_error_state(sm, event, data);
            break;
        case STATE_DONE:
            break;
    }
    
    sm->current_state = next_state;
}

Event get_user_event(void) {
    char input[MAX_INPUT_LEN];
    printf("\nAvailable events:\n");
    printf("0: START, 1: DATA, 2: PROCESS, 3: WRITE, 4: ERROR, 5: RESET, 6: EXIT\n");
    printf("Enter event number: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_EXIT;
    }
    
    char *endptr;
    long choice = strtol(input, &endptr, 10);
    if (endptr == input || choice < 0 || choice > 6) {
        return EVENT_EXIT;
    }
    
    return (Event)choice;
}

void get_data_input(char *buffer, size_t buffer_size) {
    if (buffer == NULL ||