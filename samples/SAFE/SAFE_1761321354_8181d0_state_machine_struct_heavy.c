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

int action_start(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || ed == NULL) return 0;
    sm->data_count = 0;
    printf("State machine started\n");
    return 1;
}

int action_data_ready(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || ed == NULL) return 0;
    if (ed->data_len >= sizeof(sm->buffer)) return 0;
    memcpy(sm->buffer, ed->data, ed->data_len);
    sm->buffer[ed->data_len] = '\0';
    sm->data_count++;
    printf("Data received: %s\n", sm->buffer);
    return 1;
}

int action_process(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL) return 0;
    if (sm->data_count == 0) return 0;
    printf("Processing %u data items\n", sm->data_count);
    return 1;
}

int action_complete(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL) return 0;
    printf("Processing complete. Total data: %u\n", sm->data_count);
    return 1;
}

int action_error(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL) return 0;
    printf("Error occurred in state machine\n");
    return 1;
}

int action_reset(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL) return 0;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    printf("State machine reset\n");
    return 1;
}

struct StateTransition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READY, action_start},
    {STATE_READY, EVENT_DATA_READY, STATE_READY, action_data_ready},
    {STATE_READY, EVENT_PROCESS, STATE_PROCESSING, action_process},
    {STATE_PROCESSING, EVENT_SUCCESS, STATE_COMPLETE, action_complete},
    {STATE_PROCESSING, EVENT_FAILURE, STATE_ERROR, action_error},
    {STATE_COMPLETE, EVENT_RESET, STATE_IDLE, action_reset},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE, action_reset}
};

int handle_event(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || ed == NULL) return 0;
    
    for (size_t i = 0; i < sizeof(transitions)/sizeof(transitions[0]); i++) {
        if (transitions[i].from_state == sm->current_state && 
            transitions[i].event == ed->event_type) {
            
            if (transitions[i].action(sm, ed)) {
                sm->current_state = transitions[i].to_state;
                return 1;
            }
            return 0;
        }
    }
    return 0;
}

void initialize_state_machine(struct StateMachine* sm, uint32_t max_data) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = max_data;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

int main(void) {
    struct StateMachine sm;
    initialize_state_machine(&sm, 10);
    
    struct EventData events[] = {
        {EVENT_START, "", 0},
        {EVENT_DATA_READY, "Sample data 1", 13},
        {EVENT_DATA_READY, "Sample data 2", 13},
        {EVENT_PROCESS, "", 0},
        {EVENT_SUCCESS, "", 0},
        {EVENT_RESET, "", 0}
    };
    
    for (size_t i = 0; i < sizeof(events)/sizeof(events[0]); i++) {
        if (!handle_event(&sm, &events[i])) {
            printf("Failed to handle event %zu\n", i);
            return 1;
        }
    }
    
    return 0;
}