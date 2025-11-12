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
    uint32_t max_data;
    char buffer[256];
    uint32_t buffer_pos;
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
    {STATE_IDLE, EVENT_RESET, STATE_IDLE},
    {STATE_READING, EVENT_RESET, STATE_IDLE},
    {STATE_PROCESSING, EVENT_RESET, STATE_IDLE},
    {STATE_WRITING, EVENT_RESET, STATE_IDLE}
};

static const size_t transition_count = sizeof(transitions) / sizeof(transitions[0]);

void state_machine_init(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = 10;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

enum State state_machine_handle_event(struct StateMachine *sm, enum Event event) {
    if (sm == NULL) return STATE_ERROR;
    
    for (size_t i = 0; i < transition_count; i++) {
        if (transitions[i].current == sm->current_state && transitions[i].event == event) {
            sm->current_state = transitions[i].next;
            return sm->current_state;
        }
    }
    
    return sm->current_state;
}

int simulate_data_read(struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->buffer_pos >= sizeof(sm->buffer) - 1) return 0;
    
    const char *sample_data = "Sample data ";
    size_t data_len = strlen(sample_data);
    
    if (sm->buffer_pos + data_len >= sizeof(sm->buffer)) {
        return 0;
    }
    
    memcpy(sm->buffer + sm->buffer_pos, sample_data, data_len);
    sm->buffer_pos += data_len;
    sm->data_count++;
    
    return 1;
}

int simulate_data_process(struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->buffer_pos == 0) return 0;
    
    for (uint32_t i = 0; i < sm->buffer_pos; i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    
    return 1;
}

int simulate_data_write(struct StateMachine *sm) {
    if (sm == NULL) return 0;
    if (sm->buffer_pos == 0) return 0;
    
    printf("Writing: %.*s\n", (int)sm->buffer_pos, sm->buffer);
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    
    return 1;
}

void print_state_name(enum State state) {
    const char *names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };
    
    if (state < STATE_COUNT) {
        printf("%s", names[state]);
    } else {
        printf("UNKNOWN");
    }
}

void print_event_name(enum Event event) {
    const char *names[] = {
        "START", "DATA_READY", "PROCESS_COMPLETE", "WRITE_COMPLETE", "ERROR", "RESET"
    };
    
    if (event < EVENT_COUNT) {
        printf("%s", names[event]);
    } else {
        printf("UNKNOWN");
    }
}

int main(void) {
    struct StateMachine sm;
    state_machine_init(&sm);
    
    printf("State Machine Simulation Started\n");
    
    state_machine_handle_event(&sm, EVENT_START);
    printf("State: ");
    print_state_name(sm.current_state);
    printf("\n");
    
    while (sm.data_count < sm.max_data && sm.current_state != STATE_ERROR) {
        switch (sm.current_state) {
            case STATE_READING:
                if (simulate_data_read(&sm)) {
                    state_machine_handle_event(&sm, EVENT_DATA_READY);
                } else {
                    state_machine_handle_event(&sm, EVENT_ERROR);