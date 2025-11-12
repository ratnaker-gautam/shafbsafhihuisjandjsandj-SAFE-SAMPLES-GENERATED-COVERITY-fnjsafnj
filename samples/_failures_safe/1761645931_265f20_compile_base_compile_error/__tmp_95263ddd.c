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
}

int state_machine_validate_event(enum Event event) {
    return event >= EVENT_START && event < EVENT_COUNT;
}

int state_machine_validate_state(enum State state) {
    return state >= STATE_IDLE && state < STATE_COUNT;
}

int state_machine_add_transition(struct StateMachineConfig *config, enum State from, enum Event event, enum State to) {
    if (config == NULL) return 0;
    if (!state_machine_validate_state(from) || !state_machine_validate_event(event) || !state_machine_validate_state(to)) return 0;
    if (config->transition_count >= 32) return 0;
    
    config->transitions[config->transition_count].current = from;
    config->transitions[config->transition_count].event = event;
    config->transitions[config->transition_count].next = to;
    config->transition_count++;
    return 1;
}

int state_machine_handle_event(struct StateMachine *sm, const struct StateMachineConfig *config, enum Event event) {
    if (sm == NULL || config == NULL) return 0;
    if (!state_machine_validate_event(event)) return 0;
    
    for (size_t i = 0; i < config->transition_count; i++) {
        if (config->transitions[i].current == sm->current_state && config->transitions[i].event == event) {
            sm->current_state = config->transitions[i].next;
            return 1;
        }
    }
    return 0;
}

void state_machine_simulate_reading(struct StateMachine *sm) {
    if (sm == NULL) return;
    if (sm->data_size >= 16) return;
    
    for (size_t i = 0; i < 4 && sm->data_size < 16; i++) {
        sm->data_buffer[sm->data_size] = (uint32_t)(sm->data_size * 10);
        sm->data_size++;
    }
}

void state_machine_simulate_processing(struct StateMachine *sm) {
    if (sm == NULL) return;
    if (sm->processed_size >= 16) return;
    
    sm->processed_size = 0;
    for (size_t i = 0; i < sm->data_size && sm->processed_size < 16; i++) {
        sm->processed_data[sm->processed_size] = sm->data_buffer[i] * 2;
        sm->processed_size++;
    }
}

void state_machine_simulate_writing(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    for (size_t i = 0; i < sm->processed_size && i < 16; i++) {
        printf("Output[%zu]: %u\n", i, sm->processed_data[i]);
    }
}

void state_machine_execute_state_action(struct StateMachine *sm) {
    if (sm == NULL) return;
    
    switch (sm->current_state) {
        case STATE_READING:
            state_machine_simulate_reading(sm);
            break;
        case STATE_PROCESSING:
            state_machine_simulate_processing(sm);
            break;
        case STATE_WRITING:
            state_machine_simulate_writing(sm);
            break;
        case STATE_ERROR:
            printf("Error state encountered. Code: %u\n", sm->error_code);
            break;
        default:
            break;
    }
}

const char* state_to_string(enum State state) {
    static const char* names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };
    if (state < STATE_COUNT) return names[state];
    return "UNKNOWN";
}

int main(void) {
    struct StateMachine sm;
    struct StateMachineConfig config;
    config.transition_count = 0;
    
    state_machine_init(&sm);
    
    state_machine_add_transition(&config, STATE_IDLE, EVENT_START, STATE_READING);
    state_machine_add_transition(&config, STATE_READING, EVENT_DATA_READY, STATE_PROCESSING);
    state_machine_add_transition(&config, STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING);
    state_machine_add_transition(&config, STATE_WRITING, EVENT