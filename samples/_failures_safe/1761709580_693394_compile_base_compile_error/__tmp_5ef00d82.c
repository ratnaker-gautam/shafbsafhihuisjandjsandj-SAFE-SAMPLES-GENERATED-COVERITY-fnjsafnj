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
    STATE_COMPLETE,
    STATE_ERROR
} State;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_END,
    EVENT_CANCEL,
    EVENT_INVALID
} Event;

typedef struct {
    State current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} StateMachine;

StateMachine* create_state_machine(void) {
    StateMachine* sm = malloc(sizeof(StateMachine));
    if (sm == NULL) return NULL;
    
    sm->current_state = STATE_IDLE;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
    
    return sm;
}

void destroy_state_machine(StateMachine* sm) {
    if (sm != NULL) {
        free(sm);
    }
}

Event get_next_event(void) {
    char input[MAX_INPUT_LEN];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_INVALID;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strcmp(input, "start") == 0) return EVENT_START;
    if (strcmp(input, "data") == 0) return EVENT_DATA;
    if (strcmp(input, "end") == 0) return EVENT_END;
    if (strcmp(input, "cancel") == 0) return EVENT_CANCEL;
    if (strcmp(input, "quit") == 0) return EVENT_INVALID;
    
    return EVENT_INVALID;
}

int handle_idle_state(StateMachine* sm, Event event) {
    switch (event) {
        case EVENT_START:
            sm->current_state = STATE_READING;
            sm->buffer_len = 0;
            sm->data_count = 0;
            printf("State: READING - Ready to receive data\n");
            return 1;
        default:
            printf("Error: Invalid event for IDLE state\n");
            return 0;
    }
}

int handle_reading_state(StateMachine* sm, Event event) {
    switch (event) {
        case EVENT_DATA:
            if (sm->data_count < 3) {
                sm->data_count++;
                printf("State: READING - Data received (%d/3)\n", sm->data_count);
                return 1;
            } else {
                sm->current_state = STATE_PROCESSING;
                printf("State: PROCESSING - Processing collected data\n");
                return 1;
            }
        case EVENT_END:
            if (sm->data_count > 0) {
                sm->current_state = STATE_PROCESSING;
                printf("State: PROCESSING - Processing collected data\n");
                return 1;
            } else {
                printf("Error: No data to process\n");
                return 0;
            }
        case EVENT_CANCEL:
            sm->current_state = STATE_IDLE;
            printf("State: IDLE - Operation cancelled\n");
            return 1;
        default:
            printf("Error: Invalid event for READING state\n");
            return 0;
    }
}

int handle_processing_state(StateMachine* sm, Event event) {
    switch (event) {
        case EVENT_END:
            sm->current_state = STATE_COMPLETE;
            printf("State: COMPLETE - Processed %d data items\n", sm->data_count);
            return 1;
        case EVENT_CANCEL:
            sm->current_state = STATE_ERROR;
            printf("State: ERROR - Processing cancelled\n");
            return 1;
        default:
            printf("Error: Invalid event for PROCESSING state\n");
            return 0;
    }
}

int handle_complete_state(StateMachine* sm, Event event) {
    switch (event) {
        case EVENT_START:
            sm->current_state = STATE_READING;
            sm->buffer_len = 0;
            sm->data_count = 0;
            printf("State: READING - Starting new operation\n");
            return 1;
        default:
            printf("Error: Invalid event for COMPLETE state\n");
            return 0;
    }
}

int handle_error_state(StateMachine* sm, Event event) {
    switch (event) {
        case EVENT_START:
            sm->current_state = STATE_READING;
            sm->buffer_len = 0;
            sm->data_count = 0;
            printf("State: READING - Recovery from error\n");
            return 1;
        default:
            printf("Error: Invalid event for ERROR state\n");
            return 0;
    }
}

int process_event(StateMachine* sm, Event event) {
    if (sm == NULL) return 0;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            return handle_idle_state(sm, event);
        case STATE_READING:
            return handle_reading_state(sm, event);
        case STATE_PROCESSING:
            return handle_processing_state(sm, event);
        case STATE_COMPLETE:
            return handle_complete_state(sm, event);
        case STATE_ERROR:
            return handle_error_state(sm, event);
        default:
            return 0;
    }
}

void print_help(void) {
    printf("Available commands: start, data