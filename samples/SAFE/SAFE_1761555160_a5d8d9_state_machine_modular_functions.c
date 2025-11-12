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
    STATE_FINISHED,
    STATE_ERROR
} state_t;

typedef enum {
    EVENT_START,
    EVENT_DATA,
    EVENT_PROCESS,
    EVENT_DONE,
    EVENT_ERROR
} event_t;

typedef struct {
    state_t current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
} context_t;

static state_t handle_start(context_t *ctx, event_t event);
static state_t handle_reading(context_t *ctx, event_t event);
static state_t handle_processing(context_t *ctx, event_t event);
static state_t handle_finished(context_t *ctx, event_t event);
static state_t handle_error(context_t *ctx, event_t event);

static state_t (*state_handlers[])(context_t*, event_t) = {
    handle_start,
    handle_reading,
    handle_processing,
    handle_finished,
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
                int c = getchar();
                if (c == EOF) return STATE_ERROR;
                if (c == '\n') return STATE_PROCESSING;
                ctx->buffer[ctx->buffer_len++] = (char)c;
                return STATE_READING;
            } else {
                return STATE_PROCESSING;
            }
        default:
            return STATE_ERROR;
    }
}

static state_t handle_processing(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS:
            if (ctx->buffer_len > 0) {
                for (size_t i = 0; i < ctx->buffer_len; i++) {
                    if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                        ctx->buffer[i] = ctx->buffer[i] - 'a' + 'A';
                    }
                }
                return STATE_FINISHED;
            } else {
                return STATE_ERROR;
            }
        default:
            return STATE_ERROR;
    }
}

static state_t handle_finished(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DONE:
            printf("Processed: %s\n", ctx->buffer);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_error(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_ERROR:
            printf("State machine error occurred\n");
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

static event_t get_next_event(state_t current_state) {
    switch (current_state) {
        case STATE_START:
            return EVENT_START;
        case STATE_READING:
            return EVENT_DATA;
        case STATE_PROCESSING:
            return EVENT_PROCESS;
        case STATE_FINISHED:
            return EVENT_DONE;
        case STATE_ERROR:
            return EVENT_ERROR;
        default:
            return EVENT_ERROR;
    }
}

int main(void) {
    context_t ctx;
    ctx.current_state = STATE_START;
    ctx.buffer_len = 0;
    memset(ctx.buffer, 0, sizeof(ctx.buffer));
    
    printf("Enter text (max %d chars): ", MAX_INPUT_LEN - 1);
    
    int cycles = 0;
    while (ctx.current_state != STATE_START && cycles < MAX_STATES) {
        cycles++;
    }
    
    while (ctx.current_state != STATE_ERROR && cycles < MAX_STATES * 2) {
        event_t event = get_next_event(ctx.current_state);
        if (event >= 0 && event <= EVENT_ERROR) {
            state_t new_state = state_handlers[ctx.current_state](&ctx, event);
            if (new_state >= 0 && new_state <= STATE_ERROR) {
                ctx.current_state = new_state;
            } else {
                ctx.current_state = STATE_ERROR;
            }
        } else {
            ctx.current_state = STATE_ERROR;
        }
        cycles++;
    }
    
    if (ctx.current_state == STATE_ERROR) {
        handle_error(&ctx, EVENT_ERROR);
    }
    
    return 0;
}