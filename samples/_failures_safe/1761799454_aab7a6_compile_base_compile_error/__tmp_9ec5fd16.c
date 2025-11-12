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
    STATE_ERROR,
    STATE_COUNT
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET,
    EVENT_EXIT,
    EVENT_COUNT
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
    enum State current_state;
    enum Event event;
    enum State next_state;
    int (*action)(struct StateMachine*, struct EventData*);
};

static int action_start(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || ed == NULL) return -1;
    sm->data_value = 0;
    sm->process_count = 0;
    sm->error_count = 0;
    return 0;
}

static int action_data_ready(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || ed == NULL) return -1;
    if (ed->data_payload > 1000) return -1;
    sm->data_value = ed->data_payload;
    return 0;
}

static int action_process(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || ed == NULL) return -1;
    if (sm->data_value == 0) return -1;
    if (sm->process_count >= 10) return -1;
    sm->data_value *= 2;
    sm->process_count++;
    return 0;
}

static int action_success(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || ed == NULL) return -1;
    return 0;
}

static int action_failure(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || ed == NULL) return -1;
    sm->error_count++;
    return 0;
}

static int action_reset(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || ed == NULL) return -1;
    sm->data_value = 0;
    sm->process_count = 0;
    sm->error_count = 0;
    return 0;
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

static const size_t transition_count = sizeof(transitions) / sizeof(transitions[0]);

static int handle_event(struct StateMachine* sm, struct EventData* ed) {
    if (sm == NULL || ed == NULL) return -1;
    if (sm->current_state >= STATE_COUNT) return -1;
    if (ed->event_type >= EVENT_COUNT) return -1;

    for (size_t i = 0; i < transition_count; i++) {
        if (transitions[i].current_state == sm->current_state && 
            transitions[i].event == ed->event_type) {
            int result = transitions[i].action(sm, ed);
            if (result == 0) {
                sm->current_state = transitions[i].next_state;
            }
            return result;
        }
    }
    return -1;
}

static void print_state_info(const struct StateMachine* sm) {
    if (sm == NULL) return;
    const char* state_names[] = {"IDLE", "READY", "PROCESSING", "COMPLETE", "ERROR"};
    if (sm->current_state < STATE_COUNT) {
        printf("Current State: %s\n", state_names[sm->current_state]);
    } else {
        printf("Current State: INVALID\n");
    }
    printf("Data Value: %u\n", sm->data_value);
    printf("Process Count: %u\n", sm->process_count);
    printf("Error Count: %u\n", sm->error_count);
    printf("---\n");
}

int main(void) {
    struct StateMachine sm = {STATE_IDLE, 0, 0, 0};
    int running = 1;
    
    printf("State Machine Demo\n");
    printf("Available events: 0=START, 1=DATA_READY, 2=PROCESS, 3=SUCCESS, 4=FAILURE, 5=RESET, 6=EXIT\n");
    
    while (running) {
        print_state_info(&sm);
        
        printf("Enter event number: ");
        int event_input;
        if (scanf("%d", &event_input) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        if