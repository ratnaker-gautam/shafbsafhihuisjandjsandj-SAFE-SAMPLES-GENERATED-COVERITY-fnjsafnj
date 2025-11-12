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

struct StateTransition {
    enum State current_state;
    enum Event event;
    enum State next_state;
    int (*action)(void*);
};

struct StateMachine {
    enum State current_state;
    struct StateTransition* transitions;
    size_t transition_count;
    void* context;
};

struct MachineContext {
    char buffer[256];
    size_t buffer_size;
    int data_value;
    int processed_value;
    int error_code;
};

int action_idle_start(void* ctx) {
    struct MachineContext* context = (struct MachineContext*)ctx;
    if (context == NULL) return -1;
    printf("Starting state machine\n");
    context->data_value = 0;
    context->processed_value = 0;
    context->error_code = 0;
    memset(context->buffer, 0, sizeof(context->buffer));
    context->buffer_size = 0;
    return 0;
}

int action_reading_data_ready(void* ctx) {
    struct MachineContext* context = (struct MachineContext*)ctx;
    if (context == NULL) return -1;
    if (context->buffer_size >= sizeof(context->buffer)) return -1;
    context->data_value = 42;
    snprintf(context->buffer, sizeof(context->buffer), "Data: %d", context->data_value);
    context->buffer_size = strlen(context->buffer);
    printf("Read data: %s\n", context->buffer);
    return 0;
}

int action_processing_complete(void* ctx) {
    struct MachineContext* context = (struct MachineContext*)ctx;
    if (context == NULL) return -1;
    context->processed_value = context->data_value * 2;
    printf("Processed value: %d\n", context->processed_value);
    return 0;
}

int action_writing_complete(void* ctx) {
    struct MachineContext* context = (struct MachineContext*)ctx;
    if (context == NULL) return -1;
    printf("Writing completed: %s\n", context->buffer);
    return 0;
}

int action_error(void* ctx) {
    struct MachineContext* context = (struct MachineContext*)ctx;
    if (context == NULL) return -1;
    context->error_code = 1;
    printf("Error occurred in state machine\n");
    return 0;
}

int action_reset(void* ctx) {
    struct MachineContext* context = (struct MachineContext*)ctx;
    if (context == NULL) return -1;
    context->data_value = 0;
    context->processed_value = 0;
    context->error_code = 0;
    memset(context->buffer, 0, sizeof(context->buffer));
    context->buffer_size = 0;
    printf("State machine reset\n");
    return 0;
}

void init_state_machine(struct StateMachine* sm, struct StateTransition* transitions, size_t count, struct MachineContext* ctx) {
    if (sm == NULL || transitions == NULL || ctx == NULL) return;
    sm->current_state = STATE_IDLE;
    sm->transitions = transitions;
    sm->transition_count = count;
    sm->context = ctx;
}

int process_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL || event >= EVENT_COUNT) return -1;
    
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current_state == sm->current_state && 
            sm->transitions[i].event == event) {
            
            if (sm->transitions[i].action != NULL) {
                int result = sm->transitions[i].action(sm->context);
                if (result != 0) return result;
            }
            
            sm->current_state = sm->transitions[i].next_state;
            return 0;
        }
    }
    return -1;
}

int main(void) {
    struct StateTransition transitions[] = {
        {STATE_IDLE, EVENT_START, STATE_READING, action_idle_start},
        {STATE_READING, EVENT_DATA_READY, STATE_PROCESSING, action_reading_data_ready},
        {STATE_PROCESSING, EVENT_PROCESS_COMPLETE, STATE_WRITING, action_processing_complete},
        {STATE_WRITING, EVENT_WRITE_COMPLETE, STATE_IDLE, action_writing_complete},
        {STATE_IDLE, EVENT_ERROR, STATE_ERROR, action_error},
        {STATE_READING, EVENT_ERROR, STATE_ERROR, action_error},
        {STATE_PROCESSING, EVENT_ERROR, STATE_ERROR, action_error},
        {STATE_WRITING, EVENT_ERROR, STATE_ERROR, action_error},
        {STATE_ERROR, EVENT_RESET, STATE_IDLE, action_reset}
    };
    
    struct MachineContext context;
    struct StateMachine state_machine;
    
    init_state_machine(&state_machine, transitions, 
                      sizeof(transitions) / sizeof(transitions[0]), &context);
    
    printf("State Machine Demo\n");
    
    if (process_event(&state_machine, EVENT_START) == 0