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
    EVENT_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
} event_t;

typedef struct {
    state_t current_state;
    char buffer[MAX_INPUT_LEN];
    size_t buffer_len;
    int data_count;
} context_t;

static state_t handle_start(context_t *ctx, event_t event);
static state_t handle_reading(context_t *ctx, event_t event);
static state_t handle_processing(context_t *ctx, event_t event);
static state_t handle_finished(context_t *ctx, event_t event);
static state_t handle_error(context_t *ctx, event_t event);

static void initialize_context(context_t *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_START;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
    ctx->data_count = 0;
}

static int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == 0) break;
        if (input[i] < 32 || input[i] > 126) return 0;
    }
    return 1;
}

static event_t get_next_event(context_t *ctx) {
    if (ctx == NULL) return EVENT_ERROR;
    
    static int call_count = 0;
    call_count++;
    
    if (call_count > 20) return EVENT_COMPLETE;
    
    switch (ctx->current_state) {
        case STATE_START:
            return EVENT_START;
        case STATE_READING:
            if (ctx->buffer_len > 0 && ctx->buffer_len < MAX_INPUT_LEN - 1) {
                return EVENT_DATA;
            }
            return EVENT_ERROR;
        case STATE_PROCESSING:
            if (ctx->data_count < 3) {
                return EVENT_PROCESS;
            }
            return EVENT_COMPLETE;
        case STATE_FINISHED:
            return EVENT_RESET;
        case STATE_ERROR:
            return EVENT_RESET;
        default:
            return EVENT_ERROR;
    }
}

static state_t transition_state(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (ctx->current_state) {
        case STATE_START:
            return handle_start(ctx, event);
        case STATE_READING:
            return handle_reading(ctx, event);
        case STATE_PROCESSING:
            return handle_processing(ctx, event);
        case STATE_FINISHED:
            return handle_finished(ctx, event);
        case STATE_ERROR:
            return handle_error(ctx, event);
        default:
            return STATE_ERROR;
    }
}

static state_t handle_start(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started.\n");
            ctx->buffer_len = 0;
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
                const char *sample_data = "sample";
                size_t data_len = strlen(sample_data);
                if (ctx->buffer_len + data_len < MAX_INPUT_LEN) {
                    memcpy(ctx->buffer + ctx->buffer_len, sample_data, data_len);
                    ctx->buffer_len += data_len;
                    ctx->buffer[ctx->buffer_len] = '\0';
                    printf("Read data: %s\n", ctx->buffer);
                    return STATE_PROCESSING;
                }
            }
            return STATE_ERROR;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_processing(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS:
            if (ctx->data_count < 3) {
                ctx->data_count++;
                printf("Processing data %d/3: %s\n", ctx->data_count, ctx->buffer);
                return STATE_PROCESSING;
            }
            return STATE_FINISHED;
        case EVENT_COMPLETE:
            return STATE_FINISHED;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_finished(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Processing complete. Resetting state machine.\n");
            initialize_context(ctx);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_error(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Error occurred. Resetting state machine.\n");
            initialize_context