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
    uint32_t process_count;
    uint32_t error_count;
};

struct EventData {
    enum Event event_type;
    uint32_t data_payload;
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
    return 1;
}

static int action_start(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || ed == NULL) return 0;
    sm->data_value = 0;
    sm->process_count = 0;
    sm->error_count = 0;
    return 1;
}

static int action_data_ready(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || ed == NULL) return 0;
    if (ed->data_payload > 1000) return 0;
    sm->data_value = ed->data_payload;
    return 1;
}

static int action_process(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || ed == NULL) return 0;
    if (sm->data_value == 0) return 0;
    if (sm->process_count >= 10) return 0;
    sm->process_count++;
    return 1;
}

static int action_success(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || ed == NULL) return 0;
    sm->data_value = 0;
    return 1;
}

static int action_failure(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || ed == NULL) return 0;
    if (sm->error_count >= 3) return 0;
    sm->error_count++;
    return 1;
}

static int action_reset(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || ed == NULL) return 0;
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->process_count = 0;
    sm->error_count = 0;
    return 1;
}

static const struct StateTransition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READY, action_start},
    {STATE_READY, EVENT_DATA_READY, STATE_PROCESSING, action_data_ready},
    {STATE_PROCESSING, EVENT_PROCESS, STATE_PROCESSING, action_process},
    {STATE_PROCESSING, EVENT_SUCCESS, STATE_COMPLETE, action_success},
    {STATE_PROCESSING, EVENT_FAILURE, STATE_ERROR, action_failure},
    {STATE_COMPLETE, EVENT_RESET, STATE_IDLE, action_reset},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE, action_reset}
};

static const size_t num_transitions = sizeof(transitions) / sizeof(transitions[0]);

static int handle_event(struct StateMachine* sm, struct EventData* event_data) {
    if (sm == NULL || !validate_event_data(event_data)) return 0;
    
    for (size_t i = 0; i < num_transitions; i++) {
        if (transitions[i].from_state == sm->current_state && 
            transitions[i].event == event_data->event_type) {
            
            if (transitions[i].action != NULL) {
                if (!transitions[i].action(sm, event_data)) {
                    return 0;
                }
            }
            
            sm->current_state = transitions[i].to_state;
            return 1;
        }
    }
    
    return 0;
}

static void print_state_info(const struct StateMachine* sm) {
    if (sm == NULL) return;
    
    const char* state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    printf("Current State: %s\n", state_names[sm->current_state]);
    printf("Data Value: %u\n", sm->data_value);
    printf("Process Count: %u\n", sm->process_count);
    printf("Error Count: %u\n", sm->error_count);
    printf("---\n");
}

int main(void) {
    struct StateMachine sm = {STATE_IDLE, 0, 0, 0};
    struct EventData events[] = {
        {EVENT_START, 0},
        {EVENT_DATA_READY, 42},
        {EVENT_PROCESS, 0},
        {EVENT_PROCESS, 0},
        {EVENT_SUCCESS, 0},
        {EVENT_RESET, 0},
        {EVENT_START, 0},
        {EVENT_DATA_READY, 999},
        {EVENT_P