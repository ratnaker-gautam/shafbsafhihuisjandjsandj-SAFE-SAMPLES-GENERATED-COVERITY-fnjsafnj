//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
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
    STATE_FINISHED,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_DONE,
    EVENT_ERROR
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

State handle_start_state(StateMachine *sm, Event event);
State handle_reading_state(StateMachine *sm, Event event);
State handle_processing_state(StateMachine *sm, Event event);
State handle_finished_state(StateMachine *sm, Event event);
State handle_error_state(StateMachine *sm, Event event);

void initialize_state_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_START;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

State handle_start_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started\n");
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (sm->buffer_len < MAX_INPUT_LEN - 1) {
                printf("Enter data (max %d chars): ", MAX_INPUT_LEN - 1);
                if (fgets(sm->buffer, sizeof(sm->buffer), stdin) != NULL) {
                    sm->buffer_len = strlen(sm->buffer);
                    if (sm->buffer_len > 0 && sm->buffer[sm->buffer_len - 1] == '\n') {
                        sm->buffer[sm->buffer_len - 1] = '\0';
                        sm->buffer_len--;
                    }
                    
                    if (validate_input(sm->buffer, sm->buffer_len)) {
                        sm->data_count++;
                        printf("Data received: %s (count: %d)\n", sm->buffer, sm->data_count);
                        return STATE_PROCESSING;
                    } else {
                        printf("Invalid input\n");
                        return STATE_ERROR;
                    }
                } else {
                    return STATE_ERROR;
                }
            } else {
                return STATE_ERROR;
            }
        case EVENT_DONE:
            return STATE_FINISHED;
        default:
            return STATE_ERROR;
    }
}

State handle_processing_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS:
            printf("Processing data: ");
            for (size_t i = 0; i < sm->buffer_len && i < MAX_INPUT_LEN; i++) {
                if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
                    printf("%c", sm->buffer[i] - 32);
                } else {
                    printf("%c", sm->buffer[i]);
                }
            }
            printf("\n");
            
            if (sm->data_count < 3) {
                return STATE_READING;
            } else {
                return STATE_FINISHED;
            }
        default:
            return STATE_ERROR;
    }
}

State handle_finished_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DONE:
            printf("Processing complete. Total data items: %d\n", sm->data_count);
            return STATE_FINISHED;
        default:
            return STATE_ERROR;
    }
}

State handle_error_state(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    printf("Error state reached\n");
    return STATE_ERROR;
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
        case STATE_FINISHED:
            return handle_finished_state(sm, event);
        case STATE_ERROR:
            return handle_error_state(sm, event);
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    StateMachine sm;
    initialize_state_machine(&sm);
    
    sm.current_state = process_event(&sm, EVENT_START);
    
    while (sm.current_state != STATE_FINISHED && sm.current_state != STATE_ERROR) {
        switch (sm.current_state) {
            case STATE_READING: