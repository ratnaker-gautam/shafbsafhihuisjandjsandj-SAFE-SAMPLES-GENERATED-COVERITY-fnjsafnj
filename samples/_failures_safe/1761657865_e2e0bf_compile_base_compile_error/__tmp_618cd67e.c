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
    uint32_t max_process;
};

struct EventData {
    enum Event event_type;
    uint32_t event_value;
};

struct StateTransition {
    enum State from_state;
    enum Event event;
    enum State to_state;
    int (*action)(struct StateMachine*, struct EventData*);
};

static int validate_state_machine(const struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->current_state > STATE_ERROR) return 0;
    if (sm->max_process == 0) return 0;
    return 1;
}

static int validate_event_data(const struct EventData *ed) {
    if (ed == NULL) return 0;
    if (ed->event_type > EVENT_RESET) return 0;
    return 1;
}

static int action_idle_to_ready(struct StateMachine *sm, struct EventData *ed) {
    if (!validate_state_machine(sm) || !validate_event_data(ed)) return 0;
    sm->data_value = 0;
    sm->process_count = 0;
    return 1;
}

static int action_ready_to_processing(struct StateMachine *sm, struct EventData *ed) {
    if (!validate_state_machine(sm) || !validate_event_data(ed)) return 0;
    if (ed->event_value > 1000) return 0;
    sm->data_value = ed->event_value;
    return 1;
}

static int action_processing_to_complete(struct StateMachine *sm, struct EventData *ed) {
    if (!validate_state_machine(sm) || !validate_event_data(ed)) return 0;
    if (sm->process_count >= sm->max_process) return 0;
    sm->process_count++;
    return 1;
}

static int action_processing_to_error(struct StateMachine *sm, struct EventData *ed) {
    if (!validate_state_machine(sm) || !validate_event_data(ed)) return 0;
    sm->data_value = 0;
    return 1;
}

static int action_reset(struct StateMachine *sm, struct EventData *ed) {
    if (!validate_state_machine(sm) || !validate_event_data(ed)) return 0;
    sm->current_state = STATE_IDLE;
    sm->data_value = 0;
    sm->process_count = 0;
    return 1;
}

static const struct StateTransition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READY, action_idle_to_ready},
    {STATE_READY, EVENT_DATA_READY, STATE_PROCESSING, action_ready_to_processing},
    {STATE_PROCESSING, EVENT_SUCCESS, STATE_COMPLETE, action_processing_to_complete},
    {STATE_PROCESSING, EVENT_FAILURE, STATE_ERROR, action_processing_to_error},
    {STATE_COMPLETE, EVENT_RESET, STATE_IDLE, action_reset},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE, action_reset}
};

static int handle_event(struct StateMachine *sm, struct EventData *ed) {
    if (!validate_state_machine(sm) || !validate_event_data(ed)) return 0;
    
    for (size_t i = 0; i < sizeof(transitions) / sizeof(transitions[0]); i++) {
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

static void print_state_info(const struct StateMachine *sm) {
    if (!validate_state_machine(sm)) return;
    
    const char *state_names[] = {
        "IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"
    };
    
    printf("Current State: %s\n", state_names[sm->current_state]);
    printf("Data Value: %u\n", sm->data_value);
    printf("Process Count: %u/%u\n", sm->process_count, sm->max_process);
}

int main(void) {
    struct StateMachine sm = {
        .current_state = STATE_IDLE,
        .data_value = 0,
        .process_count = 0,
        .max_process = 5
    };
    
    struct EventData events[] = {
        {EVENT_START, 0},
        {EVENT_DATA_READY, 42},
        {EVENT_SUCCESS, 0},
        {EVENT_RESET, 0},
        {EVENT_START, 0},
        {EVENT_DATA_READY, 100},
        {EVENT_FAILURE, 0},
        {EVENT_RESET, 0}
    };
    
    printf("State Machine Demo\n");
    printf("==================\n");
    
    for (size_t i = 0; i < sizeof(