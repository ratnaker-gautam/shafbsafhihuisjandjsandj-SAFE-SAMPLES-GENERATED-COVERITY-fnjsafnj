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

struct Context {
    char buffer[256];
    size_t buffer_size;
    int data_value;
    int processed_value;
    int write_count;
};

struct StateMachine {
    enum State current_state;
    struct Transition transitions[STATE_COUNT][EVENT_COUNT];
    void* context;
};

int action_idle_start(void* ctx) {
    struct Context* context = (struct Context*)ctx;
    printf("Starting data processing\n");
    context->data_value = 0;
    context->processed_value = 0;
    context->write_count = 0;
    return 0;
}

int action_reading_data_ready(void* ctx) {
    struct Context* context = (struct Context*)ctx;
    printf("Enter data value (0-100): ");
    if (scanf("%d", &context->data_value) != 1) {
        return -1;
    }
    if (context->data_value < 0 || context->data_value > 100) {
        return -1;
    }
    printf("Read value: %d\n", context->data_value);
    return 0;
}

int action_processing_complete(void* ctx) {
    struct Context* context = (struct Context*)ctx;
    context->processed_value = context->data_value * 2;
    if (context->processed_value < 0) {
        return -1;
    }
    printf("Processed value: %d\n", context->processed_value);
    return 0;
}

int action_writing_complete(void* ctx) {
    struct Context* context = (struct Context*)ctx;
    context->write_count++;
    printf("Writing value: %d (write count: %d)\n", context->processed_value, context->write_count);
    return 0;
}

int action_error(void* ctx) {
    printf("Error occurred, resetting\n");
    return 0;
}

int action_reset(void* ctx) {
    struct Context* context = (struct Context*)ctx;
    context->data_value = 0;
    context->processed_value = 0;
    context->write_count = 0;
    printf("State machine reset\n");
    return 0;
}

void initialize_state_machine(struct StateMachine* sm, struct Context* ctx) {
    sm->current_state = STATE_IDLE;
    sm->context = ctx;
    
    for (int i = 0; i < STATE_COUNT; i++) {
        for (int j = 0; j < EVENT_COUNT; j++) {
            sm->transitions[i][j].next_state = STATE_ERROR;
            sm->transitions[i][j].action = NULL;
        }
    }
    
    sm->transitions[STATE_IDLE][EVENT_START].next_state = STATE_READING;
    sm->transitions[STATE_IDLE][EVENT_START].action = action_idle_start;
    
    sm->transitions[STATE_READING][EVENT_DATA_READY].next_state = STATE_PROCESSING;
    sm->transitions[STATE_READING][EVENT_DATA_READY].action = action_reading_data_ready;
    
    sm->transitions[STATE_PROCESSING][EVENT_PROCESS_COMPLETE].next_state = STATE_WRITING;
    sm->transitions[STATE_PROCESSING][EVENT_PROCESS_COMPLETE].action = action_processing_complete;
    
    sm->transitions[STATE_WRITING][EVENT_WRITE_COMPLETE].next_state = STATE_READING;
    sm->transitions[STATE_WRITING][EVENT_WRITE_COMPLETE].action = action_writing_complete;
    
    sm->transitions[STATE_ERROR][EVENT_RESET].next_state = STATE_IDLE;
    sm->transitions[STATE_ERROR][EVENT_RESET].action = action_reset;
    
    for (int i = 0; i < STATE_COUNT; i++) {
        sm->transitions[i][EVENT_ERROR].next_state = STATE_ERROR;
        sm->transitions[i][EVENT_ERROR].action = action_error;
    }
}

int process_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL) {
        return -1;
    }
    if (event < 0 || event >= EVENT_COUNT) {
        return -1;
    }
    if (sm->current_state < 0 || sm->current_state >= STATE_COUNT) {
        return -1;
    }
    
    struct Transition* transition = &sm->transitions[sm->current_state][event];
    
    if (transition->action != NULL) {
        int result = transition->action(sm->context);
        if (result != 0) {
            return -1;
        }
    }
    
    sm->current_state = transition->next_state;
    return 0;
}

void print_state(enum State state) {
    const char* state_names[] = {
        "IDLE", "READING", "PROCESSING", "WRITING", "ERROR"
    };