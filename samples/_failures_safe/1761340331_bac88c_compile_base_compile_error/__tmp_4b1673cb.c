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

void state_machine_config_init(struct StateMachineConfig *config) {
    if (config == NULL) return;
    config->transition_count = 0;
    memset(config->transitions, 0, sizeof(config->transitions));
}

int state_machine_add_transition(struct StateMachineConfig *config, enum State current, enum Event event, enum State next) {
    if (config == NULL) return 0;
    if (current >= STATE_COUNT || event >= EVENT_COUNT || next >= STATE_COUNT) return 0;
    if (config->transition_count >= 32) return 0;
    config->transitions[config->transition_count].current = current;
    config->transitions[config->transition_count].event = event;
    config->transitions[config->transition_count].next = next;
    config->transition_count++;
    return 1;
}

enum State state_machine_handle_event(struct StateMachine *sm, const struct StateMachineConfig *config, enum Event event) {
    if (sm == NULL || config == NULL) return sm ? sm->current_state : STATE_ERROR;
    if (event >= EVENT_COUNT) {
        sm->current_state = STATE_ERROR;
        sm->error_code = 1;
        return STATE_ERROR;
    }
    for (size_t i = 0; i < config->transition_count; i++) {
        if (config->transitions[i].current == sm->current_state && config->transitions[i].event == event) {
            sm->current_state = config->transitions[i].next;
            break;
        }
    }
    return sm->current_state;
}

int state_machine_read_data(struct StateMachine *sm, const uint32_t *data, size_t size) {
    if (sm == NULL || data == NULL) return 0;
    if (size > 16) return 0;
    if (sm->current_state != STATE_IDLE) return 0;
    memcpy(sm->data_buffer, data, size * sizeof(uint32_t));
    sm->data_size = size;
    return 1;
}

int state_machine_process_data(struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->current_state != STATE_READING) return 0;
    if (sm->data_size == 0 || sm->data_size > 16) return 0;
    for (size_t i = 0; i < sm->data_size; i++) {
        if (sm->data_buffer[i] > UINT32_MAX - 100) {
            sm->error_code = 2;
            return 0;
        }
        sm->processed_data[i] = sm->data_buffer[i] + 42;
    }
    sm->processed_size = sm->data_size;
    return 1;
}

int state_machine_write_data(const struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->current_state != STATE_PROCESSING) return 0;
    if (sm->processed_size == 0 || sm->processed_size > 16) return 0;
    for (size_t i = 0; i < sm->processed_size; i++) {
        printf("%u ", sm->processed_data[i]);
    }
    printf("\n");
    return 1;
}

int main(void) {
    struct StateMachine sm;
    struct StateMachineConfig config;
    uint32_t test_data[] = {10, 20, 30, 40};
    size_t test_size = 4;
    
    state_machine_init(&sm);
    state_machine_config_init(&config);
    
    state_machine_add_transition(&config, STATE