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

static void initialize_context(context_t *ctx);
static event_t get_next_event(void);
static int validate_input(const char *input, size_t len);
static void process_data(context_t *ctx);
static void display_results(const context_t *ctx);

static state_t handle_start(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started.\n");
            return STATE_READING;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_reading(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            if (ctx->buffer_len > 0 && ctx->buffer_len < MAX_INPUT_LEN) {
                if (validate_input(ctx->buffer, ctx->buffer_len)) {
                    ctx->data_count++;
                    return STATE_PROCESSING;
                }
            }
            return STATE_ERROR;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_processing(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS:
            process_data(ctx);
            if (ctx->data_count >= 3) {
                return STATE_FINISHED;
            }
            return STATE_READING;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_finished(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            display_results(ctx);
            return STATE_FINISHED;
        case EVENT_RESET:
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_error(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            printf("Resetting from error state.\n");
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

static void initialize_context(context_t *ctx) {
    if (ctx == NULL) return;
    
    ctx->current_state = STATE_START;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
    ctx->data_count = 0;
}

static event_t get_next_event(void) {
    static int step = 0;
    const event_t events[] = {
        EVENT_START,
        EVENT_DATA,
        EVENT_PROCESS,
        EVENT_DATA,
        EVENT_PROCESS,
        EVENT_DATA,
        EVENT_PROCESS,
        EVENT_COMPLETE,
        EVENT_RESET
    };
    
    if (step < (int)(sizeof(events) / sizeof(events[0]))) {
        return events[step++];
    }
    return EVENT_RESET;
}

static int validate_input(const char *input, size_t len) {
    if (input == NULL || len == 0 || len >= MAX_INPUT_LEN) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (input[i] < 32 || input[i] > 126) {
            return 0;
        }
    }
    
    return 1;
}

static void process_data(context_t *ctx) {
    if (ctx == NULL || ctx->buffer_len == 0) return;
    
    printf("Processing: ");
    for (size_t i = 0; i < ctx->buffer_len && i < MAX_INPUT_LEN; i++) {
        if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
            printf("%c", ctx->buffer[i] - 32);
        } else {
            printf("%c", ctx->buffer[i]);
        }
    }
    printf("\n");
    
    ctx->buffer_len = 0;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
}

static void display_results(const context_t *ctx) {
    if (ctx == NULL) return;
    
    printf("Processing complete. Processed %d data items.\n", ctx->data_count);
}

int main(void) {
    context_t ctx;
    initialize_context(&ctx);
    
    const char *test_inputs[] = {
        "hello",
        "world",