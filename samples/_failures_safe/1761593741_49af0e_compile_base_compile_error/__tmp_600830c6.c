//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_DONE,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_EXIT
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int processed_count;
} StateMachine;

State handle_start_state(StateMachine *sm, Event event);
State handle_reading_state(StateMachine *sm, Event event);
State handle_processing_state(StateMachine *sm, Event event);
State handle_done_state(StateMachine *sm, Event event);
State handle_error_state(StateMachine *sm, Event event);

void initialize_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->processed_count = 0;
}

State handle_start_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started\n");
            return STATE_READING;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (sm->buffer_len < MAX_INPUT_LEN - 1) {
                char input[MAX_INPUT_LEN];
                printf("Enter data (max %d chars): ", MAX_INPUT_LEN - 1);
                if (fgets(input, sizeof(input), stdin) != NULL) {
                    size_t len = strlen(input);
                    if (len > 0 && input[len-1] == '\n') {
                        input[len-1] = '\0';
                        len--;
                    }
                    if (sm->buffer_len + len < MAX_INPUT_LEN) {
                        strncpy(sm->buffer + sm->buffer_len, input, len);
                        sm->buffer_len += len;
                        sm->buffer[sm->buffer_len] = '\0';
                        printf("Data stored: %s\n", sm->buffer);
                        return STATE_PROCESSING;
                    }
                }
            }
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            if (sm->buffer_len > 0) {
                sm->processed_count++;
                printf("Processed: %s (count: %d)\n", sm->buffer, sm->processed_count);
                memset(sm->buffer, 0, sizeof(sm->buffer));
                sm->buffer_len = 0;
                return STATE_DONE;
            }
            return STATE_ERROR;
        case EVENT_ERROR:
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_done_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            return STATE_READING;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State handle_error_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from error state\n");
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

State process_event(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (sm->current_state) {
        case STATE_START:
            return handle_start_state(sm, event);
        case STATE_READING:
            return handle_reading_state(sm, event);
        case STATE_PROCESSING:
            return handle_processing_state(sm, event);
        case STATE_DONE:
            return handle_done_state(sm, event);
        case STATE_ERROR:
            return handle_error_state(sm, event);
        default:
            return STATE_ERROR;
    }
}

Event get_user_event() {
    char input[10];
    printf("\nChoose event (0=START, 1=DATA, 2=COMPLETE, 3=ERROR, 4=RESET, 5=EXIT): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    char *endptr;
    long choice = strtol(input, &endptr, 10);
    if (endptr == input || *endptr != '\n') {
        return EVENT_ERROR;
    }
    
    if (choice < 0 || choice > 5) {
        return EVENT_ERROR;
    }
    
    return (Event)choice;
}

int main() {
    StateMachine machine;
    initialize_machine(&machine);
    
    printf("State Machine Demo\n");
    printf