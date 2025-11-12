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

struct StateHandler {
    enum State state;
    int (*handler)(struct StateMachine*);
};

int handle_idle(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    sm->data_count = 0;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    printf("State: IDLE - Ready to start\n");
    return 0;
}

int handle_reading(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    if (sm->data_count >= sm->max_data) return -1;
    
    printf("Enter data (max %u chars): ", (unsigned int)(sizeof(sm->buffer) - sm->buffer_pos - 1));
    if (fgets(sm->buffer + sm->buffer_pos, sizeof(sm->buffer) - sm->buffer_pos, stdin) == NULL) {
        return -1;
    }
    
    size_t len = strlen(sm->buffer + sm->buffer_pos);
    if (len > 0 && sm->buffer[sm->buffer_pos + len - 1] == '\n') {
        sm->buffer[sm->buffer_pos + len - 1] = '\0';
    }
    
    sm->buffer_pos += (uint32_t)len;
    sm->data_count++;
    printf("Read data: %s (count: %u)\n", sm->buffer, sm->data_count);
    return 0;
}

int handle_processing(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    if (sm->buffer_pos == 0) return -1;
    
    for (uint32_t i = 0; i < sm->buffer_pos && i < sizeof(sm->buffer); i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    printf("Processed data: %s\n", sm->buffer);
    return 0;
}

int handle_writing(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    if (sm->buffer_pos == 0) return -1;
    
    printf("Writing data: %s\n", sm->buffer);
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
    return 0;
}

int handle_error(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    printf("State: ERROR - An error occurred\n");
    return 0;
}

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

static const struct StateHandler handlers[] = {
    {STATE_IDLE, handle_idle},
    {STATE_READING, handle_reading},
    {STATE_PROCESSING, handle_processing},
    {STATE_WRITING, handle_writing},
    {STATE_ERROR, handle_error}
};

void init_state_machine(struct StateMachine* sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->data_count = 0;
    sm->max_data = 5;
    sm->buffer_pos = 0;
    memset(sm->buffer, 0, sizeof(sm->buffer));
}

enum State get_next_state(enum State current, enum Event event) {
    for (size_t i = 0; i < sizeof(transitions) / sizeof(transitions[0]); i++) {
        if (transitions[i].current == current && transitions[i].event == event) {
            return transitions[i].next;
        }
    }
    return current;
}

int execute_state_handler(struct StateMachine* sm) {
    if (sm == NULL) return -1;
    for (size_t i = 0; i < sizeof(handlers) / sizeof(handlers[0]); i++) {
        if (handlers[i].state == sm->current_state) {
            return handlers[i].handler(sm);
        }
    }
    return -1;
}

int process_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL) return -