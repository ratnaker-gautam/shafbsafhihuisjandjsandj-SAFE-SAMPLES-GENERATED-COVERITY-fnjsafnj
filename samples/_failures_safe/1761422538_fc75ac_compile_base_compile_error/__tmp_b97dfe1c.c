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
    uint32_t data_buffer[16];
    size_t data_count;
    uint32_t processed_data[16];
    size_t processed_count;
    uint32_t error_code;
};

struct Transition {
    enum State current_state;
    enum Event event;
    enum State next_state;
    int (*action)(struct StateMachine*);
};

int action_idle_start(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    sm->data_count = 0;
    sm->processed_count = 0;
    sm->error_code = 0;
    printf("State machine initialized\n");
    return 0;
}

int action_reading_data_ready(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    if (sm->data_count >= 16) return -1;
    sm->data_buffer[sm->data_count] = sm->data_count * 10;
    sm->data_count++;
    printf("Data %u added to buffer\n", (unsigned int)(sm->data_count * 10));
    return 0;
}

int action_processing_complete(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    if (sm->data_count == 0) return -1;
    if (sm->processed_count >= 16) return -1;
    sm->processed_data[sm->processed_count] = sm->data_buffer[sm->data_count - 1] * 2;
    sm->data_count--;
    sm->processed_count++;
    printf("Data processed: %u -> %u\n", 
           (unsigned int)(sm->processed_data[sm->processed_count - 1] / 2),
           (unsigned int)sm->processed_data[sm->processed_count - 1]);
    return 0;
}

int action_writing_complete(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    if (sm->processed_count == 0) return -1;
    printf("Data written: %u\n", (unsigned int)sm->processed_data[sm->processed_count - 1]);
    sm->processed_count--;
    return 0;
}

int action_error(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    sm->error_code = 1;
    printf("Error occurred in state machine\n");
    return 0;
}

int action_reset(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->processed_count = 0;
    sm->error_code = 0;
    printf("State machine reset\n");
    return 0;
}

struct Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING, action_idle_start},
    {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING, action_reading_data_ready},
    {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING, action_processing_complete},
    {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_READING, action_writing_complete},
    {STATE_READING, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_WRITING, EVENT_ERROR, STATE_ERROR, action_error},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE, action_reset}
};

const size_t transition_count = sizeof(transitions) / sizeof(transitions[0]);

int process_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL) return -1;
    if (event >= EVENT_COUNT) return -1;
    
    for (size_t i = 0; i < transition_count; i++) {
        if (transitions[i].current_state == sm->current_state && transitions[i].event == event) {
            if (transitions[i].action != NULL) {
                int result = transitions[i].action(sm);
                if (result != 0) return result;
            }
            sm->current_state = transitions[i].next_state;
            return 0;
        }
    }
    return -1;
}

const char* state_to_string(enum State state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_READING: return "READING";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_WRITING: return "WRITING";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

const char* event_to_string(enum Event event) {
    switch (event) {
        case EVENT_START: return "START";
        case EVENT_DATA_READY: return "DATA_READY";
        case EVENT_PROCESS_COMPLETE: return "PROCESS_COMPLETE";
        case EVENT_WRITE_COMPLETE: return "WRITE_COMPLETE";
        case