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

struct StateTransition {
    enum State from_state;
    enum Event event;
    enum State to_state;
    int (*action)(struct StateMachine*, struct EventData*);
};

static int validate_event_data(const struct EventData* event_data) {
    if (event_data == NULL) return 0;
    if (event_data->event_type < EVENT_START || event_data->event_type > EVENT_RESET) return 0;
    if (event_data->data_len >= sizeof(event_data->data)) return 0;
    return 1;
}

static int action_idle_to_ready(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || !validate_event_data(ed)) return 0;
    printf("Transitioning from IDLE to READY\n");
    sm->data_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    return 1;
}

static int action_ready_to_processing(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || !validate_event_data(ed)) return 0;
    if (ed->data_len == 0) return 0;
    printf("Processing data: %s\n", ed->data);
    if (sm->data_count < sm->max_data) {
        size_t copy_len = ed->data_len;
        if (copy_len >= sizeof(sm->buffer)) copy_len = sizeof(sm->buffer) - 1;
        memcpy(sm->buffer, ed->data, copy_len);
        sm->buffer[copy_len] = '\0';
        sm->data_count++;
    }
    return 1;
}

static int action_processing_to_complete(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || !validate_event_data(ed)) return 0;
    printf("Processing complete. Processed %u items\n", sm->data_count);
    return 1;
}

static int action_to_error(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || !validate_event_data(ed)) return 0;
    printf("Error occurred in state machine\n");
    return 1;
}

static int action_reset(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || !validate_event_data(ed)) return 0;
    printf("Resetting state machine\n");
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    return 1;
}

static const struct StateTransition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READY, action_idle_to_ready},
    {STATE_READY, EVENT_DATA_READY, STATE_PROCESSING, action_ready_to_processing},
    {STATE_PROCESSING, EVENT_SUCCESS, STATE_COMPLETE, action_processing_to_complete},
    {STATE_PROCESSING, EVENT_FAILURE, STATE_ERROR, action_to_error},
    {STATE_COMPLETE, EVENT_RESET, STATE_IDLE, action_reset},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE, action_reset}
};

static int process_event(struct StateMachine* sm, struct EventData* event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return 0;
    
    for (size_t i = 0; i < sizeof(transitions) / sizeof(transitions[0]); i++) {
        if (transitions[i].from_state == sm->current_state && 
            transitions[i].event == event_data->event_type) {
            if (transitions[i].action(sm, event_data)) {
                sm->current_state = transitions[i].to_state;
                return 1;
            }
        }
    }
    return 0;
}

static void initialize_state_machine(struct StateMachine* sm, uint32_t max_data) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = (max_data > 0 && max_data < 1000) ? max_data : 10;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int main(void) {
    struct StateMachine sm;
    initialize_state_machine(&sm, 5);
    
    struct EventData events[] = {
        {EVENT_START, "Start processing", 16},
        {EVENT_DATA_READY, "Sample data 1", 13},
        {EVENT_DATA_READY, "Sample data 2", 13},
        {EVENT_DATA_READY, "Sample data 3", 13},
        {EVENT_SUCCESS, "Processing done", 15},
        {EVENT_RESET, "", 0}
    };
    
    for (size_t i =