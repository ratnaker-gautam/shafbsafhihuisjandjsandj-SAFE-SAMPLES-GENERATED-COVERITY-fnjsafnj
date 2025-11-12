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

static void initialize_context(context_t *ctx) {
    if (ctx == NULL) return;
    ctx->current_state = STATE_START;
    memset(ctx->buffer, 0, sizeof(ctx->buffer));
    ctx->buffer_len = 0;
}

static int validate_input(const char *input, size_t len) {
    if (input == NULL) return 0;
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (input[i] == '\0') return 0;
    }
    return 1;
}

static event_t get_next_event(context_t *ctx) {
    if (ctx == NULL) return EVENT_ERROR;
    
    switch (ctx->current_state) {
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

static state_t handle_start(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_START:
            printf("State machine started.\n");
            return STATE_READING;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_reading(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DATA:
            printf("Enter data (max %d characters): ", MAX_INPUT_LEN - 1);
            if (fgets(ctx->buffer, sizeof(ctx->buffer), stdin) != NULL) {
                ctx->buffer_len = strlen(ctx->buffer);
                if (ctx->buffer_len > 0 && ctx->buffer[ctx->buffer_len - 1] == '\n') {
                    ctx->buffer[ctx->buffer_len - 1] = '\0';
                    ctx->buffer_len--;
                }
                if (validate_input(ctx->buffer, ctx->buffer_len)) {
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
            printf("Processing data: ");
            for (size_t i = 0; i < ctx->buffer_len; i++) {
                if (ctx->buffer[i] >= 'a' && ctx->buffer[i] <= 'z') {
                    printf("%c", ctx->buffer[i] - 32);
                } else {
                    printf("%c", ctx->buffer[i]);
                }
            }
            printf("\n");
            return STATE_FINISHED;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_finished(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_DONE:
            printf("Processing complete.\n");
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

static state_t handle_error(context_t *ctx, event_t event) {
    if (ctx == NULL) return STATE_ERROR;
    
    switch (event) {
        case EVENT_ERROR:
            printf("Error occurred. Resetting state machine.\n");
            return STATE_START;
        default:
            return STATE_ERROR;
    }
}

static state_t process_event(context_t *ctx, event_t event) {
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

int main(void) {
    context_t ctx;
    initialize_context(&ctx);
    
    int cycles = 0;
    const int max_cycles = 5;
    
    while (cycles < max_cycles) {
        event_t event = get_next_event(&ctx);
        state_t new_state = process_event(&ctx, event);
        
        if (new_state == STATE_ERROR) {
            printf("Invalid state transition detected.\n");
            break;