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
    uint32_t processed_value;
    uint32_t write_count;
};

struct Transition {
    enum State next_state;
    int (*action)(struct StateMachine*);
};

static const char* state_names[] = {
    "IDLE",
    "READING",
    "PROCESSING",
    "WRITING",
    "ERROR"
};

static const char* event_names[] = {
    "START",
    "DATA_READY",
    "PROCESS_COMPLETE",
    "WRITE_COMPLETE",
    "ERROR",
    "RESET"
};

static int action_idle_to_reading(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    sm->data_count = 0;
    for (size_t i = 0; i < 16; i++) {
        sm->data_buffer[i] = (uint32_t)(i * 10 + 5);
    }
    sm->data_count = 16;
    return 1;
}

static int action_reading_to_processing(struct StateMachine* sm) {
    if (sm == NULL || sm->data_count == 0) return 0;
    return 1;
}

static int action_processing_to_writing(struct StateMachine* sm) {
    if (sm == NULL || sm->data_count == 0) return 0;
    sm->processed_value = 0;
    for (size_t i = 0; i < sm->data_count; i++) {
        if (sm->processed_value > UINT32_MAX - sm->data_buffer[i]) {
            return 0;
        }
        sm->processed_value += sm->data_buffer[i];
    }
    sm->write_count = 0;
    return 1;
}

static int action_writing_to_idle(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    printf("Processed value: %u\n", sm->processed_value);
    sm->write_count++;
    return 1;
}

static int action_error(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    printf("State machine entered error state\n");
    return 1;
}

static int action_reset(struct StateMachine* sm) {
    if (sm == NULL) return 0;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->processed_value = 0;
    sm->write_count = 0;
    return 1;
}

static struct Transition transition_table[STATE_COUNT][EVENT_COUNT] = {
    [STATE_IDLE] = {
        [EVENT_START] = {STATE_READING, action_idle_to_reading},
        [EVENT_RESET] = {STATE_IDLE, action_reset}
    },
    [STATE_READING] = {
        [EVENT_DATA_READY] = {STATE_PROCESSING, action_reading_to_processing},
        [EVENT_ERROR] = {STATE_ERROR, action_error},
        [EVENT_RESET] = {STATE_IDLE, action_reset}
    },
    [STATE_PROCESSING] = {
        [EVENT_PROCESS_COMPLETE] = {STATE_WRITING, action_processing_to_writing},
        [EVENT_ERROR] = {STATE_ERROR, action_error},
        [EVENT_RESET] = {STATE_IDLE, action_reset}
    },
    [STATE_WRITING] = {
        [EVENT_WRITE_COMPLETE] = {STATE_IDLE, action_writing_to_idle},
        [EVENT_ERROR] = {STATE_ERROR, action_error},
        [EVENT_RESET] = {STATE_IDLE, action_reset}
    },
    [STATE_ERROR] = {
        [EVENT_RESET] = {STATE_IDLE, action_reset}
    }
};

static void state_machine_init(struct StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->processed_value = 0;
    sm->write_count = 0;
}

static int state_machine_handle_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL || event >= EVENT_COUNT || sm->current_state >= STATE_COUNT) {
        return 0;
    }
    
    struct Transition transition = transition_table[sm->current_state][event];
    if (transition.next_state == sm->current_state && transition.action == NULL) {
        return 0;
    }
    
    if (transition.action != NULL) {
        if (!transition.action(sm)) {
            return 0;
        }
    }
    
    sm->current_state = transition.next_state;
    return 1;
}

static void print_state_info(const struct StateMachine* sm) {
    if (sm == NULL || sm->current_state >= STATE_COUNT) return;
    printf("Current state: %s\n", state_names[sm->current_state]);
    printf("Data count: %