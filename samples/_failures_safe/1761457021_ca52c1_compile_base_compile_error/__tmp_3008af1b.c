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

struct StateMachineConfig {
    struct Transition transitions[32];
    size_t transition_count;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_size = 0;
    sm->processed_size = 0;
    sm->error_code = 0;
    memset(sm->data_buffer, 0, sizeof(sm->data_buffer));
    memset(sm->processed_data, 0, sizeof(sm->processed_data));
}

int state_machine_validate_config(const struct StateMachineConfig *config) {
    if (config == NULL) return 0;
    if (config->transition_count > 32) return 0;
    for (size_t i = 0; i < config->transition_count; i++) {
        if (config->transitions[i].current >= STATE_COUNT) return 0;
        if (config->transitions[i].event >= EVENT_COUNT) return 0;
        if (config->transitions[i].next >= STATE_COUNT) return 0;
    }
    return 1;
}

int state_machine_handle_event(struct StateMachine *sm, const struct StateMachineConfig *config, enum Event event) {
    if (sm == NULL || config == NULL) return 0;
    if (event >= EVENT_COUNT) return 0;
    
    for (size_t i = 0; i < config->transition_count; i++) {
        if (config->transitions[i].current == sm->current_state && config->transitions[i].event == event) {
            sm->current_state = config->transitions[i].next;
            return 1;
        }
    }
    return 0;
}

void state_machine_process_data(struct StateMachine *sm) {
    if (sm == NULL) return;
    if (sm->current_state != STATE_PROCESSING) return;
    if (sm->data_size == 0 || sm->data_size > 16) return;
    
    sm->processed_size = sm->data_size;
    for (size_t i = 0; i < sm->data_size; i++) {
        if (sm->data_buffer[i] > UINT32_MAX - 100) {
            sm->error_code = 1;
            return;
        }
        sm->processed_data[i] = sm->data_buffer[i] + 42;
    }
}

void state_machine_simulate_reading(struct StateMachine *sm) {
    if (sm == NULL) return;
    if (sm->current_state != STATE_READING) return;
    
    sm->data_size = 8;
    for (size_t i = 0; i < sm->data_size; i++) {
        sm->data_buffer[i] = (uint32_t)(i * 10);
    }
}

void state_machine_simulate_writing(struct StateMachine *sm) {
    if (sm == NULL) return;
    if (sm->current_state != STATE_WRITING) return;
    
    sm->processed_size = 0;
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    struct StateMachineConfig config = {
        .transitions = {
            {STATE_IDLE, EVENT_START, STATE_READING},
            {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING},
            {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING},
            {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_IDLE},
            {STATE_READING, EVENT_ERROR, STATE_ERROR},
            {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR},
            {STATE_WRITING, EVENT_ERROR, STATE_ERROR},
            {STATE_ERROR, EVENT_RESET, STATE_IDLE}
        },
        .transition_count = 8
    };
    
    if (!state_machine_validate_config(&config)) {
        printf("Invalid state machine configuration\n");
        return 1;
    }
    
    printf("Starting state machine simulation\n");
    
    if (!state_machine_handle_event(&sm, &config, EVENT_START)) {
        printf("Failed to handle START event\n");
        return 1;
    }
    printf("State: READING\n");
    
    state_machine_simulate_reading(&sm);
    if (!state_machine_handle_event(&sm, &config, EVENT_DATA_READY)) {
        printf("Failed to handle DATA_READY event\n");
        return 1;
    }
    printf("State: PROCESSING\n");
    
    state_machine_process_data(&sm);
    if (sm.error_code != 0) {
        if (!state_machine_handle_event(&sm, &config, EVENT_ERROR)) {
            printf("Failed to