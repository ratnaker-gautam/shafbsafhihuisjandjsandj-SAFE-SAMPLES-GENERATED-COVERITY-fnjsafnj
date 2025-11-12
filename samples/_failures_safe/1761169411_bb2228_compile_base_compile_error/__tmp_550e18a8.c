//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State {
    STATE_IDLE,
    STATE_READING,
    STATE_PROCESSING,
    STATE_WRITING,
    STATE_ERROR,
    STATE_DONE
};

enum Event {
    EVENT_START,
    EVENT_DATA_READY,
    EVENT_PROCESS_COMPLETE,
    EVENT_WRITE_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
};

struct Context {
    enum State current_state;
    int data_buffer[16];
    int data_count;
    int processed_count;
    int max_operations;
};

void initialize_context(struct Context *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_IDLE;
    memset(ctx->data_buffer, 0, sizeof(ctx->data_buffer));
    ctx->data_count = 0;
    ctx->processed_count = 0;
    ctx->max_operations = 10;
}

int handle_idle_state(struct Context *ctx, enum Event event) {
    switch (event) {
        case EVENT_START:
            ctx->current_state = STATE_READING;
            return 1;
        case EVENT_RESET:
            initialize_context(ctx);
            return 1;
        default:
            return 0;
    }
}

int handle_reading_state(struct Context *ctx, enum Event event) {
    switch (event) {
        case EVENT_DATA_READY:
            if (ctx->data_count < 16) {
                ctx->data_buffer[ctx->data_count] = ctx->data_count * 2;
                ctx->data_count++;
                ctx->current_state = STATE_PROCESSING;
                return 1;
            } else {
                ctx->current_state = STATE_ERROR;
                return 0;
            }
        case EVENT_ERROR:
            ctx->current_state = STATE_ERROR;
            return 1;
        case EVENT_RESET:
            initialize_context(ctx);
            return 1;
        default:
            return 0;
    }
}

int handle_processing_state(struct Context *ctx, enum Event event) {
    switch (event) {
        case EVENT_PROCESS_COMPLETE:
            if (ctx->data_count > 0 && ctx->processed_count < ctx->data_count) {
                ctx->data_buffer[ctx->processed_count] *= 3;
                ctx->processed_count++;
                ctx->current_state = STATE_WRITING;
                return 1;
            } else {
                ctx->current_state = STATE_ERROR;
                return 0;
            }
        case EVENT_ERROR:
            ctx->current_state = STATE_ERROR;
            return 1;
        case EVENT_RESET:
            initialize_context(ctx);
            return 1;
        default:
            return 0;
    }
}

int handle_writing_state(struct Context *ctx, enum Event event) {
    switch (event) {
        case EVENT_WRITE_COMPLETE:
            if (ctx->processed_count <= ctx->data_count) {
                if (ctx->processed_count < ctx->max_operations) {
                    ctx->current_state = STATE_READING;
                } else {
                    ctx->current_state = STATE_DONE;
                }
                return 1;
            } else {
                ctx->current_state = STATE_ERROR;
                return 0;
            }
        case EVENT_ERROR:
            ctx->current_state = STATE_ERROR;
            return 1;
        case EVENT_RESET:
            initialize_context(ctx);
            return 1;
        default:
            return 0;
    }
}

int handle_error_state(struct Context *ctx, enum Event event) {
    switch (event) {
        case EVENT_RESET:
            initialize_context(ctx);
            return 1;
        default:
            return 0;
    }
}

int handle_done_state(struct Context *ctx, enum Event event) {
    switch (event) {
        case EVENT_RESET:
            initialize_context(ctx);
            return 1;
        default:
            return 0;
    }
}

int process_event(struct Context *ctx, enum Event event) {
    if (ctx == NULL) return 0;
    
    switch (ctx->current_state) {
        case STATE_IDLE:
            return handle_idle_state(ctx, event);
        case STATE_READING:
            return handle_reading_state(ctx, event);
        case STATE_PROCESSING:
            return handle_processing_state(ctx, event);
        case STATE_WRITING:
            return handle_writing_state(ctx, event);
        case STATE_ERROR:
            return handle_error_state(ctx, event);
        case STATE_DONE:
            return handle_done_state(ctx, event);
        default:
            return 0;
    }
}

const char* state_to_string(enum State state) {
    switch (state) {
        case STATE_IDLE: return "IDLE";
        case STATE_READING: return "READING";
        case STATE_PROCESSING: return "PROCESSING";
        case STATE_WRITING: return "WRITING";
        case STATE_ERROR: return "ERROR";
        case STATE_DONE: return "DONE";
        default: return "UNKNOWN";
    }
}

const char* event_to_string(enum Event event) {
    switch (event) {
        case EVENT_START: return "START";
        case EVENT_DATA_READY: return "DATA_READY";
        case EVENT_PROCESS_COMPLETE: return "PROCESS_COMPLETE";
        case EVENT_WRITE_COMPLETE: return "WRITE_COMPLETE";
        case EVENT_ERROR: return "ERROR";
        case EVENT_RESET: return "RESET";
        default: return "UNKNOWN";
    }
}

int main(void) {
    struct Context ctx;
    initialize_context(&ctx);