//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: state_machine
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

State handle_idle(StateMachine *sm, Event event);
State handle_reading(StateMachine *sm, Event event);
State handle_processing(StateMachine *sm, Event event);
State handle_writing(StateMachine *sm, Event event);
State handle_error(StateMachine *sm, Event event);
State handle_done(StateMachine *sm, Event event);

void initialize_machine(StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    sm->buffer_len = 0;
    sm->data_count = 0;
}

int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == 0) break;
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

Event get_user_input(char *buffer, size_t max_len) {
    if (buffer == NULL || max_len == 0) return EVENT_ERROR;
    
    printf("Enter command (start/data/process/write/reset/exit): ");
    if (fgets(buffer, max_len, stdin) == NULL) return EVENT_ERROR;
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';
    
    if (strcmp(buffer, "start") == 0) return EVENT_START;
    if (strcmp(buffer, "data") == 0) return EVENT_DATA;
    if (strcmp(buffer, "process") == 0) return EVENT_PROCESS;
    if (strcmp(buffer, "write") == 0) return EVENT_WRITE;
    if (strcmp(buffer, "reset") == 0) return EVENT_RESET;
    if (strcmp(buffer, "exit") == 0) return EVENT_EXIT;
    
    return EVENT_ERROR;
}

State handle_idle(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("Starting data collection\n");
            return STATE_READING;
        case EVENT_EXIT:
            return STATE_DONE;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_IDLE;
        default:
            printf("Invalid command for IDLE state\n");
            return STATE_IDLE;
    }
}

State handle_reading(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            printf("Enter data (max %d chars): ", MAX_INPUT_LEN - 1);
            if (fgets(sm->buffer, sizeof(sm->buffer), stdin) == NULL) {
                return STATE_ERROR;
            }
            sm->buffer_len = strlen(sm->buffer);
            if (sm->buffer_len > 0 && sm->buffer[sm->buffer_len-1] == '\n') {
                sm->buffer[sm->buffer_len-1] = '\0';
                sm->buffer_len--;
            }
            if (!validate_input(sm->buffer, sm->buffer_len)) {
                printf("Invalid input data\n");
                return STATE_ERROR;
            }
            sm->data_count++;
            printf("Data stored: %s (count: %d)\n", sm->buffer, sm->data_count);
            return STATE_READING;
        case EVENT_PROCESS:
            if (sm->data_count > 0) {
                printf("Moving to processing\n");
                return STATE_PROCESSING;
            }
            printf("No data to process\n");
            return STATE_READING;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_IDLE;
        case EVENT_EXIT:
            return STATE_DONE;
        default:
            printf("Invalid command for READING state\n");
            return STATE_READING;
    }
}

State handle_processing(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_WRITE:
            printf("Processing complete. Data: %s\n", sm->buffer);
            return STATE_WRITING;
        case EVENT_RESET:
            initialize_machine(sm);
            return STATE_IDLE;
        case EVENT_EXIT:
            return STATE_DONE;
        default:
            printf("Invalid command for PROCESSING state\n");
            return STATE_PROCESSING;
    }
}

State handle_writing(StateMachine *sm, Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_ST