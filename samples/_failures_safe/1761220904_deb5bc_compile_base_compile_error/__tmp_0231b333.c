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
    uint32_t data_count;
    uint32_t max_data;
    char buffer[256];
};

struct EventData {
    enum Event event;
    char payload[128];
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = 100;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int state_machine_handle_event(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return -1;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            if (event_data->event == EVENT_START) {
                sm->current_state = STATE_READY;
                printf("Transition: IDLE -> READY\n");
            }
            break;
            
        case STATE_READY:
            if (event_data->event == EVENT_DATA_READY) {
                size_t payload_len = strlen(event_data->payload);
                if (payload_len < sizeof(sm->buffer)) {
                    strncpy(sm->buffer, event_data->payload, sizeof(sm->buffer) - 1);
                    sm->buffer[sizeof(sm->buffer) - 1] = '\0';
                    sm->current_state = STATE_PROCESSING;
                    printf("Transition: READY -> PROCESSING\n");
                } else {
                    sm->current_state = STATE_ERROR;
                    printf("Transition: READY -> ERROR\n");
                }
            }
            break;
            
        case STATE_PROCESSING:
            if (event_data->event == EVENT_SUCCESS) {
                if (sm->data_count < UINT32_MAX) {
                    sm->data_count++;
                }
                if (sm->data_count >= sm->max_data) {
                    sm->current_state = STATE_COMPLETE;
                    printf("Transition: PROCESSING -> COMPLETE\n");
                } else {
                    sm->current_state = STATE_READY;
                    printf("Transition: PROCESSING -> READY\n");
                }
            } else if (event_data->event == EVENT_FAILURE) {
                sm->current_state = STATE_ERROR;
                printf("Transition: PROCESSING -> ERROR\n");
            }
            break;
            
        case STATE_COMPLETE:
            if (event_data->event == EVENT_RESET) {
                sm->data_count = 0;
                sm->current_state = STATE_IDLE;
                printf("Transition: COMPLETE -> IDLE\n");
            }
            break;
            
        case STATE_ERROR:
            if (event_data->event == EVENT_RESET) {
                sm->data_count = 0;
                sm->current_state = STATE_IDLE;
                printf("Transition: ERROR -> IDLE\n");
            }
            break;
    }
    
    return 0;
}

void print_state_info(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    printf("Current State: ");
    switch (sm->current_state) {
        case STATE_IDLE: printf("IDLE"); break;
        case STATE_READY: printf("READY"); break;
        case STATE_PROCESSING: printf("PROCESSING"); break;
        case STATE_COMPLETE: printf("COMPLETE"); break;
        case STATE_ERROR: printf("ERROR"); break;
    }
    printf(", Data Count: %u/%u\n", sm->data_count, sm->max_data);
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    struct EventData events[] = {
        {EVENT_START, ""},
        {EVENT_DATA_READY, "Sample data payload"},
        {EVENT_SUCCESS, ""},
        {EVENT_DATA_READY, "Another data sample"},
        {EVENT_SUCCESS, ""},
        {EVENT_FAILURE, "Processing failed"},
        {EVENT_RESET, ""},
        {EVENT_START, ""},
        {EVENT_DATA_READY, "Final test data"},
        {EVENT_SUCCESS, ""}
    };
    
    size_t num_events = sizeof(events) / sizeof(events[0]);
    
    printf("State Machine Simulation\n");
    printf("========================\n");
    
    for (size_t i = 0; i < num_events; i++) {
        printf("\nEvent %zu: ", i + 1);
        switch (events[i].event) {
            case EVENT_START: printf("START"); break;
            case EVENT_DATA_READY: printf("DATA_READY"); break;
            case EVENT_PROCESS: printf("PROCESS"); break;
            case EVENT_SUCCESS: printf("SUCCESS"); break;
            case EVENT_FAILURE: printf("FAILURE"); break;
            case EVENT_RESET: printf("RESET"); break;
        }
        printf("\n");
        
        if (state_machine_handle_event(&sm, &events[i]) != 0) {
            fprintf(stderr, "Error handling event\n");
            return 1;
        }
        
        print_state_info(&sm);
    }