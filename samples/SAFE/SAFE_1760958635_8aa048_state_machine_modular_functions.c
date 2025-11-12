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
    EVENT_COMPLETE,
    EVENT_ERROR
} Event;

State handle_start(Event event, const char* data);
State handle_reading(Event event, const char* data);
State handle_processing(Event event, const char* data);
State handle_finished(Event event, const char* data);
State handle_error(Event event, const char* data);

typedef State (*StateHandler)(Event, const char*);

StateHandler handlers[] = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_finished,
    handle_error
};

State handle_start(Event event, const char* data) {
    if (data == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started\n");
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

State handle_reading(Event event, const char* data) {
    if (data == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (strlen(data) > 0 && strlen(data) < MAX_INPUT_LEN) {
                printf("Reading data: %s\n", data);
                return STATE_PROCESSING;
            }
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

State handle_processing(Event event, const char* data) {
    if (data == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS:
            printf("Processing data: %s\n", data);
            return STATE_FINISHED;
        default:
            return STATE_ERROR;
    }
}

State handle_finished(Event event, const char* data) {
    if (data == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing complete\n");
            return STATE_FINISHED;
        default:
            return STATE_ERROR;
    }
}

State handle_error(Event event, const char* data) {
    printf("Error state reached\n");
    return STATE_ERROR;
}

int process_event(State* current_state, Event event, const char* data) {
    if (current_state == NULL || data == NULL) return 0;
    if (*current_state < 0 || *current_state >= (sizeof(handlers)/sizeof(handlers[0]))) return 0;
    
    State new_state = handlers[*current_state](event, data);
    if (new_state < 0 || new_state >= (sizeof(handlers)/sizeof(handlers[0]))) return 0;
    
    *current_state = new_state;
    return 1;
}

int main(void) {
    State current_state = STATE_START;
    char input[MAX_INPUT_LEN];
    
    if (!process_event(&current_state, EVENT_START, "init")) {
        printf("Failed to start state machine\n");
        return 1;
    }
    
    printf("Enter data to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input not allowed\n");
        return 1;
    }
    
    if (!process_event(&current_state, EVENT_DATA, input)) {
        printf("Failed to process data event\n");
        return 1;
    }
    
    if (!process_event(&current_state, EVENT_PROCESS, input)) {
        printf("Failed to process event\n");
        return 1;
    }
    
    if (!process_event(&current_state, EVENT_COMPLETE, "done")) {
        printf("Failed to complete\n");
        return 1;
    }
    
    if (current_state == STATE_FINISHED) {
        printf("State machine completed successfully\n");
    } else {
        printf("State machine ended in unexpected state\n");
        return 1;
    }
    
    return 0;
}