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

struct StateMachineConfig {
    struct Transition transitions[32];
    size_t transition_count;
};

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->processed_count = 0;
    sm->error_count = 0;
    sm->buffer_len = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
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

enum State state_machine_handle_event(struct StateMachine *sm, const struct StateMachineConfig *config, enum Event event) {
    if (sm == NULL || config == NULL) return sm ? sm->current_state : STATE_ERROR;
    if (event >= EVENT_COUNT) return sm->current_state;
    
    for (size_t i = 0; i < config->transition_count; i++) {
        if (config->transitions[i].current == sm->current_state && 
            config->transitions[i].event == event) {
            sm->current_state = config->transitions[i].next;
            break;
        }
    }
    
    return sm->current_state;
}

void state_machine_process_data(struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_PROCESSING) return;
    if (sm->buffer_len == 0 || sm->buffer_len >= sizeof(sm->buffer)) return;
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    sm->processed_count++;
}

int state_machine_read_data(struct StateMachine *sm, const char *data, size_t data_len) {
    if (sm == NULL || data == NULL || data_len == 0) return 0;
    if (sm->current_state != STATE_READING) return 0;
    if (data_len >= sizeof(sm->buffer)) return 0;
    
    memcpy(sm->buffer, data, data_len);
    sm->buffer_len = data_len;
    sm->data_count++;
    return 1;
}

int state_machine_write_data(const struct StateMachine *sm) {
    if (sm == NULL || sm->current_state != STATE_WRITING) return 0;
    if (sm->buffer_len == 0 || sm->buffer_len >= sizeof(sm->buffer)) return 0;
    
    for (size_t i = 0; i < sm->buffer_len; i++) {
        if (putchar(sm->buffer[i]) == EOF) return 0;
    }
    putchar('\n');
    return 1;
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    struct StateMachineConfig config = {
        .transitions = {
            {STATE_IDLE, EVENT_START, STATE_READING},
            {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING},
            {STATE_READING, EVENT_ERROR, STATE_ERROR},
            {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING},
            {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR},
            {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_IDLE},
            {STATE_WRITING, EVENT_ERROR, STATE_ERROR},
            {STATE_ERROR, EVENT_RESET, STATE_IDLE}
        },
        .transition_count = 8
    };
    
    if (!state_machine_validate_config(&config)) {
        return EXIT_FAILURE;
    }
    
    const char *test_data[] = {"hello", "world", "state", "machine"};
    size_t test_count = sizeof(test_data) / sizeof(test_data[0]);
    
    for (size_t i = 0; i < test_count; i++) {
        state_machine_handle_event(&sm, &config, EVENT_START);
        
        if (state_machine_read_data(&sm, test_data[i], strlen(test_data[i]))) {
            state_machine_handle