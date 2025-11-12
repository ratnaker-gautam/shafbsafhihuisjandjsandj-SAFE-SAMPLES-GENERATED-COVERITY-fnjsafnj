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
    char data[128];
    uint32_t data_len;
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
    if (event_data->data_len >= sizeof(event_data->data)) return 0;
    return 1;
}

int process_idle_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    
    if (event_data->event_type == EVENT_START) {
        sm->current_state = STATE_READY;
        printf("Transition: IDLE -> READY\n");
        return 1;
    }
    return 0;
}

int process_ready_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    
    if (event_data->event_type == EVENT_DATA_READY) {
        if (event_data->data_len > 0 && event_data->data_len < sizeof(sm->buffer)) {
            strncpy(sm->buffer, event_data->data, event_data->data_len);
            sm->buffer[event_data->data_len] = '\0';
            sm->current_state = STATE_PROCESSING;
            printf("Transition: READY -> PROCESSING (Data: %s)\n", sm->buffer);
            return 1;
        }
    } else if (event_data->event_type == EVENT_RESET) {
        sm->current_state = STATE_IDLE;
        printf("Transition: READY -> IDLE\n");
        return 1;
    }
    return 0;
}

int process_processing_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    
    if (event_data->event_type == EVENT_SUCCESS) {
        if (sm->data_count < sm->max_data) {
            sm->data_count++;
        }
        sm->current_state = STATE_COMPLETE;
        printf("Transition: PROCESSING -> COMPLETE (Count: %u)\n", sm->data_count);
        return 1;
    } else if (event_data->event_type == EVENT_FAILURE) {
        sm->current_state = STATE_ERROR;
        printf("Transition: PROCESSING -> ERROR\n");
        return 1;
    } else if (event_data->event_type == EVENT_RESET) {
        sm->current_state = STATE_IDLE;
        printf("Transition: PROCESSING -> IDLE\n");
        return 1;
    }
    return 0;
}

int process_complete_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    
    if (event_data->event_type == EVENT_RESET) {
        sm->current_state = STATE_IDLE;
        sm->data_count = 0;
        memset(sm->buffer, 0, sizeof(sm->buffer));
        printf("Transition: COMPLETE -> IDLE\n");
        return 1;
    } else if (event_data->event_type == EVENT_START) {
        sm->current_state = STATE_READY;
        printf("Transition: COMPLETE -> READY\n");
        return 1;
    }
    return 0;
}

int process_error_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return 0;
    
    if (event_data->event_type == EVENT_RESET) {
        sm->current_state = STATE_IDLE;
        sm->data_count = 0;
        memset(sm->buffer, 0, sizeof(sm->buffer));
        printf("Transition: ERROR -> IDLE\n");
        return 1;
    }
    return 0;
}

int handle_event(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return 0;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            return process_idle_state(sm, event_data);
        case STATE_READY:
            return process_ready_state(sm, event_data);
        case STATE_PROCESSING:
            return process_processing_state(sm, event_data);
        case STATE_COMPLETE:
            return process_complete_state(sm, event_data);
        case STATE_ERROR:
            return process_error_state(sm, event_data);
        default:
            return 0;
    }
}

void print