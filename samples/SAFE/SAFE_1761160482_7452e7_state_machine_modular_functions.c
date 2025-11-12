//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: state_machine
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_INPUT_LEN 100
#define MAX_STATES 10

typedef enum {
    STATE_START,
    STATE_READING,
    STATE_PROCESSING,
    STATE_DONE,
    STATE_ERROR
} state_t;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
} event_t;

typedef struct {
    state_t current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
} context_t;

static state_t handle_start(context_t *ctx, event_t event);
static state_t handle_reading(context_t *ctx, event_t event);
static state_t handle_processing(context_t *ctx, event_t event);
static state_t handle_done(context_t *ctx, event_t event);
static state_t handle_error(context_t *ctx, event_t event);

static state_t (*state_handlers[])(context_t*, event_t) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_done,
    handle_error
};

static state_t handle_start(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            ctx->buffer_len = 0;
            memset(ctx->buffer, 0, sizeof(ctx->buffer));
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_reading(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (ctx->buffer_len < MAX_INPUT_LEN - 1) {
                ctx->buffer[ctx->buffer_len] = 'A';
                ctx->buffer_len++;
                ctx->buffer[ctx->buffer_len] = '\0';
                return STATE_READING;
            } else {
                return STATE_PROCESSING;
            }
        case EVENT_COMPLETE:
            return STATE_PROCESSING;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

static state_t handle_processing(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            return STATE_DONE;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_PROCESSING;
    }
}

static state_t handle_done(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_DONE;
    }
}

static state_t handle_error(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

static void process_state_machine(context_t *ctx) {
    if (ctx == NULL) return;
    
    state_t next_state = ctx->current_state;
    int steps = 0;
    const int max_steps = 20;
    
    while (steps < max_steps && next_state != STATE_DONE && next_state != STATE_ERROR) {
        event_t event;
        
        if (next_state == STATE_START) {
            event = EVENT_START;
        } else if (next_state == STATE_READING) {
            if (ctx->buffer_len >= 5) {
                event = EVENT_COMPLETE;
            } else {
                event = EVENT_DATA;
            }
        } else if (next_state == STATE_PROCESSING) {
            event = EVENT_COMPLETE;
        } else {
            event = EVENT_ERROR;
        }
        
        if (next_state < MAX_STATES && state_handlers[next_state] != NULL) {
            next_state = state_handlers[next_state](ctx, event);
        } else {
            next_state = STATE_ERROR;
        }
        
        ctx->current_state = next_state;
        steps++;
    }
}

static void print_state_info(const context_t *ctx) {
    if (ctx == NULL) return;
    
    const char *state_names[] = {
        "START", "READING", "PROCESSING", "DONE", "ERROR"
    };
    
    if (ctx->current_state < sizeof(state_names)/sizeof(state_names[0])) {
        printf("Current state: %s\n", state_names[ctx->current_state]);
    }
    
    if (ctx->buffer_len > 0) {
        printf("Buffer content: %s\n", ctx->buffer);
        printf("Buffer length: %zu\n", ctx->buffer_len);
    }
}

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_START;
    ctx.buffer_len = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    printf("State Machine Simulation\n");
    printf("Starting state machine...\n");
    
    process_state_machine(&ctx);
    
    print_state_info(&ctx);
    
    if (ctx.current_state == STATE_DONE) {
        printf("State machine completed successfully.\n");
    } else if (ctx.current_state == STATE_ERROR) {
        printf("State machine encountered an error.\n");
    } else {
        printf("State machine did not reach terminal state.\n");
    }
    
    return 0;
}