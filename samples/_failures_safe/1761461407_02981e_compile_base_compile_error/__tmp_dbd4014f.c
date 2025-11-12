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
static event_t get_next_event(context_t *ctx);
static int validate_input(const char *input, size_t len);
static void process_data(context_t *ctx);
static void reset_context(context_t *ctx);

static state_t handle_start(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started\n");
            return STATE_READING;
        case EVENT_RESET:
            reset_context(ctx);
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
            reset_context(ctx);
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
            reset_context(ctx);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_finished(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_COMPLETE:
            printf("Processing complete. Processed %d data items\n", ctx->data_count);
            return STATE_FINISHED;
        case EVENT_RESET:
            reset_context(ctx);
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_error(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            reset_context(ctx);
            return STATE_START;
        default:
            printf("Error state - reset required\n");
            return STATE_ERROR;
    }
}

static void initialize_context(context_t *ctx) {
    if (ctx == NULL) return;
    
    memset(ctx, 0, sizeof(context_t));
    ctx->current_state = STATE_START;
}

static event_t get_next_event(context_t *ctx) {
    static int step = 0;
    char input[MAX_INPUT_LEN];
    
    if (step == 0) {
        step++;
        return EVENT_START;
    }
    
    if (step >= 10) {
        return EVENT_COMPLETE;
    }
    
    printf("Enter data (or 'reset' to restart): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (strcmp(input, "reset") == 0) {
        return EVENT_RESET;
    }
    
    if (len > 0 && len < MAX_INPUT_LEN) {
        if (ctx != NULL) {
            strncpy(ctx->buffer, input, MAX_INPUT_LEN - 1);
            ctx->buffer[MAX_INPUT_LEN - 1] = '\0';
            ctx->buffer_len = len;
        }
        return EVENT_DATA;
    }
    
    return EVENT_ERROR;
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
    for (size_t i = 0; i < ctx->buffer_len && i < MAX_INPUT