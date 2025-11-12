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
    enum State current_state;
    enum Event event;
    enum State next_state;
    int (*action)(void*);
};

struct StateMachine {
    enum State current_state;
    struct Transition* transitions;
    size_t transition_count;
    void* context;
};

struct Context {
    char buffer[256];
    size_t buffer_size;
    int data_value;
    int processed_value;
    int write_count;
};

int action_idle_start(void* ctx) {
    struct Context* context = (struct Context*)ctx;
    if (context == NULL) return -1;
    printf("Starting state machine\n");
    context->data_value = 0;
    context->processed_value = 0;
    context->write_count = 0;
    return 0;
}

int action_reading_data_ready(void* ctx) {
    struct Context* context = (struct Context*)ctx;
    if (context == NULL) return -1;
    printf("Enter data value: ");
    if (scanf("%d", &context->data_value) != 1) {
        return -1;
    }
    return 0;
}

int action_processing_complete(void* ctx) {
    struct Context* context = (struct Context*)ctx;
    if (context == NULL) return -1;
    context->processed_value = context->data_value * 2;
    printf("Processed value: %d\n", context->processed_value);
    return 0;
}

int action_writing_complete(void* ctx) {
    struct Context* context = (struct Context*)ctx;
    if (context == NULL) return -1;
    context->write_count++;
    printf("Write count: %d\n", context->write_count);
    return 0;
}

int action_error(void* ctx) {
    struct Context* context = (struct Context*)ctx;
    if (context == NULL) return -1;
    printf("Error occurred\n");
    return 0;
}

int action_reset(void* ctx) {
    struct Context* context = (struct Context*)ctx;
    if (context == NULL) return -1;
    printf("Resetting state machine\n");
    memset(context, 0, sizeof(struct Context));
    return 0;
}

int state_machine_init(struct StateMachine* sm, struct Transition* trans, size_t trans_count, void* ctx) {
    if (sm == NULL || trans == NULL || ctx == NULL) return -1;
    sm->current_state = STATE_IDLE;
    sm->transitions = trans;
    sm->transition_count = trans_count;
    sm->context = ctx;
    return 0;
}

int state_machine_handle_event(struct StateMachine* sm, enum Event event) {
    if (sm == NULL) return -1;
    for (size_t i = 0; i < sm->transition_count; i++) {
        if (sm->transitions[i].current_state == sm->current_state && sm->transitions[i].event == event) {
            if (sm->transitions[i].action != NULL) {
                int result = sm->transitions[i].action(sm->context);
                if (result != 0) {
                    return result;
                }
            }
            sm->current_state = sm->transitions[i].next_state;
            return 0;
        }
    }
    return -1;
}

void print_menu(void) {
    printf("\nAvailable events:\n");
    printf("0 - START\n");
    printf("1 - DATA_READY\n");
    printf("2 - PROCESS_COMPLETE\n");
    printf("3 - WRITE_COMPLETE\n");
    printf("4 - ERROR\n");
    printf("5 - RESET\n");
    printf("6 - EXIT\n");
    printf("Choose event: ");
}

int main(void) {
    struct Context context;
    memset(&context, 0, sizeof(context));
    
    struct Transition transitions[] = {
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
    
    struct StateMachine sm;
    if (state_machine_init(&sm, transitions, sizeof(transitions)/sizeof(transitions[0]), &context) != 0) {
        return 1;
    }
    
    int running = 1;
    while (running) {
        printf("\nCurrent state: ");
        switch (sm.current_state) {
            case STATE_IDLE: printf("ID