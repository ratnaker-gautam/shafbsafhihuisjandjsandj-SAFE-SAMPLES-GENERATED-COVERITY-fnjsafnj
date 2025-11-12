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

struct Transition {
    enum State next_state;
    int (*action)(void*);
};

struct StateMachine {
    enum State current_state;
    struct Transition transitions[STATE_COUNT][EVENT_COUNT];
    char buffer[256];
    size_t buffer_size;
    int error_code;
};

int action_idle_start(void* ctx) {
    struct StateMachine* sm = (struct StateMachine*)ctx;
    if (!sm) return -1;
    printf("Starting data processing\n");
    strncpy(sm->buffer, "Sample data", sizeof(sm->buffer) - 1);
    sm->buffer[sizeof(sm->buffer) - 1] = '\0';
    sm->buffer_size = strlen(sm->buffer);
    return 0;
}

int action_reading_data_ready(void* ctx) {
    struct StateMachine* sm = (struct StateMachine*)ctx;
    if (!sm) return -1;
    printf("Data ready: %s\n", sm->buffer);
    return 0;
}

int action_processing_complete(void* ctx) {
    struct StateMachine* sm = (struct StateMachine*)ctx;
    if (!sm) return -1;
    for (size_t i = 0; i < sm->buffer_size && i < sizeof(sm->buffer); i++) {
        if (sm->buffer[i] >= 'a' && sm->buffer[i] <= 'z') {
            sm->buffer[i] = sm->buffer[i] - 'a' + 'A';
        }
    }
    printf("Processing complete: %s\n", sm->buffer);
    return 0;
}

int action_writing_complete(void* ctx) {
    struct StateMachine* sm = (struct StateMachine*)ctx;
    if (!sm) return -1;
    printf("Data written successfully\n");
    return 0;
}

int action_error(void* ctx) {
    struct StateMachine* sm = (struct StateMachine*)ctx;
    if (!sm) return -1;
    sm->error_code = 1;
    printf("Error occurred in state machine\n");
    return 0;
}

int action_reset(void* ctx) {
    struct StateMachine* sm = (struct StateMachine*)ctx;
    if (!sm) return -1;
    sm->current_state = STATE_IDLE;
    sm->buffer[0] = '\0';
    sm->buffer_size = 0;
    sm->error_code = 0;
    printf("State machine reset\n");
    return 0;
}

void initialize_state_machine(struct StateMachine* sm) {
    if (!sm) return;
    
    sm->current_state = STATE_IDLE;
    sm->buffer[0] = '\0';
    sm->buffer_size = 0;
    sm->error_code = 0;
    
    for (int s = 0; s < STATE_COUNT; s++) {
        for (int e = 0; e < EVENT_COUNT; e++) {
            sm->transitions[s][e].next_state = STATE_ERROR;
            sm->transitions[s][e].action = action_error;
        }
    }
    
    sm->transitions[STATE_IDLE][EVENT_START].next_state = STATE_READING;
    sm->transitions[STATE_IDLE][EVENT_START].action = action_idle_start;
    
    sm->transitions[STATE_READING][EVENT_DATA_READY].next_state = STATE_PROCESSING;
    sm->transitions[STATE_READING][EVENT_DATA_READY].action = action_reading_data_ready;
    
    sm->transitions[STATE_PROCESSING][EVENT_PROCESS_COMPLETE].next_state = STATE_WRITING;
    sm->transitions[STATE_PROCESSING][EVENT_PROCESS_COMPLETE].action = action_processing_complete;
    
    sm->transitions[STATE_WRITING][EVENT_WRITE_COMPLETE].next_state = STATE_IDLE;
    sm->transitions[STATE_WRITING][EVENT_WRITE_COMPLETE].action = action_writing_complete;
    
    sm->transitions[STATE_ERROR][EVENT_RESET].next_state = STATE_IDLE;
    sm->transitions[STATE_ERROR][EVENT_RESET].action = action_reset;
    
    for (int s = 0; s < STATE_COUNT; s++) {
        sm->transitions[s][EVENT_RESET].next_state = STATE_IDLE;
        sm->transitions[s][EVENT_RESET].action = action_reset;
    }
}

int process_event(struct StateMachine* sm, enum Event event) {
    if (!sm || event < 0 || event >= EVENT_COUNT) {
        return -1;
    }
    
    struct Transition* transition = &sm->transitions[sm->current_state][event];
    
    if (transition->action) {
        int result = transition->action(sm);
        if (result != 0) {
            return result;
        }
    }
    
    sm->current_state = transition->next_state;
    return 0;
}

const char