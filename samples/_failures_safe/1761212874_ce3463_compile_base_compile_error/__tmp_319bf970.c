//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

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
    if (event_data->event < EVENT_START || event_data->event > EVENT_RESET) return 0;
    if (event_data->data_len >= sizeof(event_data->data)) return 0;
    return 1;
}

static int action_idle_to_ready(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || !validate_event_data(ed)) return 0;
    printf("Transitioning from IDLE to READY\n");
    sm->data_count = 0;
    return 1;
}

static int action_ready_to_processing(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || !validate_event_data(ed)) return 0;
    if (ed->data_len == 0) return 0;
    if (sm->data_count >= sm->max_data) return 0;
    
    size_t copy_len = ed->data_len;
    if (copy_len >= sizeof(sm->buffer) - sm->data_count) {
        copy_len = sizeof(sm->buffer) - sm->data_count - 1;
    }
    
    memcpy(sm->buffer + sm->data_count, ed->data, copy_len);
    sm->data_count += copy_len;
    sm->buffer[sm->data_count] = '\0';
    
    printf("Processing data: %s\n", sm->buffer);
    return 1;
}

static int action_processing_to_complete(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || !validate_event_data(ed)) return 0;
    printf("Processing complete. Total data: %u bytes\n", sm->data_count);
    return 1;
}

static int action_to_error(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || !validate_event_data(ed)) return 0;
    printf("Error occurred in state machine\n");
    return 1;
}

static int action_reset(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || !validate_event_data(ed)) return 0;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    printf("State machine reset to IDLE\n");
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
            transitions[i].event == event_data->event) {
            
            if (transitions[i].action(sm, event_data)) {
                sm->current_state = transitions[i].to_state;
                return 1;
            }
        }
    }
    
    return 0;
}

static void initialize_state_machine(struct StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = sizeof(sm->buffer) - 1;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int main(void) {
    struct StateMachine sm;
    initialize_state_machine(&sm);
    
    struct EventData events[] = {
        {EVENT_START, "Initial data", 12},
        {EVENT_DATA_READY, "Sample input data", 17},
        {EVENT_SUCCESS, "", 0},
        {EVENT_RESET, "", 0},
        {EVENT_START, "New session", 11},
        {EVENT_DATA_READY, "More data to process", 20},
        {EVENT_FAILURE, "", 0},
        {EVENT_RESET, "", 0}
    };