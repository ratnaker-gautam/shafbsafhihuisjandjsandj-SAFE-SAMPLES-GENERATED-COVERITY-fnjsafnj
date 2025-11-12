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
    size_t data_size;
    uint32_t processed_data[16];
    size_t processed_size;
    uint32_t error_code;
};

struct Transition {
    enum State current;
    enum Event event;
    enum State next;
};

static const struct Transition transitions[] = {
    {STATE_IDLE, EVENT_START, STATE_READING},
    {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING},
    {STATE_READING, EVENT_ERROR, STATE_ERROR},
    {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING},
    {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR},
    {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_IDLE},
    {STATE_WRITING, EVENT_ERROR, STATE_ERROR},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE}
};

static const size_t transition_count = sizeof(transitions) / sizeof(transitions[0]);

static void initialize_state_machine(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_size = 0;
    sm->processed_size = 0;
    sm->error_code = 0;
    memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
    memset(sm->processed_data, 0, sizeof(sm->processed_data));
}

static int validate_transition(enum State current, enum Event event) {
    for (size_t i = 0; i < transition_count; i++) {
        if (transitions[i].current == current && transitions[i].event == event) {
            return 1;
        }
    }
    return 0;
}

static enum State get_next_state(enum State current, enum Event event) {
    for (size_t i = 0; i < transition_count; i++) {
        if (transitions[i].current == current && transitions[i].event == event) {
            return transitions[i].next;
        }
    }
    return current;
}

static int read_data(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_READING) return 0;
    
    sm->data_size = 8;
    for (size_t i = 0; i < sm->data_size; i++) {
        sm->data_buffer[i] = (uint32_t)(i * 10 + 5);
    }
    return 1;
}

static int process_data(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_PROCESSING) return 0;
    if (sm->data_size == 0 || sm->data_size > 16) return 0;
    
    sm->processed_size = sm->data_size;
    for (size_t i = 0; i < sm->processed_size; i++) {
        sm->processed_data[i] = sm->data_buffer[i] * 2;
    }
    return 1;
}

static int write_data(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_WRITING) return 0;
    if (sm->processed_size == 0 || sm->processed_size > 16) return 0;
    
    for (size_t i = 0; i < sm->processed_size; i++) {
        printf("Output[%zu]: %u\n", i, sm->processed_data[i]);
    }
    return 1;
}

static int handle_event(struct StateMachine *sm, enum Event event) {
    if (sm == NULL) return 0;
    if (!validate_transition(sm->current_state, event)) return 0;
    
    enum State next_state = get_next_state(sm->current_state, event);
    
    switch (event) {
        case EVENT_START:
            if (next_state == STATE_READING) {
                sm->current_state = STATE_READING;
            }
            break;
            
        case EVENT_DATA_READY:
            if (next_state == STATE_PROCESSING && read_data(sm)) {
                sm->current_state = STATE_PROCESSING;
            } else {
                sm->current_state = STATE_ERROR;
                sm->error_code = 1;
            }
            break;
            
        case EVENT_PROCESS_COMPLETE:
            if (next_state == STATE_WRITING && process_data(sm)) {
                sm->current_state = STATE_WRITING;
            } else {
                sm->current_state = STATE_ERROR;
                sm->error_code = 2;
            }
            break;
            
        case EVENT_WRITE_COMPLETE:
            if (next_state == STATE_IDLE && write_data(sm)) {
                sm->current_state = STATE_IDLE;
            } else {
                sm->current_state = STATE_ERROR;
                sm->error_code = 3;
            }
            break;
            
        case EVENT_ERROR:
            sm->current_state = STATE_ERROR;
            sm->error_code = 4;
            break;
            
        case EVENT