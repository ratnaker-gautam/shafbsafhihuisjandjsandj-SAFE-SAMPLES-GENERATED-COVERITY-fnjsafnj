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
    uint32_t data_count;
    uint32_t processed_count;
    uint32_t error_count;
    char buffer[256];
    size_t buffer_len;
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
    {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_READING},
    {STATE_WRITING, EVENT_ERROR, STATE_ERROR},
    {STATE_ERROR, EVENT_RESET, STATE_IDLE},
    {STATE_ERROR, EVENT_START, STATE_READING}
};

static const size_t transition_count = sizeof(transitions) / sizeof(transitions[0]);

static int handle_idle(struct StateMachine *sm, enum Event event) {
    if (sm == NULL) return 0;
    if (event == EVENT_START) {
        sm->data_count = 0;
        sm->processed_count = 0;
        sm->error_count = 0;
        sm->buffer_len = 0;
        return 1;
    }
    return 0;
}

static int handle_reading(struct StateMachine *sm, enum Event event) {
    if (sm == NULL) return 0;
    if (event == EVENT_DATA_READY) {
        const char *sample_data = "Sample data chunk ";
        size_t data_len = strlen(sample_data);
        if (sm->buffer_len + data_len < sizeof(sm->buffer)) {
            memcpy(sm->buffer + sm->buffer_len, sample_data, data_len);
            sm->buffer_len += data_len;
            sm->data_count++;
            return 1;
        }
        return 0;
    }
    return 0;
}

static int handle_processing(struct StateMachine *sm, enum Event event) {
    if (sm == NULL) return 0;
    if (event == EVENT_PROCESS_COMPLETE) {
        if (sm->buffer_len > 0 && sm->data_count > 0) {
            sm->processed_count++;
            return 1;
        }
        return 0;
    }
    return 0;
}

static int handle_writing(struct StateMachine *sm, enum Event event) {
    if (sm == NULL) return 0;
    if (event == EVENT_WRITE_COMPLETE) {
        if (sm->buffer_len > 0) {
            sm->buffer_len = 0;
            return 1;
        }
        return 0;
    }
    return 0;
}

static int handle_error(struct StateMachine *sm, enum Event event) {
    if (sm == NULL) return 0;
    if (event == EVENT_RESET || event == EVENT_START) {
        sm->error_count++;
        sm->buffer_len = 0;
        return 1;
    }
    return 0;
}

static int process_event(struct StateMachine *sm, enum Event event) {
    if (sm == NULL || event >= EVENT_COUNT) return 0;
    
    for (size_t i = 0; i < transition_count; i++) {
        if (transitions[i].current == sm->current_state && transitions[i].event == event) {
            int handler_result = 0;
            switch (sm->current_state) {
                case STATE_IDLE:
                    handler_result = handle_idle(sm, event);
                    break;
                case STATE_READING:
                    handler_result = handle_reading(sm, event);
                    break;
                case STATE_PROCESSING:
                    handler_result = handle_processing(sm, event);
                    break;
                case STATE_WRITING:
                    handler_result = handle_writing(sm, event);
                    break;
                case STATE_ERROR:
                    handler_result = handle_error(sm, event);
                    break;
                default:
                    return 0;
            }
            
            if (handler_result) {
                sm->current_state = transitions[i].next;
                return 1;
            }
            return 0;
        }
    }
    return 0;
}

static void print_state_info(const struct StateMachine *sm) {
    if (sm == NULL) return;
    const char *state_names[] = {"IDLE", "READING", "PROCESSING", "WRITING", "ERROR"};
    if (sm->current_state < STATE_COUNT) {
        printf("Current state: %s\n", state_names[sm->current_state]);
    }
    printf("Data count: %u, Processed: %u, Errors: %u\n", 
           sm->data_count, sm->processed_count, sm->error_count);
    if (sm->buffer_len > 0 && sm->buffer_len < sizeof(sm->buffer)) {
        printf("Buffer: %.*s