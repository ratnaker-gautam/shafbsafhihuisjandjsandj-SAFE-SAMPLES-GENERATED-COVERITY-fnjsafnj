//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_COUNT
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET,
    EVENT_COUNT
};

struct StateMachine {
    enum State current_state;
    uint32_t data_value;
    uint32_t processed_value;
    uint32_t write_count;
    uint32_t error_count;
    uint32_t max_iterations;
};

struct Transition {
    enum State from_state;
    enum Event event;
    enum State to_state;
    int (*action)(struct StateMachine*);
};

int action_idle_to_reading(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    sm->data_value = 0;
    sm->processed_value = 0;
    return 0;
}

int action_reading_to_processing(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    if (sm->data_value > 1000) return -1;
    sm->data_value = (sm->data_value * 17 + 23) % 1000;
    return 0;
}

int action_processing_to_writing(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    sm->processed_value = sm->data_value * 2;
    if (sm->processed_value < sm->data_value) return -1;
    return 0;
}

int action_writing_to_idle(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    sm->write_count++;
    if (sm->write_count > sm->max_iterations) return -1;
    return 0;
}

int action_error_reset(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    sm->error_count++;
    sm->data_value = 0;
    sm->processed_value = 0;
    return 0;
}

struct Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING, action_idle_to_reading},
    {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING, action_reading_to_processing},
    {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING, action_processing_to_writing},
    {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_IDLE, action_writing_to_idle},
    {STATE_READING, EVENT_ERROR, STATE_ERROR, NULL},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR, NULL},
    {STATE_WRITING, EVENT_ERROR, STATE_ERROR, NULL},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE, action_error_reset}
};

const size_t transition_count = sizeof(transitions) / sizeof(transitions[0]);

int process_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL) return -1;
    if (event >= EVENT_COUNT) return -1;
    
    for (size_t i = 0; i < transition_count; i++) {
        if (transitions[i].from_state == sm->current_state && 
            transitions[i].event == event) {
            
            if (transitions[i].action != NULL) {
                if (transitions[i].action(sm) != 0) {
                    return -1;
                }
            }
            
            sm->current_state = transitions[i].to_state;
            return 0;
        }
    }
    
    return -1;
}

void print_state_info(const struct StateMachine* sm) {
    if (sm == NULL) return;
    
    const char* state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };
    
    printf("State: %s, Data: %u, Processed: %u, Writes: %u, Errors: %u\n",
           state_names[sm->current_state],
           sm->data_value,
           sm->processed_value,
           sm->write_count,
           sm->error_count);
}

int main(void) {
    struct StateMachine sm;
    sm.current_state = STATE_IDLE;
    sm.data_value = 0;
    sm.processed_value = 0;
    sm.write_count = 0;
    sm.error_count = 0;
    sm.max_iterations = 5;
    
    printf("State Machine Simulation\n");
    
    enum Event events[] = {
        EVENT_START,
        EVENT_DATA_READY,
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE,
        EVENT_START,
        EVENT_DATA_READY,
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE,
        EVENT_START,
        EVENT_ERROR,
        EVENT_RESET,
        EVENT_START,
        EVENT_DATA_READY,
        EVENT_PROCESS_COMPLETE,
        EVENT_WRITE_COMPLETE
    };
    
    const size_t event_count = sizeof(events) / sizeof(events[0]);
    
    for (size_t i = 0; i < event_count; i++) {
        printf("Event %zu: ", i + 1);
        
        switch (events[i]) {
            case EVENT_START: printf("START"); break;
            case EVENT_DATA_READY: printf