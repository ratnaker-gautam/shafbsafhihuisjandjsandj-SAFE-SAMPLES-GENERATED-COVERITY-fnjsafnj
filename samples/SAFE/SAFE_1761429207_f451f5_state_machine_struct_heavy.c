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

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS,
    EVENT_SUCCESS,
    EVENT_FAILURE,
    EVENT_RESET
};

struct Context {
    uint32_t data_size;
    uint8_t *data_buffer;
    uint32_t processed_count;
    uint32_t error_count;
};

struct StateMachine {
    enum State current_state;
    struct Context context;
};

void initialize_context(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->data_size = 0;
    ctx->data_buffer = NULL;
    ctx->processed_count = 0;
    ctx->error_count = 0;
}

void cleanup_context(struct Context *ctx) {
    if (ctx == NULL) return;
    if (ctx->data_buffer != NULL) {
        free(ctx->data_buffer);
        ctx->data_buffer = NULL;
    }
    ctx->data_size = 0;
}

int allocate_data_buffer(struct Context *ctx, uint32_t size) {
    if (ctx == NULL || size == 0 || size > 1024) return 0;
    if (ctx->data_buffer != NULL) free(ctx->data_buffer);
    ctx->data_buffer = malloc(size);
    if (ctx->data_buffer == NULL) return 0;
    ctx->data_size = size;
    memset(ctx->data_buffer, 0, size);
    return 1;
}

void initialize_state_machine(struct StateMachine *sm) {
    if (sm == NULL) return;
    sm->current_state = STATE_IDLE;
    initialize_context(&sm->context);
}

void cleanup_state_machine(struct StateMachine *sm) {
    if (sm == NULL) return;
    cleanup_context(&sm->context);
}

const char* state_to_string(enum State s) {
    switch (s) {
        case STATE_IDLE: return "IDLE";
        case STATE_READY: return "READY";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_COMPLETE: return "COMPLETE";
        case STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

const char* event_to_string(enum Event e) {
    switch (e) {
        case EVENT_START: return "START";
        case EVENT_DATA_READY: return "DATA_READY";
        case EVENT_PROCESS: return "PROCESS";
        case EVENT_SUCCESS: return "SUCCESS";
        case EVENT_FAILURE: return "FAILURE";
        case EVENT_RESET: return "RESET";
        default: return "UNKNOWN";
    }
}

int handle_event(struct StateMachine *sm, enum Event event) {
    if (sm == NULL) return 0;

    enum State next_state = sm->current_state;

    switch (sm->current_state) {
        case STATE_IDLE:
            if (event == EVENT_START) {
                if (allocate_data_buffer(&sm->context, 64)) {
                    next_state = STATE_READY;
                } else {
                    next_state = STATE_ERROR;
                }
            }
            break;

        case STATE_READY:
            if (event == EVENT_DATA_READY) {
                next_state = STATE_PROCESSING;
            } else if (event == EVENT_RESET) {
                cleanup_context(&sm->context);
                initialize_context(&sm->context);
                next_state = STATE_IDLE;
            }
            break;

        case STATE_PROCESSING:
            if (event == EVENT_SUCCESS) {
                sm->context.processed_count++;
                next_state = STATE_COMPLETE;
            } else if (event == EVENT_FAILURE) {
                sm->context.error_count++;
                next_state = STATE_ERROR;
            }
            break;

        case STATE_COMPLETE:
            if (event == EVENT_RESET) {
                cleanup_context(&sm->context);
                initialize_context(&sm->context);
                next_state = STATE_IDLE;
            } else if (event == EVENT_PROCESS) {
                next_state = STATE_PROCESSING;
            }
            break;

        case STATE_ERROR:
            if (event == EVENT_RESET) {
                cleanup_context(&sm->context);
                initialize_context(&sm->context);
                next_state = STATE_IDLE;
            }
            break;

        default:
            next_state = STATE_ERROR;
            break;
    }

    printf("State: %s -> %s (Event: %s)\n",
           state_to_string(sm->current_state),
           state_to_string(next_state),
           event_to_string(event));

    sm->current_state = next_state;
    return 1;
}

int main(void) {
    struct StateMachine sm;
    initialize_state_machine(&sm);

    printf("State Machine Demo\n");

    handle_event(&sm, EVENT_START);
    handle_event(&sm, EVENT_DATA_READY);
    handle_event(&sm, EVENT_PROCESS);
    handle_event(&sm, EVENT_SUCCESS);
    handle_event(&sm, EVENT_PROCESS);
    handle_event(&sm, EVENT_FAILURE);
    handle_event(&sm, EVENT_RESET);

    printf("Final counts - Processed: %u, Errors: %u\n",
           sm.context.processed_count, sm.context.error_count);

    cleanup_state_machine(&sm);
    return 0;
}