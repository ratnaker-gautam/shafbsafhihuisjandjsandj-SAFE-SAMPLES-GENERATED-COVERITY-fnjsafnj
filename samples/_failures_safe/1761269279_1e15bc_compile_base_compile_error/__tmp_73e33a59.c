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
    enum Event event_type;
    char payload[128];
    uint32_t payload_len;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = 100;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int validate_event_data(const struct EventData *event_data) {
    if (event_data == NULL) return 0;
    if (event_data->event_type < EVENT_START || event_data->event_type > EVENT_RESET) return 0;
    if (event_data->payload_len >= sizeof(event_data->payload)) return 0;
    return 1;
}

void handle_idle_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return;
    
    if (event_data->event_type == EVENT_START) {
        sm->current_state = STATE_READY;
        printf("Transition: IDLE -> READY\n");
    }
}

void handle_ready_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return;
    
    if (event_data->event_type == EVENT_DATA_READY) {
        if (event_data->payload_len > 0 && event_data->payload_len < sizeof(sm->buffer)) {
            strncpy(sm->buffer, event_data->payload, event_data->payload_len);
            sm->buffer[event_data->payload_len] = '\0';
            sm->current_state = STATE_PROCESSING;
            printf("Transition: READY -> PROCESSING\n");
        } else {
            sm->current_state = STATE_ERROR;
            printf("Transition: READY -> ERROR\n");
        }
    } else if (event_data->event_type == EVENT_RESET) {
        sm->current_state = STATE_IDLE;
        printf("Transition: READY -> IDLE\n");
    }
}

void handle_processing_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return;
    
    if (event_data->event_type == EVENT_SUCCESS) {
        if (sm->data_count < sm->max_data) {
            sm->data_count++;
            sm->current_state = STATE_COMPLETE;
            printf("Transition: PROCESSING -> COMPLETE\n");
        } else {
            sm->current_state = STATE_ERROR;
            printf("Transition: PROCESSING -> ERROR\n");
        }
    } else if (event_data->event_type == EVENT_FAILURE) {
        sm->current_state = STATE_ERROR;
        printf("Transition: PROCESSING -> ERROR\n");
    } else if (event_data->event_type == EVENT_RESET) {
        sm->current_state = STATE_IDLE;
        printf("Transition: PROCESSING -> IDLE\n");
    }
}

void handle_complete_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return;
    
    if (event_data->event_type == EVENT_RESET) {
        sm->current_state = STATE_IDLE;
        sm->data_count = 0;
        memset(sm->buffer, 0, sizeof(sm->buffer));
        printf("Transition: COMPLETE -> IDLE\n");
    } else if (event_data->event_type == EVENT_DATA_READY) {
        sm->current_state = STATE_READY;
        printf("Transition: COMPLETE -> READY\n");
    }
}

void handle_error_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return;
    
    if (event_data->event_type == EVENT_RESET) {
        sm->current_state = STATE_IDLE;
        sm->data_count = 0;
        memset(sm->buffer, 0, sizeof(sm->buffer));
        printf("Transition: ERROR -> IDLE\n");
    }
}

void process_event(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            handle_idle_state(sm, event_data);
            break;
        case STATE_READY:
            handle_ready_state(sm, event_data);
            break;
        case STATE_PROCESSING:
            handle_processing_state(sm, event_data);
            break;
        case STATE_COMPLETE:
            handle_complete_state(sm, event_data);
            break;
        case STATE_ERROR:
            handle_error_state(sm, event_data);
            break;
    }
}

void print_state_info(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR