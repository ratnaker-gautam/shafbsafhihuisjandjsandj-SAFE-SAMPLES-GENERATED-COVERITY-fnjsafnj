//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
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

State (*state_handlers[])(Event, const char*) = {
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
            if (strlen(data) > 0 && strlen(data) < MAX_INPUT_LENGTH) {
                printf("Read data: %s\n", data);
                return STATE_PROCESSING;
            }
            return STATE_ERROR;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

State handle_processing(Event event, const char* data) {
    if (data == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS:
            printf("Processing data: %s\n", data);
            return STATE_FINISHED;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

State handle_finished(Event event, const char* data) {
    if (data == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing complete\n");
            return STATE_FINISHED;
        default:
            return STATE_FINISHED;
    }
}

State handle_error(Event event, const char* data) {
    printf("Error state encountered\n");
    return STATE_ERROR;
}

Event get_next_event(State current_state) {
    static int event_sequence[] = {EVENT_START, EVENT_DATA, EVENT_PROCESS, EVENT_COMPLETE};
    static int sequence_index = 0;
    
    if (sequence_index >= 4) {
        return EVENT_ERROR;
    }
    
    return event_sequence[sequence_index++];
}

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LENGTH) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    State current_state = STATE_START;
    char input_buffer[MAX_INPUT_LENGTH];
    int running = 1;
    
    printf("Enter data to process: ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        printf("Input error\n");
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (!validate_input(input_buffer)) {
        printf("Invalid input\n");
        return 1;
    }
    
    while (running) {
        Event next_event = get_next_event(current_state);
        
        if (next_event == EVENT_ERROR) {
            running = 0;
            continue;
        }
        
        State next_state = state_handlers[current_state](next_event, input_buffer);
        
        if (next_state == STATE_ERROR) {
            printf("Transition to error state\n");
            running = 0;
        } else if (next_state == STATE_FINISHED) {
            printf("Final state reached\n");
            running = 0;
        }
        
        current_state = next_state;
    }
    
    return 0;
}