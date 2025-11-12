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
    EVENT_NONE,
    EVENT_INPUT_RECEIVED,
    EVENT_PROCESS_COMPLETE,
    EVENT_ERROR,
    EVENT_RESET
} event_t;

typedef struct {
    state_t current_state;
    char input_buffer[MAX_INPUT_LEN];
    size_t input_length;
    int processed_count;
} context_t;

static void initialize_context(context_t *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_START;
    memset(ctx->input_buffer, 0, sizeof(ctx->input_buffer));
    ctx->input_length = 0;
    ctx->processed_count = 0;
}

static event_t get_user_input(context_t *ctx) {
    if (ctx == NULL) return EVENT_ERROR;
    
    printf("Enter text (max %d chars, 'quit' to exit, 'reset' to restart): ", MAX_INPUT_LEN - 1);
    
    if (fgets(ctx->input_buffer, sizeof(ctx->input_buffer), stdin) == NULL) {
        return EVENT_ERROR;
    }
    
    ctx->input_length = strlen(ctx->input_buffer);
    if (ctx->input_length > 0 && ctx->input_buffer[ctx->input_length - 1] == '\n') {
        ctx->input_buffer[--ctx->input_length] = '\0';
    }
    
    if (ctx->input_length == 0) {
        return EVENT_NONE;
    }
    
    if (strcmp(ctx->input_buffer, "quit") == 0) {
        return EVENT_NONE;
    }
    
    if (strcmp(ctx->input_buffer, "reset") == 0) {
        return EVENT_RESET;
    }
    
    if (ctx->input_length >= MAX_INPUT_LEN - 1) {
        return EVENT_ERROR;
    }
    
    return EVENT_INPUT_RECEIVED;
}

static void process_input(context_t *ctx) {
    if (ctx == NULL || ctx->input_length == 0) return;
    
    printf("Processing: '");
    for (size_t i = 0; i < ctx->input_length; i++) {
        if (ctx->input_buffer[i] >= 'a' && ctx->input_buffer[i] <= 'z') {
            printf("%c", ctx->input_buffer[i] - 32);
        } else {
            printf("%c", ctx->input_buffer[i]);
        }
    }
    printf("'\n");
    
    ctx->processed_count++;
}

static state_t handle_state_start(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_INPUT_RECEIVED:
            return STATE_PROCESSING;
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

static state_t handle_state_reading(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_INPUT_RECEIVED:
            return STATE_PROCESSING;
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_READING;
    }
}

static state_t handle_state_processing(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS_COMPLETE:
            printf("Processed %d inputs so far\n", ctx->processed_count);
            return STATE_READING;
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            process_input(ctx);
            return STATE_DONE;
    }
}

static state_t handle_state_done(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_PROCESS_COMPLETE:
            return STATE_READING;
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        case EVENT_ERROR:
            return STATE_ERROR;
        default:
            return STATE_DONE;
    }
}

static state_t handle_state_error(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_RESET:
            initialize_context(ctx);
            return STATE_START;
        default:
            printf("Error state - enter 'reset' to continue\n");
            return STATE_ERROR;
    }
}

static state_t state_machine_step(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (ctx->current_state) {
        case STATE_START:
            return handle_state_start(ctx, event);
        case STATE_READING:
            return handle_state_reading(ctx, event);
        case STATE_PROCESSING:
            return handle_state_processing(ctx, event);
        case STATE_DONE:
            return handle_state_done(ctx, event);
        case STATE_ERROR:
            return handle_state_error(ctx, event);
        default:
            return STATE_ERROR;
    }
}

int main(void) {
    context_t ctx;