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
    uint32_t processed_count;
    uint32_t error_count;
    char buffer[256];
};

struct EventData {
    enum Event event_type;
    char data[128];
    uint32_t data_length;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->processed_count = 0;
    sm->error_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int validate_event_data(const struct EventData *event_data) {
    if (event_data == NULL) return 0;
    if (event_data->event_type < EVENT_START || event_data->event_type > EVENT_RESET) return 0;
    if (event_data->data_length >= sizeof(event_data->data)) return 0;
    return 1;
}

enum State handle_idle_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return STATE_ERROR;
    
    switch (event_data->event_type) {
        case EVENT_START:
            sm->data_count = 0;
            sm->processed_count = 0;
            sm->error_count = 0;
            return STATE_READY;
        default:
            return STATE_IDLE;
    }
}

enum State handle_ready_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return STATE_ERROR;
    
    switch (event_data->event_type) {
        case EVENT_DATA_READY:
            if (event_data->data_length > 0 && event_data->data_length < sizeof(sm->buffer)) {
                memcpy(sm->buffer, event_data->data, event_data->data_length);
                sm->data_count++;
                return STATE_PROCESSING;
            }
            return STATE_READY;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_READY;
    }
}

enum State handle_processing_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return STATE_ERROR;
    
    switch (event_data->event_type) {
        case EVENT_SUCCESS:
            sm->processed_count++;
            return STATE_COMPLETE;
        case EVENT_FAILURE:
            sm->error_count++;
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_PROCESSING;
    }
}

enum State handle_complete_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return STATE_ERROR;
    
    switch (event_data->event_type) {
        case EVENT_DATA_READY:
            if (event_data->data_length > 0 && event_data->data_length < sizeof(sm->buffer)) {
                memcpy(sm->buffer, event_data->data, event_data->data_length);
                sm->data_count++;
                return STATE_PROCESSING;
            }
            return STATE_COMPLETE;
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_COMPLETE;
    }
}

enum State handle_error_state(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || event_data == NULL) return STATE_ERROR;
    
    switch (event_data->event_type) {
        case EVENT_RESET:
            return STATE_IDLE;
        default:
            return STATE_ERROR;
    }
}

int process_event(struct StateMachine *sm, const struct EventData *event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return 0;
    
    enum State new_state = sm->current_state;
    
    switch (sm->current_state) {
        case STATE_IDLE:
            new_state = handle_idle_state(sm, event_data);
            break;
        case STATE_READY:
            new_state = handle_ready_state(sm, event_data);
            break;
        case STATE_PROCESSING:
            new_state = handle_processing_state(sm, event_data);
            break;
        case STATE_COMPLETE:
            new_state = handle_complete_state(sm, event_data);
            break;
        case STATE_ERROR:
            new_state = handle_error_state(sm, event_data);
            break;
        default:
            new_state = STATE_ERROR;
            break;
    }
    
    sm->current_state = new_state;
    return 1;
}

void print_state_machine_status(const struct StateMachine *sm) {
    if (sm == NULL) return;
    
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    printf("Current State: %s\n", state_names[sm->current_state]);
    printf("Data Count: %u\n", sm->data_count);
    printf("Processed Count: %u\n", sm->processed_count);
    printf("Error Count: %u