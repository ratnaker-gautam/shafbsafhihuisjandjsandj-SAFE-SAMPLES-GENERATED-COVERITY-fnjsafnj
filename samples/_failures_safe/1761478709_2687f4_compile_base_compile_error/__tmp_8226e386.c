//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET
};

struct StateMachine {
    enum State current_state;
    uint32_t data_value;
    uint8_t retry_count;
    uint8_t max_retries;
};

struct EventData {
    enum Event event_type;
    uint32_t data_payload;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->retry_count = 0;
    sm->max_retries = 3;
}

const char* state_to_string(enum State state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_READY: return "READY";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_COMPLETE: return "COMPLETE";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

const char* event_to_string(enum Event event) {
    switch (event) {
        case EVENT_START: return "START";
        case EVENT_DATA_READY: return "DATA_READY";
        case EVENT_PROCESS: return "PROCESS";
        case EVENT_SUCCESS: return "SUCCESS";
        case EVENT_FAILURE: return "FAILURE";
        case EVENT_RESET: return "RESET";
        default: return "UNKNOWN";
    }
}

int validate_event_for_state(enum State state, enum Event event) {
    switch (state) {
        case STATE_IDLE:
            return (event == EVENT_START || event == EVENT_RESET);
        case STATE_READY:
            return (event == EVENT_DATA_READY || event == EVENT_RESET);
        case STATE_PROCESSING:
            return (event == EVENT_SUCCESS || event == EVENT_FAILURE || event == EVENT_RESET);
        case STATE_COMPLETE:
            return (event == EVENT_RESET);
        case STATE_ERROR:
            return (event == EVENT_RESET);
        default:
            return 0;
    }
}

void process_event(struct StateMachine *sm, struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return;
    
    if (!validate_event_for_state(sm->current_state, event_data->event_type)) {
        printf("Invalid event %s for state %s\n", 
               event_to_string(event_data->event_type),
               state_to_string(sm->current_state));
        return;
    }

    printf("Processing event %s in state %s\n",
           event_to_string(event_data->event_type),
           state_to_string(sm->current_state));

    switch (sm->current_state) {
        case STATE_IDLE:
            if (event_data->event_type == EVENT_START) {
                sm->current_state = STATE_READY;
                sm->data_value = 0;
            }
            break;
            
        case STATE_READY:
            if (event_data->event_type == EVENT_DATA_READY) {
                sm->data_value = event_data->data_payload;
                sm->current_state = STATE_PROCESSING;
                sm->retry_count = 0;
            }
            break;
            
        case STATE_PROCESSING:
            if (event_data->event_type == EVENT_SUCCESS) {
                sm->current_state = STATE_COMPLETE;
                printf("Processing completed successfully. Result: %u\n", sm->data_value * 2);
            } else if (event_data->event_type == EVENT_FAILURE) {
                sm->retry_count++;
                if (sm->retry_count >= sm->max_retries) {
                    sm->current_state = STATE_ERROR;
                    printf("Max retries exceeded. Entering error state.\n");
                } else {
                    printf("Processing failed. Retry %u/%u\n", sm->retry_count, sm->max_retries);
                }
            }
            break;
            
        case STATE_COMPLETE:
        case STATE_ERROR:
            if (event_data->event_type == EVENT_RESET) {
                state_machine_init(sm);
                printf("State machine reset to IDLE\n");
            }
            break;
    }
}

int main(void) {
    struct StateMachine sm;
    struct EventData events[] = {
        {EVENT_START, 0},
        {EVENT_DATA_READY, 42},
        {EVENT_FAILURE, 0},
        {EVENT_FAILURE, 0},
        {EVENT_FAILURE, 0},
        {EVENT_RESET, 0},
        {EVENT_START, 0},
        {EVENT_DATA_READY, 100},
        {EVENT_SUCCESS, 0}
    };
    
    size_t num_events = sizeof(events) / sizeof(events[0]);
    
    state_machine_init(&sm);
    
    printf("State Machine Simulation Started\n");
    printf("Initial state: %s\n", state_to_string(sm.current_state));
    
    for (size_t i = 0; i < num_events; i++) {
        printf("\n--- Step %zu ---\n", i + 1);
        process_event(&sm, &events[i]);
        printf("