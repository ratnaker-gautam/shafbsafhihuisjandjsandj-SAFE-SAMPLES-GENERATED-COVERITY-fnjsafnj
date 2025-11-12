//DeepSeek-V3 V2.5 Category: Safe ; Style: struct_heavy ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum State {
    STATE_IDLE,
    STATE_READY,
    STATE_PROCESSING,
    STATE_COMPLETE,
    STATE_ERROR
};

struct Event {
    int type;
    char data[32];
};

struct Context {
    enum State current_state;
    uint32_t counter;
    char buffer[64];
};

struct StateTransition {
    enum State from_state;
    int event_type;
    enum State to_state;
    int (*action)(struct Context*);
};

int action_idle_to_ready(struct Context* ctx) {
    if (ctx == NULL) return -1;
    ctx->counter = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    return 0;
}

int action_ready_to_processing(struct Context* ctx) {
    if (ctx == NULL) return -1;
    if (ctx->counter >= 1000) return -1;
    ctx->counter++;
    return 0;
}

int action_processing_to_complete(struct Context* ctx) {
    if (ctx == NULL) return -1;
    if (strlen(ctx->buffer) >= sizeof(ctx->buffer) - 1) return -1;
    snprintf(ctx->buffer + strlen(ctx->buffer), 
             sizeof(ctx->buffer) - strlen(ctx->buffer), "Processed:%u", ctx->counter);
    return 0;
}

int action_error(struct Context* ctx) {
    if (ctx == NULL) return -1;
    ctx->counter = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    strncpy(ctx->buffer, "Error occurred", sizeof(ctx->buffer) - 1);
    return 0;
}

int action_reset(struct Context* ctx) {
    if (ctx == NULL) return -1;
    ctx->counter = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    return 0;
}

struct StateMachine {
    struct Context context;
    struct StateTransition* transitions;
    size_t transition_count;
};

void init_state_machine(struct StateMachine* sm) {
    if (sm == NULL) return;
    sm->context.current_state = STATE_IDLE;
    sm->context.counter = 0;
    memset(sm->context.buffer, 0, sizeof(sm->context.buffer));
}

int process_event(struct StateMachine* sm, struct Event event) {
    if (sm == NULL) return -1;
    if (event.type < 0 || event.type > 4) return -1;

    for (size_t i = 0; i < sm->transition_count; i++) {
        struct StateTransition* trans = &sm->transitions[i];
        if (trans->from_state == sm->context.current_state && 
            trans->event_type == event.type) {
            
            if (trans->action != NULL) {
                if (trans->action(&sm->context) != 0) {
                    return -1;
                }
            }
            
            sm->context.current_state = trans->to_state;
            return 0;
        }
    }
    
    return -1;
}

int main(void) {
    struct StateTransition transitions[] = {
        {STATE_IDLE, 0, STATE_READY, action_idle_to_ready},
        {STATE_READY, 1, STATE_PROCESSING, action_ready_to_processing},
        {STATE_PROCESSING, 2, STATE_COMPLETE, action_processing_to_complete},
        {STATE_READY, 3, STATE_ERROR, action_error},
        {STATE_PROCESSING, 3, STATE_ERROR, action_error},
        {STATE_COMPLETE, 4, STATE_IDLE, action_reset},
        {STATE_ERROR, 4, STATE_IDLE, action_reset}
    };

    struct StateMachine sm;
    sm.transitions = transitions;
    sm.transition_count = sizeof(transitions) / sizeof(transitions[0]);
    init_state_machine(&sm);

    struct Event events[] = {
        {0, "Start"},
        {1, "Process"},
        {2, "Finish"},
        {4, "Reset"}
    };

    for (size_t i = 0; i < sizeof(events) / sizeof(events[0]); i++) {
        if (process_event(&sm, events[i]) == 0) {
            printf("State: %d, Counter: %u, Buffer: %s\n", 
                   sm.context.current_state, sm.context.counter, sm.context.buffer);
        } else {
            printf("Event processing failed\n");
            break;
        }
    }

    return 0;
}