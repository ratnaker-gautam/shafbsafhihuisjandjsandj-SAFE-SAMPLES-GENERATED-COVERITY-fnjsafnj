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

static int handle_idle_state(struct StateMachine *sm, enum Event event);
static int handle_reading_state(struct StateMachine *sm, enum Event event);
static int handle_processing_state(struct StateMachine *sm, enum Event event);
static int handle_writing_state(struct StateMachine *sm, enum Event event);
static int handle_error_state(struct StateMachine *sm, enum Event event);

static int (*state_handlers[STATE_COUNT])(struct StateMachine *, enum Event) = {
    handle_idle_state,
    handle_reading_state,
    handle_processing_state,
    handle_writing_state,
    handle_error_state
};

static void initialize_state_machine(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
    sm->data_count = 0;
    memset(sm->processed_data, 0, sizeof(sm->processed_data));
    sm->processed_count = 0;
    sm->error_code = 0;
}

static int transition_state(struct StateMachine *sm, enum Event event) {
    if (sm == NULL || event >= EVENT_COUNT) return 0;
    
    for (size_t i = 0; i < transition_count; i++) {
        if (transitions[i].current == sm->current_state && transitions[i].event == event) {
            sm->current_state = transitions[i].next;
            return 1;
        }
    }
    return 0;
}

static int handle_idle_state(struct StateMachine *sm, enum Event event) {
    if (sm == NULL) return 0;
    
    if (event == EVENT_START) {
        printf("Starting data processing pipeline\n");
        sm->data_count = 8;
        for (size_t i = 0; i < sm->data_count; i++) {
            sm->data_buffer[i] = (uint32_t)(i * 10);
        }
        return transition_state(sm, EVENT_DATA_READY);
    }
    return 0;
}

static int handle_reading_state(struct StateMachine *sm, enum Event event) {
    if (sm == NULL) return 0;
    
    if (event == EVENT_DATA_READY) {
        printf("Data ready: ");
        for (size_t i = 0; i < sm->data_count && i < 16; i++) {
            printf("%u ", sm->data_buffer[i]);
        }
        printf("\n");
        return transition_state(sm, EVENT_PROCESS_COMPLETE);
    } else if (event == EVENT_ERROR) {
        sm->error_code = 1;
        return transition_state(sm, EVENT_ERROR);
    }
    return 0;
}

static int handle_processing_state(struct StateMachine *sm, enum Event event) {
    if (sm == NULL) return 0;
    
    if (event == EVENT_PROCESS_COMPLETE) {
        if (sm->data_count > 16) return 0;
        
        sm->processed_count = sm->data_count;
        for (size_t i = 0; i < sm->processed_count; i++) {
            sm->processed_data[i] = sm->data_buffer[i] * 2;
        }
        printf("Processing complete: ");
        for (size_t i = 0; i < sm->processed_count && i < 16; i++) {
            printf("%u ", sm->processed_data[i]);
        }
        printf("\n");
        return transition_state(sm, EVENT_WRITE_COMPLETE);
    } else if (event == EVENT_ERROR) {
        sm->error_code = 2;
        return transition_state(sm, EVENT_ERROR);
    }
    return 0;
}

static int handle_writing_state(struct StateMachine *sm, enum Event event) {
    if (sm == NULL) return 0;
    
    if (event == EVENT_WRITE_COMPLETE) {
        printf("Writing completed successfully\n");
        return transition_state(sm, EVENT_RESET);
    } else if (event == EVENT